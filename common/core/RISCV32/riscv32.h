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

class riscv32 : public sc_core::sc_module
{
private:

	typedef enum
	{
		FORWARDING = 0,
		STALL
	} HAZARDMODE;

	std::string m_name;
	isa32::ISAFORMAT m_isa;
	std::uint32_t PC;
	std::uint32_t cur_inst;
	
	std::uint32_t temp_rd;
	std::uint32_t temp_rs1;
	std::uint32_t temp_rs2;
	// Define event for pipeline process
	sc_core::sc_event e_inst_fetch;
	sc_core::sc_event e_inst_decode;
	sc_core::sc_event e_execute;
	sc_core::sc_event e_data_memory_access;
	sc_core::sc_event e_write_back;

	sc_core::sc_event e_pipeline_process_done;
private:



	void inst_fetch_process()
	{
		while (true)
		{
			wait(e_inst_fetch);
			// synchronization with 1 clock cycle
			wait(clk.posedge_event());

			// Inst_fetch process operation
			if (check_for_hazard() == STALL)
			{
				wait(e_pipeline_process_done);
			}
			

			// Go to next stage
			e_inst_decode.notify();
			// re-triggering process and waiting to the next cycle
			e_inst_fetch.notify();

		}
	}

	void inst_decode_process()
	{
		while (true)
		{
			wait(e_inst_decode);
			// synchronization with 1 clock cycle
			wait(clk.posedge_event());

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

			// ALU operation
			

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


			// Complete pipline process
			e_pipeline_process_done.notify();
		}
	}

	void start_cpu()
	{
		e_inst_fetch.notify();
	}

	riscv32::HAZARDMODE check_for_hazard()
	{
		// if data hazard occurs, copy rd, rs1, rs2 to temporal variables

		// In the case Stall operation occur
		if (true)	// inst = load, 
		{
			return STALL;
		}
		return FORWARDING;
	}


	void execute(isa32::ISAFORMAT _isa, uint32_t PC)
	{

	}



public:
	sc_core::sc_in<bool> clk;
	sc_core::sc_in<bool> rst;

	riscv32(sc_core::sc_module_name name) : m_name(name), PC(0x00)
	{

		SC_THREAD(inst_fetch_process);
		sensitive << e_inst_fetch << e_pipeline_process_done;

		SC_THREAD(inst_decode_process);
		sensitive << e_inst_decode;

		SC_THREAD(data_memory_access_process);
		sensitive << e_data_memory_access;

		SC_THREAD(write_back_process);
		sensitive << e_write_back;

		
	}

};


#endif