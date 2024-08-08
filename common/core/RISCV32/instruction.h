/*
 *	Author: Huan Nguyen-Duy
 *  Date: 2/08/2024
 */

#ifndef _INSTRUCTION_H
#define _INSTRUCTION_H

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/multi_passthrough_initiator_socket.h>
#include <tlm_utils/multi_passthrough_target_socket.h>
#include <map>
#include <vector>
#include <iostream>
#include <list>
#include <cstdint>
#include "commondef.h"

// This class is defined to handle for each instruction
class Instruction : public sc_core::sc_module
{
private:
	std::string m_name;
	std::map<std::string, std::function<std::uint32_t(isa32::ISAFORMAT, std::uint32_t)>> instructionmap;

private:
	// Define the instruction execution in here
	std::uint32_t execute_add(isa32::ISAFORMAT _isa, std::uint32_t _PC)
	{
		return _PC;
	}

	std::uint32_t execute_sub(isa32::ISAFORMAT _isa, std::uint32_t _PC)
	{
		return _PC;
	}

	std::uint32_t execute_xor(isa32::ISAFORMAT _isa, std::uint32_t _PC)
	{
		return _PC;
	}

	std::uint32_t execute_or(isa32::ISAFORMAT _isa, std::uint32_t _PC)
	{
		return _PC;
	}

	std::uint32_t execute_and(isa32::ISAFORMAT _isa, std::uint32_t _PC)
	{
		return _PC;
	}

	std::uint32_t execute_sll(isa32::ISAFORMAT _isa, std::uint32_t _PC)
	{
		return _PC;
	}

	std::uint32_t execute_srl(isa32::ISAFORMAT _isa, std::uint32_t _PC)
	{
		return _PC;
	}

	std::uint32_t execute_sra(isa32::ISAFORMAT _isa, std::uint32_t _PC)
	{
		return _PC;
	}

	std::uint32_t execute_slt(isa32::ISAFORMAT _isa, std::uint32_t _PC)
	{
		return _PC;
	}

	std::uint32_t execute_sltu(isa32::ISAFORMAT _isa, std::uint32_t _PC)
	{
		return _PC;
	}

	std::uint32_t execute_addi(isa32::ISAFORMAT _isa, std::uint32_t _PC)
	{
		return _PC;
	}

	std::uint32_t execute_xori(isa32::ISAFORMAT _isa, std::uint32_t _PC)
	{
		return _PC;
	}

	std::uint32_t execute_ori(isa32::ISAFORMAT _isa, std::uint32_t _PC)
	{
		return _PC;
	}

	std::uint32_t execute_andi(isa32::ISAFORMAT _isa, std::uint32_t _PC)
	{
		return _PC;
	}

	std::uint32_t execute_slli(isa32::ISAFORMAT _isa, std::uint32_t _PC)
	{
		return _PC;
	}

	std::uint32_t execute_srli(isa32::ISAFORMAT _isa, std::uint32_t _PC)
	{
		return _PC;
	}

	std::uint32_t execute_srai(isa32::ISAFORMAT _isa, std::uint32_t _PC)
	{
		return _PC;
	}

	std::uint32_t execute_slti(isa32::ISAFORMAT _isa, std::uint32_t _PC)
	{
		return _PC;
	}
	
	std::uint32_t execute_sltiu(isa32::ISAFORMAT _isa, std::uint32_t _PC)
	{
		return _PC;
	}

	std::uint32_t execute_lb(isa32::ISAFORMAT _isa, std::uint32_t _PC)
	{
		return _PC;
	}

	std::uint32_t execute_lh(isa32::ISAFORMAT _isa, std::uint32_t _PC)
	{
		return _PC;
	}

	std::uint32_t execute_lw(isa32::ISAFORMAT _isa, std::uint32_t _PC)
	{
		return _PC;
	}

	std::uint32_t execute_lbu(isa32::ISAFORMAT _isa, std::uint32_t _PC)
	{
		return _PC;
	}

	std::uint32_t execute_lhu(isa32::ISAFORMAT _isa, std::uint32_t _PC)
	{
		return _PC;
	}

	std::uint32_t execute_sb(isa32::ISAFORMAT _isa, std::uint32_t _PC)
	{
		return _PC;
	}

	std::uint32_t execute_sh(isa32::ISAFORMAT _isa, std::uint32_t _PC)
	{
		return _PC;
	}

	std::uint32_t execute_sw(isa32::ISAFORMAT _isa, std::uint32_t _PC)
	{
		return _PC;
	}

	std::uint32_t execute_beq(isa32::ISAFORMAT _isa, std::uint32_t _PC)
	{
		return _PC;
	}

	std::uint32_t execute_bne(isa32::ISAFORMAT _isa, std::uint32_t _PC)
	{
		return _PC;
	}

	std::uint32_t execute_blt(isa32::ISAFORMAT _isa, std::uint32_t _PC)
	{
		return _PC;
	}

	std::uint32_t execute_bge(isa32::ISAFORMAT _isa, std::uint32_t _PC)
	{
		return _PC;
	}

	std::uint32_t execute_bltu(isa32::ISAFORMAT _isa, std::uint32_t _PC)
	{
		return _PC;
	}

	std::uint32_t execute_bgeu(isa32::ISAFORMAT _isa, std::uint32_t _PC)
	{
		return _PC;
	}

	std::uint32_t execute_jal(isa32::ISAFORMAT _isa, std::uint32_t _PC)
	{
		return _PC;
	}

	std::uint32_t execute_jalr(isa32::ISAFORMAT _isa, std::uint32_t _PC)
	{
		return _PC;
	}

	std::uint32_t execute_lui(isa32::ISAFORMAT _isa, std::uint32_t _PC)
	{
		return _PC;
	}

	std::uint32_t execute_auipc(isa32::ISAFORMAT _isa, std::uint32_t _PC)
	{
		return _PC;
	}

	std::uint32_t execute_eccall(isa32::ISAFORMAT _isa, std::uint32_t _PC)
	{
		return _PC;
	}

	std::uint32_t execute_ebreak(isa32::ISAFORMAT _isa, std::uint32_t _PC)
	{
		return _PC;
	}

private:

	void mapping_instruction()
	{
		instructionmap["add"]		=	[this](isa32::ISAFORMAT _isa, std::uint32_t _PC) { return this->execute_add(_isa, _PC); };
		instructionmap["sub"]		=	[this](isa32::ISAFORMAT _isa, std::uint32_t _PC) { return this->execute_sub(_isa, _PC); };
		instructionmap["xor"]		=	[this](isa32::ISAFORMAT _isa, std::uint32_t _PC) { return this->execute_xor(_isa, _PC); };
		instructionmap["or"]		=	[this](isa32::ISAFORMAT _isa, std::uint32_t _PC) { return this->execute_or(_isa, _PC); };
		instructionmap["and"]		=	[this](isa32::ISAFORMAT _isa, std::uint32_t _PC) { return this->execute_and(_isa, _PC); };
		instructionmap["sll"]		=	[this](isa32::ISAFORMAT _isa, std::uint32_t _PC) { return this->execute_sll(_isa, _PC); };
		instructionmap["srl"]		=	[this](isa32::ISAFORMAT _isa, std::uint32_t _PC) { return this->execute_srl(_isa, _PC); };
		instructionmap["sra"]		=	[this](isa32::ISAFORMAT _isa, std::uint32_t _PC) { return this->execute_sra(_isa, _PC); };
		instructionmap["slt"]		=	[this](isa32::ISAFORMAT _isa, std::uint32_t _PC) { return this->execute_slt(_isa, _PC); };
		instructionmap["sltu"]		=	[this](isa32::ISAFORMAT _isa, std::uint32_t _PC) { return this->execute_sltu(_isa, _PC); };
		instructionmap["addi"]		=	[this](isa32::ISAFORMAT _isa, std::uint32_t _PC) { return this->execute_addi(_isa, _PC); };
		instructionmap["xori"]		=	[this](isa32::ISAFORMAT _isa, std::uint32_t _PC) { return this->execute_xori(_isa, _PC); };
		instructionmap["ori"]		=	[this](isa32::ISAFORMAT _isa, std::uint32_t _PC) { return this->execute_ori(_isa, _PC); };
		instructionmap["andi"]		=	[this](isa32::ISAFORMAT _isa, std::uint32_t _PC) { return this->execute_andi(_isa, _PC); };
		instructionmap["slli"]		=	[this](isa32::ISAFORMAT _isa, std::uint32_t _PC) { return this->execute_slli(_isa, _PC); };
		instructionmap["srli"]		=	[this](isa32::ISAFORMAT _isa, std::uint32_t _PC) { return this->execute_srli(_isa, _PC); };
		instructionmap["srai"]		=	[this](isa32::ISAFORMAT _isa, std::uint32_t _PC) { return this->execute_srai(_isa, _PC); };
		instructionmap["slti"]		=	[this](isa32::ISAFORMAT _isa, std::uint32_t _PC) { return this->execute_slti(_isa, _PC); };
		instructionmap["sltiu"]		=	[this](isa32::ISAFORMAT _isa, std::uint32_t _PC) { return this->execute_sltiu(_isa, _PC); };
		instructionmap["lb"]		=	[this](isa32::ISAFORMAT _isa, std::uint32_t _PC) { return this->execute_lb(_isa, _PC); };
		instructionmap["lh"]		=	[this](isa32::ISAFORMAT _isa, std::uint32_t _PC) { return this->execute_lh(_isa, _PC); };
		instructionmap["lw"]		=	[this](isa32::ISAFORMAT _isa, std::uint32_t _PC) { return this->execute_lw(_isa, _PC); };
		instructionmap["lbu"]		=	[this](isa32::ISAFORMAT _isa, std::uint32_t _PC) { return this->execute_lbu(_isa, _PC); };
		instructionmap["lhu"]		=	[this](isa32::ISAFORMAT _isa, std::uint32_t _PC) { return this->execute_lhu(_isa, _PC); };
		instructionmap["sb"]		=	[this](isa32::ISAFORMAT _isa, std::uint32_t _PC) { return this->execute_sb(_isa, _PC); };
		instructionmap["sh"]		=	[this](isa32::ISAFORMAT _isa, std::uint32_t _PC) { return this->execute_sh(_isa, _PC); };
		instructionmap["sw"]		=	[this](isa32::ISAFORMAT _isa, std::uint32_t _PC) { return this->execute_sw(_isa, _PC); };
		instructionmap["beq"]		=	[this](isa32::ISAFORMAT _isa, std::uint32_t _PC) { return this->execute_beq(_isa, _PC); };
		instructionmap["bne"]		=	[this](isa32::ISAFORMAT _isa, std::uint32_t _PC) { return this->execute_bne(_isa, _PC); };
		instructionmap["blt"]		=	[this](isa32::ISAFORMAT _isa, std::uint32_t _PC) { return this->execute_blt(_isa, _PC); };
		instructionmap["bge"]		=	[this](isa32::ISAFORMAT _isa, std::uint32_t _PC) { return this->execute_bge(_isa, _PC); };
		instructionmap["bltu"]		=	[this](isa32::ISAFORMAT _isa, std::uint32_t _PC) { return this->execute_bltu(_isa, _PC); };
		instructionmap["bgeu"]		=	[this](isa32::ISAFORMAT _isa, std::uint32_t _PC) { return this->execute_bgeu(_isa, _PC); };
		instructionmap["jal"]		=	[this](isa32::ISAFORMAT _isa, std::uint32_t _PC) { return this->execute_jal(_isa, _PC); };
		instructionmap["jalr"]		=	[this](isa32::ISAFORMAT _isa, std::uint32_t _PC) { return this->execute_jalr(_isa, _PC); };
		instructionmap["lui"]		=	[this](isa32::ISAFORMAT _isa, std::uint32_t _PC) { return this->execute_lui(_isa, _PC); };
		instructionmap["auipc"]		=	[this](isa32::ISAFORMAT _isa, std::uint32_t _PC) { return this->execute_auipc(_isa, _PC); };
		instructionmap["eccall"]	=	[this](isa32::ISAFORMAT _isa, std::uint32_t _PC) { return this->execute_eccall(_isa, _PC); };
		instructionmap["ebreak"]	=	[this](isa32::ISAFORMAT _isa, std::uint32_t _PC) { return this->execute_ebreak(_isa, _PC); };
	}

public:
	Instruction(sc_core::sc_module_name name) : m_name(name)
	{
		mapping_instruction();
	}
	
	~Instruction() {};

};

#endif
