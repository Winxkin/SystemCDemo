#include <systemc>
#include <iostream>
#include "RISCV32/decode_inst.h"
#include "06_core/riscv32_inst_mix.h"

using namespace isa32;

inline void tp06_02_test1()
{
    uint32_t instruction = 0xE002E037;

    ISAFORMAT isadeocde;
    isadeocde = decode_ISA(instruction);
    dump_isa_info(isadeocde);

    instruction = 0x00030613;

    isadeocde = decode_ISA(instruction);
    dump_isa_info(isadeocde);

    instruction = 0x00100313;

    isadeocde = decode_ISA(instruction);
    dump_isa_info(isadeocde);

    instruction = 0x00628223;

    isadeocde = decode_ISA(instruction);
    dump_isa_info(isadeocde);

    instruction = 0x00008067;

    isadeocde = decode_ISA(instruction);
    dump_isa_info(isadeocde);
}