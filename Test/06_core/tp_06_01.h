#include <systemc>
#include <iostream>
#include "RISCV32\commondef.h"
#include "06_core/riscv32_inst_mix.h"

using namespace riscv32;

inline void tp06_01_r_type_test()
{
    uint32_t instruction_r_type = r_type_inst_mix(0b0110011, 0x0, 0x0, 0x0, 0x8, 0x3);

    ISAFORMAT isadeocde;
    isadeocde = decode_ISA(instruction_r_type);
    dump_isa_info(isadeocde);

    instruction_r_type = r_type_inst_mix(0b0110011, 0x0, 0x0, 0x20, 0x8, 0x3);

    isadeocde = decode_ISA(instruction_r_type);
    dump_isa_info(isadeocde);

    instruction_r_type = r_type_inst_mix(0b0110011, 0x0, 0x04, 0x0, 0x8, 0x3);

    isadeocde = decode_ISA(instruction_r_type);
    dump_isa_info(isadeocde);

    instruction_r_type = r_type_inst_mix(0b0110011, 0x0, 0x06, 0x0, 0x8, 0x3);

    isadeocde = decode_ISA(instruction_r_type);
    dump_isa_info(isadeocde);

    instruction_r_type = r_type_inst_mix(0b0110011, 0x0, 0x07, 0x0, 0x8, 0x3);

    isadeocde = decode_ISA(instruction_r_type);
    dump_isa_info(isadeocde);

    instruction_r_type = r_type_inst_mix(0b0110011, 0x0, 0x01, 0x0, 0x8, 0x3);

    isadeocde = decode_ISA(instruction_r_type);
    dump_isa_info(isadeocde);


    instruction_r_type = r_type_inst_mix(0b0110011, 0x0, 0x05, 0x0, 0x8, 0x3);

    isadeocde = decode_ISA(instruction_r_type);
    dump_isa_info(isadeocde);

    instruction_r_type = r_type_inst_mix(0b0110011, 0x0, 0x05, 0x20, 0x8, 0x3);

    isadeocde = decode_ISA(instruction_r_type);
    dump_isa_info(isadeocde);

    instruction_r_type = r_type_inst_mix(0b0110011, 0x0, 0x2, 0x00, 0x8, 0x3);

    isadeocde = decode_ISA(instruction_r_type);
    dump_isa_info(isadeocde);

    instruction_r_type = r_type_inst_mix(0b0110011, 0x0, 0x3, 0x00, 0x8, 0x3);

    isadeocde = decode_ISA(instruction_r_type);
    dump_isa_info(isadeocde);

}

inline void tp06_01_i_type_test()
{
    uint32_t instruction_i_type = i_type_inst_mix(0b0010011, 0x0, 0x0, 0x0, 0x8);

    ISAFORMAT isadeocde;
    isadeocde = decode_ISA(instruction_i_type);
    dump_isa_info(isadeocde);

    instruction_i_type = i_type_inst_mix(0b0010011, 0x0, 0x4, 0x20, 0x8);

    isadeocde = decode_ISA(instruction_i_type);
    dump_isa_info(isadeocde);

    instruction_i_type = i_type_inst_mix(0b0010011, 0x0, 0x06, 0x0, 0x8);

    isadeocde = decode_ISA(instruction_i_type);
    dump_isa_info(isadeocde);

    instruction_i_type = i_type_inst_mix(0b0010011, 0x0, 0x07, 0x0, 0x8);

    isadeocde = decode_ISA(instruction_i_type);
    dump_isa_info(isadeocde);

    instruction_i_type = i_type_inst_mix(0b0010011, 0x0, 0x01, 0x0, 0x8);

    isadeocde = decode_ISA(instruction_i_type);
    dump_isa_info(isadeocde);

    instruction_i_type = i_type_inst_mix(0b0010011, 0x0, 0x05, 0x20 << 5, 0x8);

    isadeocde = decode_ISA(instruction_i_type);
    dump_isa_info(isadeocde);


    instruction_i_type = i_type_inst_mix(0b0010011, 0x0, 0x05, 0x0, 0x8);

    isadeocde = decode_ISA(instruction_i_type);
    dump_isa_info(isadeocde);

    instruction_i_type = i_type_inst_mix(0b0010011, 0x0, 0x02, 0x20, 0x8);

    isadeocde = decode_ISA(instruction_i_type);
    dump_isa_info(isadeocde);

    instruction_i_type = i_type_inst_mix(0b0010011, 0x0, 0x3, 0x00, 0x8);

    isadeocde = decode_ISA(instruction_i_type);
    dump_isa_info(isadeocde);

    instruction_i_type = i_type_inst_mix(0b0000011, 0x0, 0x0, 0x00, 0x8);

    isadeocde = decode_ISA(instruction_i_type);
    dump_isa_info(isadeocde);
    instruction_i_type = i_type_inst_mix(0b0000011, 0x0, 0x1, 0x00, 0x8);

    isadeocde = decode_ISA(instruction_i_type);
    dump_isa_info(isadeocde);
    instruction_i_type = i_type_inst_mix(0b0000011, 0x0, 0x2, 0x00, 0x8);

    isadeocde = decode_ISA(instruction_i_type);
    dump_isa_info(isadeocde);

    instruction_i_type = i_type_inst_mix(0b0000011, 0x0, 0x4, 0x00, 0x8);

    isadeocde = decode_ISA(instruction_i_type);
    dump_isa_info(isadeocde);

    instruction_i_type = i_type_inst_mix(0b0000011, 0x0, 0x5, 0x00, 0x8);

    isadeocde = decode_ISA(instruction_i_type);
    dump_isa_info(isadeocde);

    instruction_i_type = i_type_inst_mix(0b1100111, 0x0, 0x0, 0x00, 0x8);

    isadeocde = decode_ISA(instruction_i_type);
    dump_isa_info(isadeocde);

    instruction_i_type = i_type_inst_mix(0b1110011, 0x0, 0x0, 0x00, 0x8);

    isadeocde = decode_ISA(instruction_i_type);
    dump_isa_info(isadeocde);
    instruction_i_type = i_type_inst_mix(0b1110011, 0x0, 0x0, 0x01, 0x8);

    isadeocde = decode_ISA(instruction_i_type);
    dump_isa_info(isadeocde);

}

inline void tp06_01_s_type_test()
{
    uint32_t instruction_s_type = s_type_inst_mix(0b0100011, 0x0, 0x0, 0x0, 0x8, 0x3);

    ISAFORMAT isadeocde;
    isadeocde = decode_ISA(instruction_s_type);
    dump_isa_info(isadeocde);

    instruction_s_type = s_type_inst_mix(0b0100011, 0x0, 0x1, 0x1, 0x8, 0x3);

    isadeocde = decode_ISA(instruction_s_type);
    dump_isa_info(isadeocde);

    instruction_s_type = s_type_inst_mix(0b0100011, 0x0, 0x02, 0x2, 0x8, 0x3);

    isadeocde = decode_ISA(instruction_s_type);
    dump_isa_info(isadeocde);

}

inline void tp06_01_b_type_test()
{
    uint32_t instruction_b_type = b_type_inst_mix(0b1100011, 0x0, 0x0, 0x0, 0x8, 0x0, 0x3);

    ISAFORMAT isadeocde;
    isadeocde = decode_ISA(instruction_b_type);
    dump_isa_info(isadeocde);

    instruction_b_type = b_type_inst_mix(0b1100011, 0x0, 0x1, 0x1, 0x8,0x0, 0x3);

    isadeocde = decode_ISA(instruction_b_type);
    dump_isa_info(isadeocde);

    instruction_b_type = b_type_inst_mix(0b1100011, 0x0, 0x04, 0x4, 0x8, 0x0, 0x3);

    isadeocde = decode_ISA(instruction_b_type);
    dump_isa_info(isadeocde);

    instruction_b_type = b_type_inst_mix(0b1100011, 0x0, 0x5, 0x5, 0x8, 0x0, 0x3);

    isadeocde = decode_ISA(instruction_b_type);
    dump_isa_info(isadeocde);

    instruction_b_type = b_type_inst_mix(0b1100011, 0x0, 0x06, 0x6, 0x8, 0x0, 0x3);

    isadeocde = decode_ISA(instruction_b_type);
    dump_isa_info(isadeocde);
    instruction_b_type = b_type_inst_mix(0b1100011, 0x0, 0x07, 0x7, 0x8, 0x0, 0x3);

    isadeocde = decode_ISA(instruction_b_type);
    dump_isa_info(isadeocde);


}


inline void tp06_01_u_type_test()
{
    uint32_t instruction_u_type = u_type_inst_mix(0b0110111, 0x0, 0x0);

    ISAFORMAT isadeocde;
    isadeocde = decode_ISA(instruction_u_type);
    dump_isa_info(isadeocde);

    instruction_u_type = u_type_inst_mix(0b0010111, 0x0, 0x1);

    isadeocde = decode_ISA(instruction_u_type);
    dump_isa_info(isadeocde);


}