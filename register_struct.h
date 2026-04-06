#pragma once
//========================================
//       This is generated code!
//========================================
#include <cstdint>

struct registers_t
{
    std::uint64_t REG_LOAD_ABM_VIA_PCI         = 0xFFFFFFFF;
    std::uint64_t REG_LOAD_ABM_VIA_PCI_load_0  = 0xFFFFFFFF;
    std::uint64_t REG_LOAD_ABM_VIA_PCI_load_1  = 0xFFFFFFFF;
    std::uint64_t REG_ABM_PCI_SRC_ADDR         = 0xFFFFFFFF;
    std::uint64_t REG_SELECT_HSI               = 0xFFFFFFFF;
    std::uint64_t REG_FORCE_SMEM               = 0xFFFFFFFF;
    std::uint64_t REG_CHIPIO_ADDR              = 0xFFFFFFFF;
    std::uint64_t REG_CHIPIO_DATA              = 0xFFFFFFFF;
    std::uint64_t REG_CHIPIO_DATA_INCR         = 0xFFFFFFFF;
    std::uint64_t REG_CHIP_SIM_SELECT          = 0xFFFFFFFF;
    std::uint64_t REG_SMEM_BUSY                = 0xFFFFFFFF;
    std::uint64_t REG_SMEM_ROWS_UPD            = 0xFFFFFFFF;
    std::uint64_t REG_SMEM_WORDS_UPD           = 0xFFFFFFFF;
    std::uint64_t REG_ABM_COUNT                = 0xFFFFFFFF;
    std::uint64_t REG_HS_CLK_REF               = 0xFFFFFFFF;
    std::uint64_t REG_HS_CLK_VCO               = 0xFFFFFFFF;
    std::uint64_t REG_HS_CLK_VCO_divclk_divide = 0xFFFFFFFF;
    std::uint64_t REG_HS_CLK_VCO_clkfbout_mult = 0xFFFFFFFF;
    std::uint64_t REG_HS_CLK_VCO_clkfbout_frac = 0xFFFFFFFF;
    std::uint64_t REG_HS_CLK_DIV               = 0xFFFFFFFF;
    std::uint64_t REG_HS_CLK_DIV_clkout_divide = 0xFFFFFFFF;
    std::uint64_t REG_HS_CLK_DIV_clkout_frac   = 0xFFFFFFFF;
    std::uint64_t REG_HS_CLK_FREQ              = 0xFFFFFFFF;
};

bool read_register_definitions(registers_t& reg, std::string filename = "fpga_regs.h");
