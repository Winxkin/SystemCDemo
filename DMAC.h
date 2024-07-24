/*
 *	Author: Huan Nguyen-Duy
 *  Date: 24/07/2024
 */

#ifndef _DMAC_H
#define _DMAC_H
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/multi_passthrough_initiator_socket.h>
#include <tlm_utils/multi_passthrough_target_socket.h>
#include <map>
#include <vector>
#include <iostream>
#include <cstdint>
#include "Registerif.h"

// Notice: DMA Controller supports maximum 256 channels
#define DMA_MAX_CH 256

/*Define register address*/
/* i from 0 to 255 */
#define DMADESADDR(i)	 (0x00 + 0x04*(i))			// 0x000	->	0x400
#define DMASRCADDR(i)	 (0x400 + 0x04*(i))			// 0x400	->	0x800
#define DMADATALENGTH(i) (0x800 + 0x04*(i))			// 0x800	->	0xC00

/* i from 0 to 7 */
#define DMAREQ(i)		 (0xC00 + 0x04*(i))			// 0xC00	->	0xC20
#define DMAACK(i)		 (0x1000 + 0x04*(i))		// 0xC20	->	0xC40
#define DMAINT(i)		 (0x1400 + 0x04*(i))		// 0xC40	->	0xC60
#define DMACHEN(i)		 (0xC00 + 0x04*(i))			// 0xC60	->	0xC80


template<unsigned int BUSWIDTH = 32>
class DMAC : public sc_core::sc_module {

private:
	std::string m_name;
	RegisterInterface regs;
	unsigned int current_ch;
	unsigned int current_reg_req_ch;
	std::string current_reg_req_name;
	bool is_running;
	bool is_testmode;
	sc_core::sc_event e_DMA_request;
	sc_core::sc_event e_DMA_run;
	sc_core::sc_event e_DMA_run_done;
private:
	/*
	 * nb_transport_fw
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
	tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_core::sc_time& delay) {
		switch (phase)
		{
		case tlm::BEGIN_REQ:
		{
			// Handle BEGIN_REQ phase
			switch (trans.get_command()) {
			case tlm::TLM_WRITE_COMMAND:
			{
				unsigned int wdata = 0;
				std::memcpy(&wdata, trans.get_data_ptr(), sizeof(wdata));
				regs.update_register(trans.get_address(), wdata);
				// std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << ": Received transaction with address 0x" << std::hex << trans.get_address() << " data: 0x" << std::hex << wdata << std::dec << std::endl;
				trans.set_response_status(tlm::TLM_OK_RESPONSE);
				break;
			}
			case tlm::TLM_READ_COMMAND:
			{
				trans.set_response_status(tlm::TLM_OK_RESPONSE);
				break;
			}
			default:
				break;
			}

			/*
				Return data and END_REQ to bus
			*/
			tlm::tlm_phase next_phase = tlm::END_REQ;
			target_socket->nb_transport_bw(trans, next_phase, delay);
			break;
		}
		case tlm::END_REQ:
		{
			// Handle END_REQ phase (shouldn't happen here)
			break;
		}
		default:
			break;
		}
		return tlm::TLM_ACCEPTED;
	}

	/*
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
			break;
		}
		case tlm::END_REQ:
		{
			// Handle END_REQ phase
			break;
		}
		default:
			break;
		};
		return tlm::TLM_ACCEPTED;
	}

	void init_registers()
	{
		for (unsigned int i = 0; i < DMA_MAX_CH; i++)
		{
			regs.add_register("DMADESADDR" + std::string(i), DMADESADDR(i), 0x00, 0xFFFF, i);
			regs.add_register("DMASRCADDR" + std::string(i), DMASRCADDR(i), 0x00, 0xFFFF, i);
			regs.add_register("DMADATALENGTH" + std::string(i), DMADATALENGTH(i), 0x00, 0xFFFF, i);
		}
		for (unsigned int i = 0; i < (DMA_MAX_CH/32); i++)
		{
			regs.add_register("DMAREQ" + std::string(i), DMAREQ(i), 0x00, 0xFFFF, i);
			regs.add_register("DMAACK" + std::string(i), DMAACK(i), 0x00, 0xFFFF, i);
			regs.add_register("DMAINT" + std::string(i), DMAINT(i), 0x00, 0xFFFF, i);
			regs.add_register("DMACHEN" + std::string(i), DMACHEN(i), 0x00, 0xFFFF, i);
			
			// Registration call back function
			regs.set_register_callback("DMAREQ" + std::string(i), std::bind(&DMAC::cb_DMAREQ, this,
				std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5));
			regs.set_register_callback("DMAACK" + std::string(i), std::bind(&DMAC::cb_DMAACK, this,
				std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5));
			regs.set_register_callback("DMAINT" + std::string(i), std::bind(&DMAC::cb_DMAINT, this,
				std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5));
		}

	}

	/* Call back register */
	void cb_DMAREQ(const std::string& name, uint32_t value, uint32_t old_value, uint32_t mask, uint32_t ch)
	{
		// Request DMA operation in here
		if (!is_running)
		{
			is_testmode = true;
			current_reg_req_ch = ch;
			current_reg_req_name = name;
			e_DMA_request.notify();
		}
	}

	void cb_DMAACK(const std::string& name, uint32_t value, uint32_t old_value, uint32_t mask, uint32_t ch)
	{
		// Set DMA ACK signals in here
	}

	void cb_DMAINT(const std::string& name, uint32_t value, uint32_t old_value, uint32_t mask, uint32_t ch)
	{
		// Set Interrupt signals in here
	}

	/* SC_METHOD and SC_THREAD are defined in here */
	void mth_request_signals()
	{
		// SC_THREAD is triggered only one at the same simulation time
		if (!is_running)
		{
			e_DMA_request.notify();
		}
	}

	void thr_priority_process()
	{
		while (true)
		{
			// waiting to next cycle
			wait(e_DMA_request);
			wait(clk.posedge_event());
			is_running = true;
			if (!is_testmode)
			{
				for (unsigned int i = 0; i < DMA_MAX_CH; i++) {
					if (DMA_req[i].read()) {
						std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << "	DMA_req[" << i << "] has changed to HIGH" << std::endl;
						current_ch = i;
						e_DMA_run.notify();	// Bus delay simulation time
						wait(e_DMA_run_done);
						// continuting with next channels
						i = 0;
					}
				}
			}
			else
			{
				// using register
				uint32_t value = regs[current_reg_req_name].get_value();
				for (unsigned int i = 0; i < 32; i++)
				{
					if ((value >> i) & 0x01)
					{
						e_DMA_run.notify();	// Bus delay simulation time
						wait(e_DMA_run_done);
						// continuting with next channels
						regs[DMAACK(current_reg_req_ch)] = (0x01 << i) | regs[DMAACK(current_reg_req_ch)].get_value();
					}
				}
			}
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << "	DMA process is done" << std::endl;
			is_running = false;
			is_testmode = false;
			
		}
	}

	void thr_DMA_run_process()
	{
		while (true)
		{
			wait(e_DMA_run);

			e_DMA_run_done.notify();
		}
	}

public:
	tlm_utils::simple_target_socket<DMAC, BUSWIDTH> target_socket;
	tlm_utils::simple_initiator_socket<DMAC, BUSWIDTH> initiator_socket;
	sc_core::sc_out<bool> DMA_ack[DMA_MAX_CH];
	sc_core::sc_in<bool> DMA_req[DMA_MAX_CH];
	sc_core::sc_out<bool> DMA_int[DMA_MAX_CH];
	sc_core::sc_in<bool> clk;
	sc_core::sc_in<bool> rst;


	DMAC(sc_core::sc_module_name name) :
		sc_core::sc_module(name)
		, m_name(name)
		, target_socket("target_socket")
		, initiator_socket("initiator_socket")
		, current_ch(0)
		, is_running(false)
		, is_testmode(false)
	{
		target_socket.register_nb_transport_fw(this, &DMAC::nb_transport_fw);
		initiator_socket.register_nb_transport_bw(this, &DMAC::nb_transport_bw);
		// initialization registers
		init_registers();
		// Registration Method and thread
		SC_METHOD(mth_request_signals)
		for (int i = 0; i < DMA_MAX_CH; ++i) 
		{
			sensitive << DMA_req[i]; // Register each input port with the SC_METHOD
		}

		SC_THREAD(thr_priority_process);
		sensitive << e_DMA_request;

		SC_THREAD(thr_DMA_run_process);
		sensitive << e_DMA_run;
	};

	~DMAC() {};

};


#endif