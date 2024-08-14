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

template<unsigned int BUSWIDTH = 32>
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

	sc_core::sc_event e_test;

	// Define process handle to control SC_THEARD
	sc_core::sc_process_handle t_inst_fetch;
	sc_core::sc_process_handle t_inst_decode;
	sc_core::sc_process_handle t_execute;
	sc_core::sc_process_handle t_data_memory_access;
	sc_core::sc_process_handle t_write_back;

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
			

			execute_instruction();
			

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
			// Transfering data through BUS MMIO
			suspend_pipeline();
			
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
			e_test.notify();
			// Complete pipline process
			e_pipeline_process_done.notify();
		}
	}

	void test()
	{
		while (true)
		{
			wait(e_test);
			wait(clk.posedge_event());
			wait(clk.posedge_event());
			wait(clk.posedge_event());
			wait(clk.posedge_event());
			wait(clk.posedge_event());
			// simulating for delay on bus MMIO
			resume_pipeline();


		}
	}



	void resume_pipeline()
	{
		t_inst_fetch.resume();
		t_inst_decode.resume();
		t_execute.resume();
	}

	void suspend_pipeline()
	{
		t_inst_fetch.suspend();
		t_inst_decode.suspend();
		t_execute.suspend();
	}


	void execute_instruction()
	{
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
		else if (m_isa.inst == "sw")
		{
			suspend_pipeline();
			// In this case the user want to setting register of peripheral through bus mmio
			m_Instruction[m_isa.inst](m_RegisterBank, m_DataMem, PC, m_isa.rd, m_isa.rs1, m_isa.rs2, m_isa.imm);

			std::uint32_t x_rs1 = m_RegisterBank.get_x_reg(m_isa.rs1);
			std::uint32_t x_rs2 = m_RegisterBank.get_x_reg(m_isa.rs2);
			std::uint32_t data = m_DataMem.get_word(x_rs1 + m_isa.imm);
			unsigned char* _data = reinterpret_cast<unsigned char*>(&data);

			tlm::tlm_generic_payload trans;
			sc_core::sc_time delay = sc_core::SC_ZERO_TIME;
			trans.set_command(tlm::TLM_WRITE_COMMAND);
			trans.set_address(x_rs1 + m_isa.imm);
			trans.set_data_length(0x04);
			trans.set_data_ptr(_data);
			trans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

			if (m_message)
			{
				std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << ": Sending Write transaction with address 0x" << std::hex << (x_rs1 + m_isa.imm) << std::endl;
			}
			tlm::tlm_phase phase = tlm::BEGIN_REQ;
			tlm::tlm_sync_enum status;
			status = initiator_socket->nb_transport_fw(trans, phase, delay);

		}
		else
		{
			// handling instruction, in case of branch the PC is increase in execution process
			m_Instruction[m_isa.inst](m_RegisterBank, m_DataMem, PC, m_isa.rd, m_isa.rs1, m_isa.rs2, m_isa.imm);
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

	/**@brief
	 * nb_transport_bw
	 *
	 * Implements the non-blocking backward transport interface for the initiator.
	 *
	 * @param trans Reference to the generic payload object containing the transaction details
	 *              such as command, address, and data.
	 *
	 * @param phase Reference to the transaction phase. The current phase of the transaction,
	 *              which may be updated by the function.
	 *
	 * @param delay Reference to the annotated delay. Specifies the timing delay for the transaction
	 *              and may be updated by the function.
	 *
	 * @return tlm::tlm_sync_enum Enumeration indicating the synchronization state of the transaction:
	 *         - TLM_ACCEPTED: Transaction is accepted, and no immediate further action is required.
	 *         - TLM_UPDATED: Transaction phase has been updated. The initiator should check the new phase.
	 *         - TLM_COMPLETED: Transaction is completed immediately, and no further phases will occur.
	 */
	tlm::tlm_sync_enum nb_transport_bw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_core::sc_time& delay) {
		switch (phase)
		{
		case tlm::BEGIN_REQ:
		{
			// Handle BEGIN_REQ phase
			if (m_message)
			{
				std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << "	BEGIN_REQ received" << std::endl;
			}
			break;
		}
		case tlm::END_REQ:
		{
			// Handle END_REQ phase (shouldn't happen here)
			if (m_message)
			{
				std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << "	END_REQ received" << std::endl;
			}
			
			// To resume pipeline process
			resume_pipeline();
			break;
		}
		default:
			break;
		};
		return tlm::TLM_ACCEPTED;
	}

public:
	sc_core::sc_in<bool> clk;
	sc_core::sc_in<bool> rst;

	tlm_utils::simple_initiator_socket<riscv32, BUSWIDTH> initiator_socket;

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
		sensitive << e_inst_fetch;
		t_inst_fetch = sc_get_current_process_handle();

		SC_THREAD(inst_decode_process);
		sensitive << e_inst_decode;
		t_inst_decode = sc_get_current_process_handle();

		SC_THREAD(execute_process);
		sensitive << e_execute;
		t_execute = sc_get_current_process_handle();

		SC_THREAD(data_memory_access_process);
		sensitive << e_data_memory_access;
		t_data_memory_access = sc_get_current_process_handle();

		SC_THREAD(write_back_process);
		sensitive << e_write_back;
		t_write_back = sc_get_current_process_handle();

		SC_THREAD(irq_handler_process);
		sensitive << e_irq_handler;


		SC_METHOD(monitor_clk);
		sensitive << clk.pos();
		dont_initialize();

		SC_METHOD(irq_triggered);
		dont_initialize();

		SC_THREAD(test);
		sensitive << e_test;


	}

	void load_instruction(std::uint32_t _inst)
	{
		m_InstMem.load_instruction(_inst);
	}

	void start_cpu()
	{
		is_first_time = true;
		e_inst_fetch.notify();
	}

};


#endif