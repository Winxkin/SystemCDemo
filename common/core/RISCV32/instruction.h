/*
 *	Author: Huan Nguyen-Duy
 *  Date: 2/08/2024
 */

#ifndef _INSTRUCTION_H
#define _INSTRUCTION_H

#include <systemc>
#include <tlm>
#include <map>
#include <vector>
#include <iostream>
#include <list>
#include <cstdint>
#include "decode_inst.h"
#include "registerbank.h"
#include "memory.h"

// This class is defined to handle for each instruction
class Instruction : public sc_core::sc_module
{
private:
	std::string m_name;
	bool m_message;
	std::map<std::string, std::function<bool(RegisterBank&, Memory&, std::uint32_t&, std::uint32_t&, std::uint32_t&, std::uint32_t&, std::uint32_t&)>> instructionmap;

private:


	// Define the instruction execution in here
	bool execute_add(RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm)
	{
		std::uint32_t x_rd = _reg.get_x_reg(_rd);
		std::uint32_t x_rs1 = _reg.get_x_reg(_rs1);
		std::uint32_t x_rs2 = _reg.get_x_reg(_rs2);

		if (m_message)
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	execute_add\n";
		}

		x_rd = x_rs1 + x_rs2;

		_reg.set_x_reg(_rd, x_rd);

		return true;
	}

	bool execute_sub(RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm)
	{

		std::uint32_t x_rd = _reg.get_x_reg(_rd);
		std::uint32_t x_rs1 = _reg.get_x_reg(_rs1);
		std::uint32_t x_rs2 = _reg.get_x_reg(_rs2);

		if (m_message)
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	execute_sub\n";
		}

		x_rd = x_rs1 - x_rs2;

		_reg.set_x_reg(_rd, x_rd);

		return true;
	}

	bool execute_xor(RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm)
	{
		std::uint32_t x_rd = _reg.get_x_reg(_rd);
		std::uint32_t x_rs1 = _reg.get_x_reg(_rs1);
		std::uint32_t x_rs2 = _reg.get_x_reg(_rs2);

		if (m_message)
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	execute_xor\n";
		}

		x_rd = x_rs1 ^ x_rs2;

		_reg.set_x_reg(_rd, x_rd);

		return true;
	}

	bool execute_or(RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm)
	{
		std::uint32_t x_rd = _reg.get_x_reg(_rd);
		std::uint32_t x_rs1 = _reg.get_x_reg(_rs1);
		std::uint32_t x_rs2 = _reg.get_x_reg(_rs2);

		if (m_message)
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	execute_or\n";
		}

		x_rd = x_rs1 | x_rs2;

		_reg.set_x_reg(_rd, x_rd);
		return true;
	}

	bool execute_and(RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm)
	{
		std::uint32_t x_rd = _reg.get_x_reg(_rd);
		std::uint32_t x_rs1 = _reg.get_x_reg(_rs1);
		std::uint32_t x_rs2 = _reg.get_x_reg(_rs2);

		if (m_message)
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	execute_and\n";
		}

		x_rd = x_rs1 & x_rs2;

		_reg.set_x_reg(_rd, x_rd);
		return true;
	}

	bool execute_sll(RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm)
	{
		std::uint32_t x_rd = _reg.get_x_reg(_rd);
		std::uint32_t x_rs1 = _reg.get_x_reg(_rs1);
		std::uint32_t x_rs2 = _reg.get_x_reg(_rs2);

		if (m_message)
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	execute_sll\n";
		}

		x_rd = x_rs1 << x_rs2;

		_reg.set_x_reg(_rd, x_rd);
		return true;
	}

	bool execute_srl(RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm)
	{
		std::uint32_t x_rd = _reg.get_x_reg(_rd);
		std::uint32_t x_rs1 = _reg.get_x_reg(_rs1);
		std::uint32_t x_rs2 = _reg.get_x_reg(_rs2);

		if (m_message)
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	execute_srl\n";
		}

		x_rd = x_rs1 >> x_rs2;

		_reg.set_x_reg(_rd, x_rd);
		return true;
	}

	bool execute_sra(RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm)
	{
		std::uint32_t x_rd = _reg.get_x_reg(_rd);
		std::uint32_t x_rs1 = _reg.get_x_reg(_rs1);
		std::uint32_t x_rs2 = _reg.get_x_reg(_rs2);

		if (m_message)
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	execute_sra\n";
		}

		x_rd = x_rs1 >> x_rs2;
		x_rd = x_rd | ((x_rs1 >> 31 | 0x01) << 31);

		_reg.set_x_reg(_rd, x_rd);
		return true;
	}

	bool execute_slt(RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm)
	{
		std::uint32_t x_rd = _reg.get_x_reg(_rd);
		std::uint32_t x_rs1 = _reg.get_x_reg(_rs1);
		std::uint32_t x_rs2 = _reg.get_x_reg(_rs2);

		if (m_message)
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	execute_slt\n";
		}

		x_rd = (x_rs1 < x_rs2) ? 1 : 0;

		_reg.set_x_reg(_rd, x_rd);
		return true;
	}

	bool execute_sltu(RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm)
	{
		std::uint32_t x_rd = _reg.get_x_reg(_rd);
		std::uint32_t x_rs1 = _reg.get_x_reg(_rs1);
		std::uint32_t x_rs2 = _reg.get_x_reg(_rs2);

		if (m_message)
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	execute_sltu\n";
		}

		x_rd = (x_rs1 < x_rs2) ? 1 : 0;

		_reg.set_x_reg(_rd, x_rd);
		return true;
	}

	bool execute_addi(RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm)
	{
		std::uint32_t x_rd = _reg.get_x_reg(_rd);
		std::uint32_t x_rs1 = _reg.get_x_reg(_rs1);
		std::uint32_t x_rs2 = _reg.get_x_reg(_rs2);

		if (m_message)
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	execute_addi\n";
		}

		x_rd = x_rs1 + _imm;

		_reg.set_x_reg(_rd, x_rd);
		return true;
	}

	bool execute_xori(RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm)
	{
		std::uint32_t x_rd = _reg.get_x_reg(_rd);
		std::uint32_t x_rs1 = _reg.get_x_reg(_rs1);
		std::uint32_t x_rs2 = _reg.get_x_reg(_rs2);

		if (m_message)
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	execute_xori\n";
		}

		x_rd = x_rs1 ^ _imm;

		_reg.set_x_reg(_rd, x_rd);
		return true;
	}

	bool execute_ori(RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm)
	{
		std::uint32_t x_rd = _reg.get_x_reg(_rd);
		std::uint32_t x_rs1 = _reg.get_x_reg(_rs1);
		std::uint32_t x_rs2 = _reg.get_x_reg(_rs2);

		if (m_message)
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	execute_ori\n";
		}

		x_rd = x_rs1 | _imm;

		_reg.set_x_reg(_rd, x_rd);
		return true;
	}

	bool execute_andi(RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm)
	{
		std::uint32_t x_rd = _reg.get_x_reg(_rd);
		std::uint32_t x_rs1 = _reg.get_x_reg(_rs1);
		std::uint32_t x_rs2 = _reg.get_x_reg(_rs2);

		if (m_message)
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	execute_andi\n";
		}

		x_rd = x_rs1 & _imm;

		_reg.set_x_reg(_rd, x_rd);
		return true;
	}

	bool execute_slli(RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm)
	{
		std::uint32_t x_rd = _reg.get_x_reg(_rd);
		std::uint32_t x_rs1 = _reg.get_x_reg(_rs1);
		std::uint32_t x_rs2 = _reg.get_x_reg(_rs2);

		if (m_message)
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	execute_slli\n";
		}

		x_rd = x_rs1 << (_imm & 0x1f );

		_reg.set_x_reg(_rd, x_rd);
		return true;
	}

	bool execute_srli(RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm)
	{
		std::uint32_t x_rd = _reg.get_x_reg(_rd);
		std::uint32_t x_rs1 = _reg.get_x_reg(_rs1);
		std::uint32_t x_rs2 = _reg.get_x_reg(_rs2);

		if (m_message)
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	execute_srli\n";
		}

		x_rd = x_rs1 >> (_imm & 0x1f);

		_reg.set_x_reg(_rd, x_rd);
		return true;
	}

	bool execute_srai(RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm)
	{

		std::uint32_t x_rd = _reg.get_x_reg(_rd);
		std::uint32_t x_rs1 = _reg.get_x_reg(_rs1);
		std::uint32_t x_rs2 = _reg.get_x_reg(_rs2);

		if (m_message)
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	execute_srai\n";
		}

		x_rd = x_rs1 >> (_imm & 0x1f);
		x_rd = x_rd | ((x_rs1 >> 31 | 0x01) << 31);
		_reg.set_x_reg(_rd, x_rd);
		return true;
	}

	bool execute_slti(RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm)
	{
		std::uint32_t x_rd = _reg.get_x_reg(_rd);
		std::uint32_t x_rs1 = _reg.get_x_reg(_rs1);
		std::uint32_t x_rs2 = _reg.get_x_reg(_rs2);

		if (m_message)
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	execute_slti\n";
		}

		x_rd = (x_rs1 < _imm) ? 1 : 0;
		_reg.set_x_reg(_rd, x_rd);
		return true;
	}
	
	bool execute_sltiu(RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm)
	{
		std::uint32_t x_rd = _reg.get_x_reg(_rd);
		std::uint32_t x_rs1 = _reg.get_x_reg(_rs1);
		std::uint32_t x_rs2 = _reg.get_x_reg(_rs2);

		if (m_message)
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	execute_sltiu\n";
		}

		x_rd = (x_rs1 < _imm) ? 1 : 0;
		_reg.set_x_reg(_rd, x_rd);
		return true;
	}

	bool execute_lb(RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm)
	{
		std::uint32_t x_rd = _reg.get_x_reg(_rd);
		std::uint32_t x_rs1 = _reg.get_x_reg(_rs1);
		std::uint32_t x_rs2 = _reg.get_x_reg(_rs2);

		if (m_message)
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	execute_lb\n";
		}

		x_rd = (std::uint32_t)_mem.get_byte(x_rs1 + _imm);
		_reg.set_x_reg(_rd, x_rd);
		return true;
	}

	bool execute_lh(RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm)
	{
		std::uint32_t x_rd = _reg.get_x_reg(_rd);
		std::uint32_t x_rs1 = _reg.get_x_reg(_rs1);
		std::uint32_t x_rs2 = _reg.get_x_reg(_rs2);

		if (m_message)
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	execute_lh\n";
		}

		x_rd = (std::uint32_t)_mem.get_half(x_rs1 + _imm);
		_reg.set_x_reg(_rd, x_rd);
		return true;
	}

	bool execute_lw(RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm)
	{
		std::uint32_t x_rd = _reg.get_x_reg(_rd);
		std::uint32_t x_rs1 = _reg.get_x_reg(_rs1);
		std::uint32_t x_rs2 = _reg.get_x_reg(_rs2);

		if (m_message)
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	execute_lw\n";
		}

		x_rd = (std::uint32_t)_mem.get_word(x_rs1 + _imm);
		_reg.set_x_reg(_rd, x_rd);
		return true;
	}

	bool execute_lbu(RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm)
	{
		std::uint32_t x_rd = _reg.get_x_reg(_rd);
		std::uint32_t x_rs1 = _reg.get_x_reg(_rs1);
		std::uint32_t x_rs2 = _reg.get_x_reg(_rs2);

		if (m_message)
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	execute_lbu\n";
		}

		x_rd = (std::uint32_t)_mem.get_byte(x_rs1 + _imm);
		_reg.set_x_reg(_rd, x_rd);
		return true;
	}

	bool execute_lhu(RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm)
	{
		std::uint32_t x_rd = _reg.get_x_reg(_rd);
		std::uint32_t x_rs1 = _reg.get_x_reg(_rs1);
		std::uint32_t x_rs2 = _reg.get_x_reg(_rs2);

		if (m_message)
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	execute_lhu\n";
		}

		x_rd = (std::uint32_t)_mem.get_half(x_rs1 + _imm);
		_reg.set_x_reg(_rd, x_rd);
		return true;
	}

	bool execute_sb(RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm)
	{
		std::uint32_t x_rd = _reg.get_x_reg(_rd);
		std::uint32_t x_rs1 = _reg.get_x_reg(_rs1);
		std::uint32_t x_rs2 = _reg.get_x_reg(_rs2);

		if (m_message)
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	execute_sb\n";
		}

		x_rs2 = x_rs2 & 0xff;
		_mem.set_byte(x_rs1 + _imm, (std::uint8_t)x_rs2);

		return true;
	}

	bool execute_sh(RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm)
	{
		std::uint32_t x_rd = _reg.get_x_reg(_rd);
		std::uint32_t x_rs1 = _reg.get_x_reg(_rs1);
		std::uint32_t x_rs2 = _reg.get_x_reg(_rs2);

		if (m_message)
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	execute_sh\n";
		}

		x_rs2 = x_rs2 & 0xffff;
		_mem.set_half(x_rs1 + _imm, (std::uint16_t)x_rs2);
		return true;
	}

	bool execute_sw(RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm)
	{
		std::uint32_t x_rd = _reg.get_x_reg(_rd);
		std::uint32_t x_rs1 = _reg.get_x_reg(_rs1);
		std::uint32_t x_rs2 = _reg.get_x_reg(_rs2);

		if (m_message)
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	execute_sw\n";
		}

		_mem.set_word(x_rs1 + _imm, x_rs2);

		return true;
	}

	bool execute_beq(RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm)
	{
		std::uint32_t x_rs1 = _reg.get_x_reg(_rs1);
		std::uint32_t x_rs2 = _reg.get_x_reg(_rs2);

		if (m_message)
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	execute_beq\n";
		}

		if (x_rs1 == x_rs2)
		{
			_PC = _PC + _imm;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool execute_bne(RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm)
	{
		std::uint32_t x_rs1 = _reg.get_x_reg(_rs1);
		std::uint32_t x_rs2 = _reg.get_x_reg(_rs2);

		if (m_message)
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	execute_bne\n";
		}

		if (x_rs1 != x_rs2)
		{
			_PC = _PC + _imm;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool execute_blt(RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm)
	{
		std::uint32_t x_rs1 = _reg.get_x_reg(_rs1);
		std::uint32_t x_rs2 = _reg.get_x_reg(_rs2);

		if (m_message)
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	execute_blt\n";
		}

		if (x_rs1 < x_rs2)
		{
			_PC = _PC + _imm;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool execute_bge(RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm)
	{
		std::uint32_t x_rs1 = _reg.get_x_reg(_rs1);
		std::uint32_t x_rs2 = _reg.get_x_reg(_rs2);

		if (m_message)
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	execute_bge\n";
		}

		if (x_rs1 >= x_rs2)
		{
			_PC = _PC + _imm;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool execute_bltu(RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm)
	{
		std::uint32_t x_rs1 = _reg.get_x_reg(_rs1);
		std::uint32_t x_rs2 = _reg.get_x_reg(_rs2);

		if (m_message)
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	execute_bltu\n";
		}

		if (x_rs1 < x_rs2)
		{
			_PC = _PC + _imm;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool execute_bgeu(RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm)
	{
		std::uint32_t x_rs1 = _reg.get_x_reg(_rs1);
		std::uint32_t x_rs2 = _reg.get_x_reg(_rs2);

		if (m_message)
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	execute_bgeu\n";
		}

		if (x_rs1 >= x_rs2)
		{
			_PC = _PC + _imm;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool execute_jal(RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm)
	{
		std::uint32_t x_rd = _reg.get_x_reg(_rd);

		if (m_message)
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	execute_jal\n";
		}

		x_rd = _PC + 4;
		
		_reg.set_x_reg(_rd, x_rd);

		// branch
		_PC = _PC + _imm;
		return true;
	}

	bool execute_jalr(RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm)
	{
		std::uint32_t x_rd = _reg.get_x_reg(_rd);
		std::uint32_t x_rs1 = _reg.get_x_reg(_rs1);

		if (m_message)
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	execute_jalr\n";
		}

		x_rd = _PC + 4;
		
		_reg.set_x_reg(_rd, x_rd);

		_PC = x_rs1 + _imm;
		return true;
	}

	bool execute_lui(RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm)
	{
		std::uint32_t x_rd = _reg.get_x_reg(_rd);

		if (m_message)
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	execute_lui\n";
		}

		x_rd = _imm << 12;

		_reg.set_x_reg(_rd, x_rd);
		return true;
	}

	bool execute_auipc(RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm)
	{
		std::uint32_t x_rd = _reg.get_x_reg(_rd);

		if (m_message)
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	execute_auipc\n";
		}

		x_rd = _PC + (_imm << 12);

		_reg.set_x_reg(_rd, x_rd);
		return true;
	}

	bool execute_eccall(RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm)
	{
		if (m_message)
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	execute_eccall\n";
		}
		return true;
	}

	bool execute_ebreak(RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm)
	{
		if (m_message)
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	execute_ebreak\n";
		}
		return true;
	}

private:

	void mapping_instruction()
	{
		instructionmap["add"]		=	[this](RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm) { return this->execute_add(_reg, _mem, _PC, _rd, _rs1, _rs2, _imm); };
		instructionmap["sub"]		=	[this](RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm) { return this->execute_sub(_reg, _mem, _PC, _rd, _rs1, _rs2, _imm); };
		instructionmap["xor"]		=	[this](RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm) { return this->execute_xor(_reg, _mem, _PC, _rd, _rs1, _rs2, _imm); };
		instructionmap["or"]		=	[this](RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm) { return this->execute_or(_reg, _mem, _PC, _rd, _rs1, _rs2, _imm); };
		instructionmap["and"]		=	[this](RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm) { return this->execute_and(_reg, _mem, _PC, _rd, _rs1, _rs2, _imm); };
		instructionmap["sll"]		=	[this](RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm) { return this->execute_sll(_reg, _mem, _PC, _rd, _rs1, _rs2, _imm); };
		instructionmap["srl"]		=	[this](RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm) { return this->execute_srl(_reg, _mem, _PC, _rd, _rs1, _rs2, _imm); };
		instructionmap["sra"]		=	[this](RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm) { return this->execute_sra(_reg, _mem, _PC, _rd, _rs1, _rs2, _imm); };
		instructionmap["slt"]		=	[this](RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm) { return this->execute_slt(_reg, _mem, _PC, _rd, _rs1, _rs2, _imm); };
		instructionmap["sltu"]		=	[this](RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm) { return this->execute_sltu(_reg, _mem, _PC, _rd, _rs1, _rs2, _imm); };
		instructionmap["addi"]		=	[this](RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm) { return this->execute_addi(_reg, _mem, _PC, _rd, _rs1, _rs2, _imm); };
		instructionmap["xori"]		=	[this](RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm) { return this->execute_xori(_reg, _mem, _PC, _rd, _rs1, _rs2, _imm); };
		instructionmap["ori"]		=	[this](RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm) { return this->execute_ori(_reg, _mem, _PC, _rd, _rs1, _rs2, _imm); };
		instructionmap["andi"]		=	[this](RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm) { return this->execute_andi(_reg, _mem, _PC, _rd, _rs1, _rs2, _imm); };
		instructionmap["slli"]		=	[this](RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm) { return this->execute_slli(_reg, _mem, _PC, _rd, _rs1, _rs2, _imm); };
		instructionmap["srli"]		=	[this](RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm) { return this->execute_srli(_reg, _mem, _PC, _rd, _rs1, _rs2, _imm); };
		instructionmap["srai"]		=	[this](RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm) { return this->execute_srai(_reg, _mem, _PC, _rd, _rs1, _rs2, _imm); };
		instructionmap["slti"]		=	[this](RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm) { return this->execute_slti(_reg, _mem, _PC, _rd, _rs1, _rs2, _imm); };
		instructionmap["sltiu"]		=	[this](RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm) { return this->execute_sltiu(_reg, _mem, _PC, _rd, _rs1, _rs2, _imm); };
		instructionmap["lb"]		=	[this](RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm) { return this->execute_lb(_reg, _mem, _PC, _rd, _rs1, _rs2, _imm); };
		instructionmap["lh"]		=	[this](RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm) { return this->execute_lh(_reg, _mem, _PC, _rd, _rs1, _rs2, _imm); };
		instructionmap["lw"]		=	[this](RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm) { return this->execute_lw(_reg, _mem, _PC, _rd, _rs1, _rs2, _imm); };
		instructionmap["lbu"]		=	[this](RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm) { return this->execute_lbu(_reg, _mem, _PC, _rd, _rs1, _rs2, _imm); };
		instructionmap["lhu"]		=	[this](RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm) { return this->execute_lhu(_reg, _mem, _PC, _rd, _rs1, _rs2, _imm); };
		instructionmap["sb"]		=	[this](RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm) { return this->execute_sb(_reg, _mem, _PC, _rd, _rs1, _rs2, _imm); };
		instructionmap["sh"]		=	[this](RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm) { return this->execute_sh(_reg, _mem, _PC, _rd, _rs1, _rs2, _imm); };
		instructionmap["sw"]		=	[this](RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm) { return this->execute_sw(_reg, _mem, _PC, _rd, _rs1, _rs2, _imm); };
		instructionmap["beq"]		=	[this](RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm) { return this->execute_beq(_reg, _mem, _PC, _rd, _rs1, _rs2, _imm); };
		instructionmap["bne"]		=	[this](RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm) { return this->execute_bne(_reg, _mem, _PC, _rd, _rs1, _rs2, _imm); };
		instructionmap["blt"]		=	[this](RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm) { return this->execute_blt(_reg, _mem, _PC, _rd, _rs1, _rs2, _imm); };
		instructionmap["bge"]		=	[this](RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm) { return this->execute_bge(_reg, _mem, _PC, _rd, _rs1, _rs2, _imm); };
		instructionmap["bltu"]		=	[this](RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm) { return this->execute_bltu(_reg, _mem, _PC, _rd, _rs1, _rs2, _imm); };
		instructionmap["bgeu"]		=	[this](RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm) { return this->execute_bgeu(_reg, _mem, _PC, _rd, _rs1, _rs2, _imm); };
		instructionmap["jal"]		=	[this](RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm) { return this->execute_jal(_reg, _mem, _PC, _rd, _rs1, _rs2, _imm); };
		instructionmap["jalr"]		=	[this](RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm) { return this->execute_jalr(_reg, _mem, _PC, _rd, _rs1, _rs2, _imm); };
		instructionmap["lui"]		=	[this](RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm) { return this->execute_lui(_reg, _mem, _PC, _rd, _rs1, _rs2, _imm); };
		instructionmap["auipc"]		=	[this](RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm) { return this->execute_auipc(_reg, _mem, _PC, _rd, _rs1, _rs2, _imm); };
		instructionmap["eccall"]	=	[this](RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm) { return this->execute_eccall(_reg, _mem, _PC, _rd, _rs1, _rs2, _imm); };
		instructionmap["ebreak"]	=	[this](RegisterBank& _reg, Memory& _mem, std::uint32_t& _PC, std::uint32_t& _rd, std::uint32_t& _rs1, std::uint32_t& _rs2, std::uint32_t& _imm) { return this->execute_ebreak(_reg, _mem, _PC, _rd, _rs1, _rs2, _imm); };
	}

public:

	Instruction(sc_core::sc_module_name name, bool message = false) 
		: m_name(name), m_message(message)
	{
		mapping_instruction();
	}
	
	~Instruction() {};

	std::function<bool(RegisterBank&, Memory&, std::uint32_t&, std::uint32_t&, std::uint32_t&, std::uint32_t&, std::uint32_t&)>& operator[](std::string _inst)
	{
		if (instructionmap.find(_inst) == instructionmap.end()) {
			throw std::runtime_error("Instruction not found");
		}
		return instructionmap[_inst];
	}

};

#endif
