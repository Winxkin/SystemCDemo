/*
 *	Author: Huan Nguyen-Duy
 *  Date: 2/08/2024
 */


#ifndef _COMMONDEF_H
#define _COMMONDEF_H
#include <iostream>

namespace riscv32
{

    // Define the RISC-V instruction format lengths
#define OPCODE_LENGTH 7
#define RD_LENGTH 5
#define FUNCT3_LENGTH 3
#define RS1_LENGTH 5
#define RS2_LENGTH 5
#define FUNCT7_LENGTH 7
#define IMM_I_LENGTH 12
#define IMM_S_LENGTH 12
#define IMM_B_LENGTH 13
#define IMM_U_LENGTH 20
#define IMM_J_LENGTH 21

// Define the opcode type
#define R_O_TYPE	0b0110011
#define R_A_TYPE	0b0101111
#define I_O_TYPE	0b0010011
#define I_L_TYPE	0b0000011
#define I_E_TYPE	0b1110011
#define I_J_TYPE	0b1100111
#define S_TYPE		0b0100011
#define B_TYPE		0b1100011
#define J_TYPE		0b1101111
#define U_TYPE		0b0110111

// define emum
    typedef enum
    {
        ISA_R_TYPE = 0,
        ISA_I_TYPE,
        ISA_S_TYPE,
        ISA_B_TYPE,
        ISA_U_TYPE,
        ISA_J_TYPE,
        ISA_UNKNOWN
    } ISAOPCODETYPE;

    typedef struct
    {
        ISAOPCODETYPE opcodetype;
        std::uint32_t opcode;
        std::uint32_t rd;
        std::uint32_t funct3;
        std::uint32_t funct7;
        std::uint32_t rs1;
        std::uint32_t rs2;
        std::uint32_t imm;
    } ISAFORMAT;


    // Function to extract a bitfield from a 32-bit instruction
    inline std::uint32_t get_bits(std::uint32_t instruction, int start, int length)
    {
        return (instruction >> start) & ((1 << length) - 1);
    }

    // Decoding function support for RISC-V 32bit
    inline std::uint32_t decode_opcode(std::uint32_t instruction)
    {
        return get_bits(instruction, 0, OPCODE_LENGTH);
    }

    inline std::uint32_t decode_funct3(std::uint32_t instruction)
    {
        return get_bits(instruction, 12, FUNCT3_LENGTH);
    }

    inline std::uint32_t decode_funct7(std::uint32_t instruction)
    {
        return get_bits(instruction, 25, FUNCT7_LENGTH);
    }

    inline std::uint32_t decode_rs1(std::uint32_t instruction)
    {
        return get_bits(instruction, 15, RS1_LENGTH);
    }

    inline std::uint32_t decode_rs2(std::uint32_t instruction)
    {
        return get_bits(instruction, 20, RS2_LENGTH);
    }

    inline std::uint32_t decode_rd(std::uint32_t instruction)
    {
        return get_bits(instruction, 7, RD_LENGTH);
    }

    inline std::uint32_t decode_imm_i(std::uint32_t instruction)
    {
        return get_bits(instruction, 20, IMM_I_LENGTH);
    }

    inline std::uint32_t decode_imm_s(std::uint32_t instruction)
    {
        return (get_bits(instruction, 25, 7) << 5) | get_bits(instruction, 7, 5);
    }

    inline std::uint32_t decode_imm_b(std::uint32_t instruction)
    {
        return  (get_bits(instruction, 31, 1) << 12) | (get_bits(instruction, 25, 6) << 5) | (get_bits(instruction, 8, 4) << 1) | (get_bits(instruction, 7, 1) << 11);
    }

    inline std::uint32_t decode_imm_u(std::uint32_t instruction)
    {
        return  get_bits(instruction, 12, IMM_U_LENGTH);
    }

    inline std::uint32_t decode_imm_j(std::uint32_t instruction)
    {
        return (get_bits(instruction, 31, 1) << 20) | (get_bits(instruction, 21, 10) << 1) | (get_bits(instruction, 20, 1) << 11) | (get_bits(instruction, 12, 8) << 12);
    }

    inline ISAFORMAT decode_ISA(std::uint32_t instruction)
    {
        ISAFORMAT _isa;
        _isa.opcode = decode_opcode(instruction);
        _isa.funct3 = decode_funct3(instruction);
        _isa.funct7 = decode_funct7(instruction);
        _isa.rd = decode_rd(instruction);
        _isa.rs1 = decode_rs1(instruction);
        _isa.rs2 = decode_rs2(instruction);

        switch (_isa.opcode)
        {
        case R_O_TYPE:
        {
            _isa.opcodetype = ISA_R_TYPE;
            break;
        }
        case R_A_TYPE:
        {
            _isa.opcodetype = ISA_R_TYPE;
            break;
        }
        case I_O_TYPE:
        {
            _isa.opcodetype = ISA_I_TYPE;
            break;
        }
        case I_L_TYPE:
        {
            _isa.opcodetype = ISA_I_TYPE;
            break;
        }
        case I_E_TYPE:
        {
            _isa.opcodetype = ISA_I_TYPE;
            break;
        }
        case I_J_TYPE:
        {
            _isa.opcodetype = ISA_I_TYPE;
            break;
        }
        case S_TYPE:
        {
            _isa.opcodetype = ISA_S_TYPE;
            break;
        }
        case B_TYPE:
        {
            _isa.opcodetype = ISA_B_TYPE;
            break;
        }
        case J_TYPE:
        {
            _isa.opcodetype = ISA_J_TYPE;
            break;
        }
        case U_TYPE:
        {
            _isa.opcodetype = ISA_U_TYPE;
            break;
        }
        default:
        {
            _isa.opcodetype = ISA_UNKNOWN;
            break;
        }
        }
        return _isa;
    }

    inline void dump_isa_info(ISAFORMAT isa)
    {
        std::string opcodeTypeName = "";

        switch (isa.opcodetype)
        {
        case ISA_R_TYPE: opcodeTypeName = "ISA_R_TYPE"; break;
        case ISA_I_TYPE: opcodeTypeName = "ISA_I_TYPE"; break;
        case ISA_S_TYPE: opcodeTypeName = "ISA_S_TYPE"; break;
        case ISA_B_TYPE: opcodeTypeName = "ISA_B_TYPE"; break;
        case ISA_U_TYPE: opcodeTypeName = "ISA_U_TYPE"; break;
        case ISA_J_TYPE: opcodeTypeName = "ISA_J_TYPE"; break;
        default:
            break;
        }

        std::cout << "RISCV-32 ISA Format:" << std::endl;
        std::cout << "Opcode Type: " << opcodeTypeName << std::endl;
        std::cout << "Opcode     : 0x" << std::hex << isa.opcode << std::endl;
        std::cout << "rd         : 0x" << std::hex << isa.rd << std::endl;
        std::cout << "funct3     : 0x" << std::hex << isa.funct3 << std::endl;
        std::cout << "funct7     : 0x" << std::hex << isa.funct7 << std::endl;
        std::cout << "rs1        : 0x" << std::hex << isa.rs1 << std::endl;
        std::cout << "rs2        : 0x" << std::hex << isa.rs2 << std::endl;
        std::cout << "imm        : 0x" << std::hex << isa.imm << std::endl;
    }


}
#endif