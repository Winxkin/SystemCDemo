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
#include "commondef.h"
#include "instruction.h"


class riscv32 : public sc_core::sc_module
{
private:

	typedef enum
	{
		NONE = 0,
		FORWARDING,
		STALL
	} HAZARDMODE;

	riscv32::HAZARDMODE m_hazard;

	std::string m_name;
	isa32::ISAFORMAT m_isa; // ??
	std::uint32_t PC;
	std::uint32_t cur_inst;
	
	std::uint32_t temp_rd;
	std::uint32_t temp_rs1;
	std::uint32_t temp_rs2;

	bool is_first_time;
	// Define event for pipeline process
	sc_core::sc_event e_inst_fetch;
	sc_core::sc_event e_inst_decode;
	sc_core::sc_event e_execute;
	sc_core::sc_event e_data_memory_access;
	sc_core::sc_event e_write_back;

	sc_core::sc_event e_pipeline_process_done;
private:
	// Internal modules
	Instruction m_Instruction;

private:

	
	void inst_fetch_process()
	{
		while (true)
		{
			wait(e_inst_fetch);
			// synchronization with 1 clock cycle
			wait(clk.posedge_event());
			// Inst_fetch process operation
			if (m_hazard == STALL && is_first_time == false)
			{
				wait(clk.posedge_event());
			}
			// Waiting 4 times sc_zero_time to control the priority of this process
			wait(sc_core::SC_ZERO_TIME);
			wait(sc_core::SC_ZERO_TIME);
			wait(sc_core::SC_ZERO_TIME);
			wait(sc_core::SC_ZERO_TIME);
			wait(sc_core::SC_ZERO_TIME);


			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ] inst_fetch_process\n";
			
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
			// Waiting 3 times sc_zero_time to control the priority of this process
			wait(sc_core::SC_ZERO_TIME); 
			wait(sc_core::SC_ZERO_TIME);
			wait(sc_core::SC_ZERO_TIME);
			wait(sc_core::SC_ZERO_TIME);

			// copy old _isa to temporal structe

			// decode new _isa
			
			// Check data hazard of current instruction
			check_for_hazard();

			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ] inst_decode_process\n";

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
			// Waiting 2 times sc_zero_time to control the priority of this process
			wait(sc_core::SC_ZERO_TIME);
			wait(sc_core::SC_ZERO_TIME);
			wait(sc_core::SC_ZERO_TIME);

			// ALU operation
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ] execute_process\n";

			if (m_hazard == FORWARDING)
			{
				// Using temporal data to calculate
			}
			else
			{
				// Using data that read from reg to calculate
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
			// Waiting 1 time sc_zero_time to control the priority of this process
			wait(sc_core::SC_ZERO_TIME);
			wait(sc_core::SC_ZERO_TIME);

			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ] data_memory_access_process\n";


			// If forwarding mode, copy data output to temporal variables

			// Copy data output to temporal variable
		
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

			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ] write_back_process\n";

			// Release hazard flag.
			m_Instruction.set_hazard(false);
			// Complete pipline process
			e_pipeline_process_done.notify();
		}
	}




	riscv32::HAZARDMODE check_for_hazard()
	{
		// if data hazard occurs, copy rd, rs1, rs2 to temporal variables

		// In the case Stall operation occur
		if (true)	// inst = load, 
		{
			m_hazard = STALL;
			return STALL;
		}
		return FORWARDING;
	}


	void execute(isa32::ISAFORMAT _isa, uint32_t& PC)
	{
		// To call the corresponding instruction
		m_Instruction[_isa.inst](_isa, PC);
	}

	void monitor_clk()
	{
		std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ] Clock posedge is triggered.\n";
	}


public:
	sc_core::sc_in<bool> clk;
	sc_core::sc_in<bool> rst;

	riscv32(sc_core::sc_module_name name) :
		m_name(name)
		, PC(0x00)
		, m_Instruction("Instruction")
		, is_first_time(true)
		, m_hazard(NONE)
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

		SC_METHOD(monitor_clk);
		sensitive << clk.pos();

		
	}

	void start_cpu()
	{
		is_first_time = true;
		e_inst_fetch.notify();
	}

};


#endif