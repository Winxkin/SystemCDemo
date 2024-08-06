/*
 *	Author: Huan Nguyen-Duy
 *  Date: 2/08/2024
 */


#ifndef _COMMONDEF_H
#define _COMMONDEF_H
#include <iostream>
#include <map>
#include <string>
#include <tuple>

namespace riscv32
{

    // Define the RISC-V instruction format lengths
#define OPCODE_LENGTH   7
#define RD_LENGTH       5
#define FUNCT3_LENGTH   3
#define RS1_LENGTH      5
#define RS2_LENGTH      5
#define FUNCT7_LENGTH   7
#define IMM_I_LENGTH    12
#define IMM_S_LENGTH    12
#define IMM_B_LENGTH    13
#define IMM_U_LENGTH    20
#define IMM_J_LENGTH    21

// Define the opcode type
#define R_TYPE	    0b0110011
#define I_O_TYPE	0b0010011
#define I_L_TYPE	0b0000011
#define I_E_TYPE	0b1110011
#define I_J_TYPE	0b1100111
#define S_TYPE		0b0100011
#define B_TYPE		0b1100011
#define J_TYPE		0b1101111
#define U_L_TYPE	0b0110111
#define U_A_TYPE	0b0010111

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

    typedef struct ISAFORMAT
    {
        ISAOPCODETYPE opcodetype;
        std::uint32_t opcode;
        std::uint32_t rd;
        std::uint32_t funct3;
        std::uint32_t funct7;
        std::uint32_t rs1;
        std::uint32_t rs2;
        std::uint32_t imm;
        std::string inst;
        ISAFORMAT()
        {
            opcodetype = ISA_UNKNOWN;
            opcode = 0x0;
            rd = 0x0;
            funct3 = 0x0;
            funct7 = 0x0;
            rs1 = 0x0; 
            rs2 = 0x0; 
            imm = 0x0;
            inst = "";
        };
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
        case R_TYPE:
        {
            _isa.opcodetype = ISA_R_TYPE;

            // To Indentify the instructions
            if (_isa.funct3 == 0x00 && _isa.funct7 == 0x00)
            {
                _isa.inst = "add";
            }
            else if (_isa.funct3 == 0x00 && _isa.funct7 == 0x20)
            {
                _isa.inst = "sub";
            }
            else if (_isa.funct3 == 0x04 && _isa.funct7 == 0x00)
            {
                _isa.inst = "xor";
            }
            else if (_isa.funct3 == 0x06 && _isa.funct7 == 0x00)
            {
                _isa.inst = "or";
            }
            else if (_isa.funct3 == 0x07 && _isa.funct7 == 0x00)
            {
                _isa.inst = "and";
            }
            else if (_isa.funct3 == 0x01 && _isa.funct7 == 0x00)
            {
                _isa.inst = "sll";
            }
            else if (_isa.funct3 == 0x05 && _isa.funct7 == 0x00)
            {
                _isa.inst = "srl";
            }
            else if (_isa.funct3 == 0x05 && _isa.funct7 == 0x20)
            {
                _isa.inst = "sra";
            }
            else if (_isa.funct3 == 0x02 && _isa.funct7 == 0x00)
            {
                _isa.inst = "slt";
            }
            else if (_isa.funct3 == 0x03 && _isa.funct7 == 0x00)
            {
                _isa.inst = "sltu";
            }
            break;
        }
        case I_O_TYPE:
        {
            _isa.opcodetype = ISA_I_TYPE;
            _isa.imm = decode_imm_i(instruction);

            // To Indentify the instructions
            if (_isa.funct3 == 0x00)
            {
                _isa.inst = "addi";
            }
            else if (_isa.funct3 == 0x04)
            {
                _isa.inst = "xori";
            }
            else if (_isa.funct3 == 0x06)
            {
                _isa.inst = "ori";
            }
            else if (_isa.funct3 == 0x07)
            {
                _isa.inst = "andi";
            }
            else if (_isa.funct3 == 0x01 && ((_isa.imm >> 5) & 0x7f) == 0x00)
            {
                _isa.inst = "slli";
            }
            else if (_isa.funct3 == 0x05 && ((_isa.imm >> 5) & 0x7f) == 0x00)
            {
                _isa.inst = "srli";
            }
            else if (_isa.funct3 == 0x05 && ((_isa.imm >> 5) & 0x7f) == 0x20)
            {
                _isa.inst = "srai";
            }
            else if (_isa.funct3 == 0x02)
            {
                _isa.inst = "slti";
            }
            else if (_isa.funct3 == 0x03)
            {
                _isa.inst = "sltiu";
            }
            break;
        }
        case I_L_TYPE:
        {
            _isa.opcodetype = ISA_I_TYPE;
            _isa.imm = decode_imm_i(instruction);

            // To Indentify the instructions
            if (_isa.funct3 == 0x00)
            {
                _isa.inst = "lb";
            }
            else if (_isa.funct3 == 0x01)
            {
                _isa.inst = "lh";
            }
            else if (_isa.funct3 == 0x02)
            {
                _isa.inst = "lw";
            }
            else if (_isa.funct3 == 0x04)
            {
                _isa.inst = "lbu";
            }
            else if (_isa.funct3 == 0x05)
            {
                _isa.inst = "lhu";
            }
            break;
        }
        case I_E_TYPE:
        {
            _isa.opcodetype = ISA_I_TYPE;
            _isa.imm = decode_imm_i(instruction);
            // To Indentify the instructions
            if (_isa.funct3 == 0x00 && _isa.imm == 0x00)
            {
                _isa.inst = "ecall";
            }
            else if (_isa.funct3 == 0x00 && _isa.imm == 0x01)
            {
                _isa.inst = "ebreak";
            }
            break;
        }
        case I_J_TYPE:
        {
            _isa.opcodetype = ISA_I_TYPE;
            _isa.imm = decode_imm_i(instruction);

            // To Indentify the instructions
            if (_isa.funct3 == 0x00)
            {
                _isa.inst = "jalr";
            }
            break;
        }
        case S_TYPE:
        {
            _isa.opcodetype = ISA_S_TYPE;
            _isa.imm = decode_imm_s(instruction);

            // To Indentify the instructions
            if (_isa.funct3 == 0x00)
            {
                _isa.inst = "sb";
            }
            else if (_isa.funct3 == 0x01)
            {
                _isa.inst = "sh";
            }
            else if (_isa.funct3 == 0x02)
            {
                _isa.inst = "sw";
            }
            break;
        }
        case B_TYPE:
        {
            _isa.opcodetype = ISA_B_TYPE;
            _isa.imm = decode_imm_b(instruction);

            // To Indentify the instructions
            if (_isa.funct3 == 0x00)
            {
                _isa.inst = "beq";
            }
            else if (_isa.funct3 == 0x01)
            {
                _isa.inst = "bne";
            }
            else if (_isa.funct3 == 0x04)
            {
                _isa.inst = "blt";
            }
            else if (_isa.funct3 == 0x05)
            {
                _isa.inst = "bge";
            }
            else if (_isa.funct3 == 0x06)
            {
                _isa.inst = "bltu";
            }
            else if (_isa.funct3 == 0x07)
            {
                _isa.inst = "bgeu";
            }
            break;
        }
        case J_TYPE:
        {
            _isa.opcodetype = ISA_J_TYPE;
            _isa.imm = decode_imm_j(instruction);

            // To Indentify the instructions
            _isa.inst = "jal";

            break;
        }
        case U_L_TYPE:
        {
            _isa.opcodetype = ISA_U_TYPE;
            _isa.imm = decode_imm_u(instruction);
            // To Indentify the instructions
            _isa.inst = "lui";
            break;
        }
        case U_A_TYPE:
        {
            _isa.opcodetype = ISA_U_TYPE;
            _isa.imm = decode_imm_u(instruction);
            // To Indentify the instructions
            _isa.inst = "auipc";
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
        case ISA_R_TYPE: opcodeTypeName = "R-TYPE"; break;
        case ISA_I_TYPE: opcodeTypeName = "I-TYPE"; break;
        case ISA_S_TYPE: opcodeTypeName = "S-TYPE"; break;
        case ISA_B_TYPE: opcodeTypeName = "B-TYPE"; break;
        case ISA_U_TYPE: opcodeTypeName = "U-TYPE"; break;
        case ISA_J_TYPE: opcodeTypeName = "J-TYPE"; break;
        default: opcodeTypeName = "UNKNOWN"; break;
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
        std::cout << "Instruction: "   << isa.inst << std::endl;
    }


}
#endif