#include <cstdio>
#include <iostream>
#include "registers.h"


// Manages FPGA registers
CRegisters registers;

// This is every register this program cares about
struct reg_t
{
    uint64_t REG_CHIPIO_ADDR;
    uint64_t REG_CHIPIO_DATA_INCR;
};


reg_t reg;

unsigned char region[1024 * 1024];

int main(int argc, const char** argv)
{
    try
    {
        registers.read_definitions("fpga_regs.h");
        registers.set_base_addr(region);
        reg.REG_CHIPIO_ADDR      = registers.get_address("REG_CHIPIO_ADDR");
        reg.REG_CHIPIO_DATA_INCR = registers.get_address("REG_CHIPIO_DATA_INCR"); 
        
        registers.write(reg.REG_CHIPIO_DATA_INCR, 0xAABBCCDD);
        uint32_t v = registers.read(reg.REG_CHIPIO_DATA_INCR);
        printf("We see 0x%X\n", v);

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        exit(1);
    }
    
    return 0;
}
