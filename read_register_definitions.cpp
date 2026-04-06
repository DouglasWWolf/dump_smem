//========================================
//       This is generated code!
//========================================
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include "register_struct.h"

static void eval(registers_t& reg, const char* name, uint64_t descriptor)
{
    if (strcmp(name, "REG_LOAD_ABM_VIA_PCI"        ) == 0) {reg.REG_LOAD_ABM_VIA_PCI         = descriptor; return;}
    if (strcmp(name, "REG_LOAD_ABM_VIA_PCI_load_0" ) == 0) {reg.REG_LOAD_ABM_VIA_PCI_load_0  = descriptor; return;}
    if (strcmp(name, "REG_LOAD_ABM_VIA_PCI_load_1" ) == 0) {reg.REG_LOAD_ABM_VIA_PCI_load_1  = descriptor; return;}
    if (strcmp(name, "REG_ABM_PCI_SRC_ADDR"        ) == 0) {reg.REG_ABM_PCI_SRC_ADDR         = descriptor; return;}
    if (strcmp(name, "REG_SELECT_HSI"              ) == 0) {reg.REG_SELECT_HSI               = descriptor; return;}
    if (strcmp(name, "REG_FORCE_SMEM"              ) == 0) {reg.REG_FORCE_SMEM               = descriptor; return;}
    if (strcmp(name, "REG_CHIPIO_ADDR"             ) == 0) {reg.REG_CHIPIO_ADDR              = descriptor; return;}
    if (strcmp(name, "REG_CHIPIO_DATA"             ) == 0) {reg.REG_CHIPIO_DATA              = descriptor; return;}
    if (strcmp(name, "REG_CHIPIO_DATA_INCR"        ) == 0) {reg.REG_CHIPIO_DATA_INCR         = descriptor; return;}
    if (strcmp(name, "REG_CHIP_SIM_SELECT"         ) == 0) {reg.REG_CHIP_SIM_SELECT          = descriptor; return;}
    if (strcmp(name, "REG_SMEM_BUSY"               ) == 0) {reg.REG_SMEM_BUSY                = descriptor; return;}
    if (strcmp(name, "REG_SMEM_ROWS_UPD"           ) == 0) {reg.REG_SMEM_ROWS_UPD            = descriptor; return;}
    if (strcmp(name, "REG_SMEM_WORDS_UPD"          ) == 0) {reg.REG_SMEM_WORDS_UPD           = descriptor; return;}
    if (strcmp(name, "REG_ABM_COUNT"               ) == 0) {reg.REG_ABM_COUNT                = descriptor; return;}
    if (strcmp(name, "REG_HS_CLK_REF"              ) == 0) {reg.REG_HS_CLK_REF               = descriptor; return;}
    if (strcmp(name, "REG_HS_CLK_VCO"              ) == 0) {reg.REG_HS_CLK_VCO               = descriptor; return;}
    if (strcmp(name, "REG_HS_CLK_VCO_divclk_divide") == 0) {reg.REG_HS_CLK_VCO_divclk_divide = descriptor; return;}
    if (strcmp(name, "REG_HS_CLK_VCO_clkfbout_mult") == 0) {reg.REG_HS_CLK_VCO_clkfbout_mult = descriptor; return;}
    if (strcmp(name, "REG_HS_CLK_VCO_clkfbout_frac") == 0) {reg.REG_HS_CLK_VCO_clkfbout_frac = descriptor; return;}
    if (strcmp(name, "REG_HS_CLK_DIV"              ) == 0) {reg.REG_HS_CLK_DIV               = descriptor; return;}
    if (strcmp(name, "REG_HS_CLK_DIV_clkout_divide") == 0) {reg.REG_HS_CLK_DIV_clkout_divide = descriptor; return;}
    if (strcmp(name, "REG_HS_CLK_DIV_clkout_frac"  ) == 0) {reg.REG_HS_CLK_DIV_clkout_frac   = descriptor; return;}
    if (strcmp(name, "REG_HS_CLK_FREQ"             ) == 0) {reg.REG_HS_CLK_FREQ              = descriptor; return;}
};

//=============================================================================
// read_register_definitions() - Reads in an FPGA register definiton file and
//                               fills in a registers_t structure with the
//                               values it finds there
//=============================================================================
bool read_register_definitions(registers_t& reg, std::string filename)
{
    char line[1000], *in, *out;
    char name[1000];

    // Open the input file
    FILE* ifile = fopen(filename.c_str(), "r");

    // Complain if we can't
    if (ifile == nullptr) return false;

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
        uint64_t descriptor = strtoull(in, nullptr, 0);

        // Fill in the appropriate field in our register structure
        eval(reg, name, descriptor);
    }

    // We're done with the input file
    fclose(ifile);
    return true;
}
//=============================================================================
