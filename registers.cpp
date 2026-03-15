#include "registers.h"
#include <cstring>
#include <cstdarg>
#include <stdexcept>

//=================================================================================================
// throwRuntime() - Throws a runtime exception
//=================================================================================================
static void throwRuntime(const char* fmt, ...)
{
    char buffer[1024];
    va_list ap;
    va_start(ap, fmt);
    vsprintf(buffer, fmt, ap);
    va_end(ap);

    throw std::runtime_error(buffer);
}
//=================================================================================================


//=================================================================================================
// read_definitions() - Reads in an FPGA register definiton file and builds a map that maps
//                      a register name to a register address
//=================================================================================================
void CRegisters::read_definitions(std::string s, bool append)
{
    char line[1000], *in, *out;
    char name[1000];

    // If we're not appending to our existing map, clear the map
    if (!append) map_.clear();

    // Open the input file
    FILE* ifile = fopen(s.c_str(), "r");

    // Complain if we can't
    if (ifile == nullptr) throwRuntime("File not found: %s", s.c_str());

    // Loop through every line of the input file
    while (fgets(line, sizeof(line), ifile))
    {
        // Skip past any leading whitespace
        in = line;
        while (*in == ' ' || *in == '\t') ++in;
        
        // Ignore any line that doesn't begin with "#define"
        if (strncmp(in, "#define", 7) != 0) continue;
        
        // Skip past the #define
        in = in + 7;

        // Skip past whitespace after #define
        while (*in == ' ' || *in == '\t') ++in;
        
        // Point to the name field
        out = name;

        // Copy the token to "name[]" and nul-terminate it
        while (*in != ' ' && *in != '\t' && *in != 10 && *in != 13) *out++ = *in++;
        *out = 0;

        // If we've hit the end of the line, this isn't a register definition
        if (*in == 10 || *in == 13) continue;

        // Skip past whitespace after register name
        while (*in == ' ' || *in == '\t') ++in;
        
        // Extract the address of the register
        uint64_t address = strtoull(in, nullptr, 0);

        // Store the register name and address in our map
        map_[name] = address;
    }

    // We're done with the input file
    fclose(ifile);
}
//=================================================================================================



//=================================================================================================
// get_address() - Returns the address that corresponds to a given register name
//=================================================================================================
uint64_t CRegisters::get_address(std::string s)
{
    // Find the register name
    auto it = map_.find(s);
    
    // Complain if we can't
    if (it == map_.end()) throwRuntime("Unknown register %s", s.c_str());

    // Hand the corresponding register address to the caller
    return it->second;
}
//=================================================================================================



//=================================================================================================
// write() - Store a 32-bit or 64-bit value
//=================================================================================================
void CRegisters::write(uint64_t reg, uint64_t value)
{
    // Break the register definition into an offset and type
    uint32_t reg_offset = reg & 0xFFFFFFFF;
    uint32_t reg_type   = reg >> 32;

    // Store an ordinary 32-bit value
    if (reg_type == 0x00000000 || reg_type == 0x20000000)
    {
        *(uint32_t*)(base_addr_ + reg_offset) = (uint32_t)value;
        return;
    }

    // Store an ordinary 64-bit value
    if (reg_type == 0x40000000)
    {
        *(uint32_t*)(base_addr_ + reg_offset + 0) = (uint32_t)(value >> 32);
        *(uint32_t*)(base_addr_ + reg_offset + 4) = (uint32_t)value;
        return;
    }

    // If we get here, we didn't understand the register type
    throwRuntime("Bad register 0x%lX", reg);    
}
//=================================================================================================


//=================================================================================================
// read() - Returns a 32-bit or 64-bit value from a register
//=================================================================================================
uint64_t CRegisters::read(uint64_t reg)
{
    // Break the register definition into an offset and type
    uint32_t reg_offset = reg & 0xFFFFFFFF;
    uint32_t reg_type   = reg >> 32;

    // Read an ordinary 32-bit value
    if (reg_type == 0x00000000 || reg_type == 0x20000000)
    {
        return *(uint32_t*)(base_addr_ + reg_offset);
    }

    // Read an ordinary 64-bit value
    if (reg_type == 0x40000000)
    {
        uint64_t hi = *(uint32_t*)(base_addr_ + reg_offset + 0);
        uint64_t lo = *(uint32_t*)(base_addr_ + reg_offset + 4);
        return (hi << 32) | lo;
    }

    // If we get here, we didn't understand the register type
    throwRuntime("Bad register 0x%lX", reg);

    // This is just here to keep the compiler happy
    return 0;
}
//=================================================================================================
