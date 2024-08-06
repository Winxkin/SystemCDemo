// All systemc modules should include systemc.h header file
/*
#include "TestList.h"

// sc_main in top level function like in C++ main


int sc_main(int argc, char* argv[]) {

    // Running test case here
    tp_05_01();
    return 0;
}
*/

#include <systemc>
#include <iostream>
#include "RISCV32\commondef.h"
using namespace riscv32;

int sc_main(int argc, char* argv[]) {
    // Example instructions (in binary format)
    uint32_t instruction_r_type = 0b00000000101000001000000110110011; // add x3, x1, x2
    uint32_t instruction_i_type = 0b00000000000100001000001010010011; // addi x5, x1, 1
    uint32_t instruction_s_type = 0b00000000000100001000001010100011; // sw x1, 1(x2)
    uint32_t instruction_b_type = 0b00000000000100001000001011100011; // beq x1, x2, 2
    uint32_t instruction_u_type = 0b00000000000000001000000010110111; // lui x1, 2
    uint32_t instruction_j_type = 0b00000000000000000000000011101111; // jal x1, 0


    ISAFORMAT isadeocde;
    isadeocde = decode_ISA(instruction_r_type);
    dump_isa_info(isadeocde);

    isadeocde = decode_ISA(instruction_i_type);
    dump_isa_info(isadeocde);

    isadeocde = decode_ISA(instruction_s_type);
    dump_isa_info(isadeocde);

    isadeocde = decode_ISA(instruction_b_type);
    dump_isa_info(isadeocde);

    isadeocde = decode_ISA(instruction_u_type);
    dump_isa_info(isadeocde);

    return 0;
}



