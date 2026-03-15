#pragma once
#include <stdint.h>
#include <string>
#include <map>


class CRegisters
{
public:


    // Set the base address of the memory region where our registers live
    void    set_base_addr(unsigned char* base_addr) {base_addr_ = base_addr;}
    
    // Reads in the register definitions
    void    read_definitions(std::string s, bool append = false);

    // Fetch the address of the specified register
    uint64_t get_address(std::string s);

    // Return a pointer to the specified register
    uint32_t* get_ptr(uint64_t reg);

    // Write a register
    void    write(uint64_t reg, uint64_t value);

    // Read a register
    uint64_t read(uint64_t reg);


protected:

    // This maps a register name to a register value
    std::map<std::string, uint64_t> map_;

    // The base address where our registers live
    unsigned char* base_addr_;
};
