#include <cstdio>
#include <cstdarg>
#include <iostream>
#include "registers.h"
#include "PciDevice.h"
#include "register_struct.h"

using std::string;

// Manages FPGA registers
CRegisters fpga;

// The PCI bus
PciDevice PCI;

// SMEM is exactly one megabyte
unsigned char smem_space[1024*1024];

// Command line options
struct opt_t
{
    bool    abm_order  = true;
    string  filename   = "smem.bin";
} opt;


// This is every register this program cares about
registers_t reg;

// Function prototypes
void execute();
void parse_command_line(const char** argv);

int main(int argc, const char** argv)
{
    try
    {
        parse_command_line(argv);
        execute();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        exit(1);
    }
    
    return 0;
}

//=================================================================================================
// parse_command_line() - Parse the command-line options
//=================================================================================================
void parse_command_line(const char** argv)
{
    int i=1;
    string token;

    while (argv[i])
    {
        token = argv[i++];

        if (token == "-abm")
        {
            opt.abm_order = true;
            continue;
        }

        if (token == "-smem")
        {
            opt.abm_order = false;
            continue;
        }

        if (token == "-file" && argv[i])
        {
            opt.filename = argv[i++];
            continue;
        }
    }

    // If we made it to the end of the command-line options, all is well
    if (argv[i] == nullptr) return;

    // If we get here, we discovered an invalid option
    fprintf(stderr, "bad command line option %s\n", token.c_str());
    exit(1);
}
//=================================================================================================


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
// fetch_row() - Fetches a single row of data from SMEM
//=================================================================================================
void fetch_row(int row, int bank, unsigned char* out)
{
    // Compute the chip address of the first word of this row of data
    uint32_t chip_addr = 0x8000 + bank*0x20000 + row*256;    

    // Tell the FPGA the sensor-chip address we want to start at
    fpga.write(reg.REG_CHIPIO_ADDR, chip_addr);
    
    // Read 64 32-bit words and save them to our buffer
    for (int i=0; i<64; ++i)
    {
        // Fetch the next 32-bit word
        uint32_t value = fpga.read(reg.REG_CHIPIO_DATA_INCR);

        // Write the four bytes to the buffer
        *out++ = (value >> 24) & 0xFF;
        *out++ = (value >> 16) & 0xFF;
        *out++ = (value >>  8) & 0xFF;
        *out++ = (value      ) & 0xFF;
    }
}
//=================================================================================================


//=================================================================================================
// fetch_in_smem_order() - Fetch the contents of SMEM in ABM row order (i.e., row first)
//=================================================================================================
void fetch_in_smem_order()
{
    unsigned char* out = smem_space;

    for (int bank = 0; bank < 8; ++bank)
    {
        for (int row = 0; row < 512; ++row)
        {
            fetch_row(row, bank, out);
            out += 0x100;
        }
    }    
}
//=================================================================================================




//=================================================================================================
// fetch_in_abm_order() - Fetch the contents of SMEM in SMEM row order (i.e., bank first)
//                        32-bit words in the ABM are in little-endian byte order
//=================================================================================================
void fetch_in_abm_order()
{
    unsigned char* out = smem_space;

    for (int row = 0; row < 512; ++row)
    {
        for (int bank = 0; bank < 8; ++ bank)
        {
            fetch_row(row, bank, out);
            out += 0x100;
        }
    }    
}
//=================================================================================================



//=================================================================================================
// execute() - The is the mainline program execution
//=================================================================================================
void execute()
{
    const char* filename = "fpga_regs.h";

    // Read our definitions file
    if (!read_register_definitions(reg, filename))
        throwRuntime("file not found: %s", filename);

    // Open a connection to our PCI device
    PCI.open("10ee:903f");

    // Tell our registers what their base address in userspace is
    fpga.set_base_addr(PCI.resourceList()[0].baseAddr);

    // Fetch the contents of smem into "smem_space"
    if (opt.abm_order)
        fetch_in_abm_order();
    else
        fetch_in_smem_order();

    // Now write out the file
    const char* fn = opt.filename.c_str();
    FILE* ofile = fopen(fn, "w");
    if (ofile == nullptr) throwRuntime("Can't create %s", fn);
    fwrite(smem_space, 1, 1024*1024, ofile);
    fclose(ofile);
}
//=================================================================================================
