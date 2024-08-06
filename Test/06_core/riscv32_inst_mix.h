/*
 *	Author: Huan Nguyen-Duy
 *  Date: 06/08/2024
 */

#ifndef _RISCV32_INST_MIX_H
#define _RISCV32_INST_MIX_H

#include <iostream>
#include <map>
#include <string>
#include <bitset>
#include <tuple>

 // Function to convert a number to a binary string
inline std::string toBinaryString(std::uint32_t num) {
	return std::bitset<32>(num).to_string();
}

inline uint32_t r_type_inst_mix(uint32_t opcode, uint32_t rd, uint32_t funct3, uint32_t funct7, uint32_t rs1, uint32_t rs2)
{

	uint32_t instruction_set = ((funct7 & 0x7F) << 25) | ((rs2 & 0x1F) << 20) | ((rs1 & 0x1F) << 15) | ((funct3 & 0x7) << 12)
		| ((rd & 0x1F) << 7) | (opcode & 0x7F);
	std::cout << "instruction R-Type: " << toBinaryString(instruction_set) << std::endl;
	return instruction_set;
}

inline uint32_t i_type_inst_mix(uint32_t opcode, uint32_t rd, uint32_t funct3, uint32_t imm_0_11, uint32_t rs1)
{
	
	uint32_t instruction_set = ((imm_0_11 & 0xFFF) << 20) | ((rs1 & 0x1F) << 15) | ((funct3 & 0x7) << 12)
		| ((rd & 0x1F) << 7) | (opcode & 0x7F);
	std::cout << "instruction I-Type: " << toBinaryString(instruction_set) << std::endl;
	return instruction_set;
}

inline uint32_t s_type_inst_mix(uint32_t opcode, uint32_t im_0_4, uint32_t funct3, uint32_t imm_5_11, uint32_t rs1, uint32_t rs2)
{

	uint32_t instruction_set = ((imm_5_11 & 0x7F) << 25) | ((rs2 & 0x1F) << 20) | ((rs1 & 0x1F) << 15) | ((funct3 & 0x7) << 12)
		| ((im_0_4 & 0x1F) << 7) | (opcode & 0x7F);
	std::cout << "instruction S-Type: " << toBinaryString(instruction_set) << std::endl;
	return instruction_set;
}

inline uint32_t b_type_inst_mix(uint32_t opcode, uint32_t im_0_4, uint32_t funct3, uint32_t imm_5_10, uint32_t imm_12, uint32_t rs1, uint32_t rs2)
{

	uint32_t instruction_set = ((imm_12 & 0x1) << 30) | ((imm_5_10 & 0x3F) << 25) | ((rs2 & 0x1F) << 20) | ((rs1 & 0x1F) << 15) | ((funct3 & 0x7) << 12)
		| ((im_0_4 & 0x1F) << 7) | (opcode & 0x7F);
	std::cout << "instruction B-Type: " << toBinaryString(instruction_set) << std::endl;
	return instruction_set;
}


inline uint32_t u_type_inst_mix(uint32_t opcode, uint32_t rd, uint32_t imm_12_31)
{

	uint32_t instruction_set = ((imm_12_31 & 0xfffff) << 12) | ((rd & 0x1F) << 7) | (opcode & 0x7F);
	std::cout << "instruction U-Type: " << toBinaryString(instruction_set) << std::endl;
	return instruction_set;
}





#endif
