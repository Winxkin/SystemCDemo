/*
 *	Author: Huan Nguyen-Duy
 *  Date: 2/08/2024
 */

#ifndef _RISCV32_H
#define _RISCV32_H

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
#include "decode_inst.h"
#include "instruction.h"
#include "registerbank.h"


class riscv32 : public sc_core::sc_module
{
private:


	std::string m_name;
	isa32::ISAFORMAT m_isa; 
	std::uint32_t PC;

	// The current instruction that is loaded from instruction memory
	std::uint32_t cur_inst;

	bool is_branch;
	bool is_first_time;
	// Define event for pipeline process
	sc_core::sc_event e_inst_fetch;
	sc_core::sc_event e_inst_decode;
	sc_core::sc_event e_execute;
	sc_core::sc_event e_data_memory_access;
	sc_core::sc_event e_write_back;

	sc_core::sc_event e_irq_handler;

	sc_core::sc_event e_pipeline_process_done;
private:
	// Internal modules
	Instruction m_Instruction;
	RegisterBank m_RegisterBank;
	Memory m_DataMem;
	Memory m_InstMem;
private:

	
	void inst_fetch_process()
	{
		while (true)
		{
			wait(e_inst_fetch);
			// synchronization with 1 clock cycle
			wait(clk.posedge_event());
			// Inst_fetch process operation
			// Waiting 5 times sc_zero_time to control the priority of this process
			wait(sc_core::SC_ZERO_TIME);
			wait(sc_core::SC_ZERO_TIME);
			wait(sc_core::SC_ZERO_TIME);
			wait(sc_core::SC_ZERO_TIME);
			wait(sc_core::SC_ZERO_TIME);


			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << " PC: " << PC << "	inst_fetch_process\n";
			
			
			// Check condition in here
			// Go to next stage
			is_first_time = false;
			e_inst_decode.notify();
		}
	}

	void inst_decode_process()
	{
		while (true)
		{
			wait(e_inst_decode);
			// synchronization with 1 clock cycle
			e_inst_fetch.notify();
			wait(clk.posedge_event());
			// Waiting 4 times sc_zero_time to control the priority of this process
			wait(sc_core::SC_ZERO_TIME); 
			wait(sc_core::SC_ZERO_TIME);
			wait(sc_core::SC_ZERO_TIME);
			wait(sc_core::SC_ZERO_TIME);

			// Check if the previous instruction is branch -> flushing the current instruction.
			if (is_branch)
			{
				is_branch = false;
				continue;
			}

			// decode new _isa
			m_isa = isa32::decode_ISA(cur_inst);
			
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	inst_decode_process\n";

			// Increasing PC = PC + 4 byte
			Inc_PC_default();
			// Go to next stage
			e_execute.notify();
			
		}
	}

	void execute_process()
	{
		while (true)
		{
			wait(e_execute);
			// synchronization with 1 clock cycle
			wait(clk.posedge_event());
			// Waiting 3 times sc_zero_time to control the priority of this process
			wait(sc_core::SC_ZERO_TIME);
			wait(sc_core::SC_ZERO_TIME);
			wait(sc_core::SC_ZERO_TIME);

			// ALU operation
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	execute_process\n";
			

			// update PC if the instruction is jumping
			// check if the instruction is branch
			if ((m_isa.inst == "beq") || (m_isa.inst == "bne") || (m_isa.inst == "blt") || (m_isa.inst == "bge") ||
				(m_isa.inst == "bltu") || (m_isa.inst == "bgeu") || (m_isa.inst == "jal") || (m_isa.inst == "jalr"))
			{
				if (m_Instruction[m_isa.inst](m_RegisterBank, m_DataMem, PC, m_isa.rd, m_isa.rs1, m_isa.rs2, m_isa.imm))
				{
					is_branch = true;
				}
			}
			else
			{
				// handling instruction, in case of branch the PC is increase in execution process
				m_Instruction[m_isa.inst](m_RegisterBank, m_DataMem, PC, m_isa.rd, m_isa.rs1, m_isa.rs2, m_isa.imm);
			}
			

			// Go to next stage
			e_data_memory_access.notify();

		}
	}

	void data_memory_access_process()
	{
		while (true)
		{
			wait(e_data_memory_access);
			// synchronization with 1 clock cycle
			wait(clk.posedge_event());
			// Waiting 2 time sc_zero_time to control the priority of this process
			wait(sc_core::SC_ZERO_TIME);
			wait(sc_core::SC_ZERO_TIME);

			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	data_memory_access_process\n";

			// Go to next stage
			e_write_back.notify();
		}
	}

	void write_back_process()
	{
		while (true)
		{
			wait(e_write_back);
			// synchronization with 1 clock cycle
			wait(clk.posedge_event());
			wait(sc_core::SC_ZERO_TIME);

			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	write_back_process\n";

			// Complete pipline process
			e_pipeline_process_done.notify();
		}
	}





	void monitor_clk()
	{
		std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]	Clock posedge is triggered.\n";
	}

	void irq_handler_process()
	{
		while (true)
		{
			wait(e_irq_handler);

		}
	}

	void irq_triggered()
	{
		e_irq_handler.notify();
	}

	void Inc_PC_default()
	{
		PC = PC + 4; // default 4 byte = 32bit
	}

public:
	sc_core::sc_in<bool> clk;
	sc_core::sc_in<bool> rst;

	riscv32(sc_core::sc_module_name name) :
		m_name(name)
		, PC(0x00)
		, m_Instruction("Instruction")
		, m_RegisterBank("RegisterBank")
		, m_DataMem("DataMem", 64000) // 64KB
		, m_InstMem("InstMem", 64000) // 64KB
		, is_first_time(true)
		, is_branch(false)
	{


		SC_THREAD(inst_fetch_process);
		sensitive << e_inst_fetch << e_pipeline_process_done;

		SC_THREAD(inst_decode_process);
		sensitive << e_inst_decode;

		SC_THREAD(execute_process);
		sensitive << e_execute;

		SC_THREAD(data_memory_access_process);
		sensitive << e_data_memory_access;

		SC_THREAD(write_back_process);
		sensitive << e_write_back;

		SC_THREAD(irq_handler_process);
		sensitive << e_irq_handler;


		SC_METHOD(monitor_clk);
		sensitive << clk.pos();
		dont_initialize();

		SC_METHOD(irq_triggered);
		dont_initialize();

		
	}

	void start_cpu()
	{
		is_first_time = true;
		e_inst_fetch.notify();
	}

};


#endif