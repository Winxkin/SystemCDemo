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
#include <list>
#include <cstdint>
#include "Registerif.h"
#include "commondef.h"

// Notice: DMA Controller supports maximum 256 channels
#define DMA_MAX_CH 256

/*Define register address*/
/* i from 0 to 255 */
#define DMADESADDR(i)	 (0x00 + 0x04*(i))			// 0x000	->	0x400
#define DMASRCADDR(i)	 (0x400 + 0x04*(i))			// 0x400	->	0x800
#define DMADATALENGTH(i) (0x800 + 0x04*(i))			// 0x800	->	0xC00

/* i from 0 to 7 */
#define DMAREQ(i)		 (0xC00 + 0x04*(i))			// 0xC00	->	0xC20
#define DMAACK(i)		 (0xC20 + 0x04*(i))			// 0xC20	->	0xC40
#define DMAINT(i)		 (0xC40 + 0x04*(i))			// 0xC40	->	0xC60
#define DMACHEN(i)		 (0xC60 + 0x04*(i))			// 0xC60	->	0xC80 


template<unsigned int BUSWIDTH = 32>
class DMAC : public sc_core::sc_module {

private:
	std::string m_name;
	bool m_message;
	RegisterInterface regs;
	unsigned int m_current_ch;
	unsigned int m_cur_reg_ch;
	std::string m_cur_reg_name;
	bool m_running;
	bool m_testmode;
	sc_core::sc_event e_DMA_request;
	sc_core::sc_event e_DMA_run;
	sc_core::sc_event e_DMA_run_done;
	sc_core::sc_event e_DMA_forward;
	tlm::tlm_generic_payload current_trans;
	std::list<unsigned int> port_req_ids;
private:

	/**@brief
	* copy_tlm_generic_payload
	*
	* Impelmentation the copy operation from source TLM generic payload to destination TLM generic payload
	*
	* @param des Reference to destination TLM generic payload
	*
	* @param src Reference to source TLM generic payload
	*
	*/
	void copy_tlm_generic_payload(tlm::tlm_generic_payload& des, tlm::tlm_generic_payload& src)
	{
		des.set_command(src.get_command());
		des.set_address(src.get_address());
		des.set_data_length(src.get_data_length());

		unsigned char* data = new unsigned char[src.get_data_length()];
		std::memcpy(data, src.get_data_ptr(), src.get_data_length());
		des.set_data_ptr(data);

		des.set_response_status(src.get_response_status());
		des.set_streaming_width(src.get_streaming_width());
		des.set_byte_enable_ptr(src.get_byte_enable_ptr());
		des.set_dmi_allowed(src.is_dmi_allowed());
	}

	/**@brief
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
			if (m_message)
			{
				LOG("(%s) (Target socket) BEGIN_REQ received\n", m_name.c_str());
			}
			switch (trans.get_command()) {
			case tlm::TLM_WRITE_COMMAND:
			{
				unsigned int wdata = 0;
				std::memcpy(&wdata, trans.get_data_ptr(), sizeof(wdata));
				regs.update_register(trans.get_address(), wdata);
				if (m_message)
				{
					LOG("(%s) Received transaction with address 0x%lX, data: 0x%X\n", m_name.c_str(), trans.get_address(), wdata);
				}
				trans.set_response_status(tlm::TLM_OK_RESPONSE);
				break;
			}
			case tlm::TLM_READ_COMMAND:
			{
				unsigned int value = regs[trans.get_address()].get_value();
				unsigned char* cvalue = reinterpret_cast<unsigned char*>(&value);
				memcpy(trans.get_data_ptr(), cvalue, trans.get_data_length());
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
			if (m_message)
			{
				LOG("(%s) (Target socket) END_REQ received\n", m_name.c_str());
			}
			break;
		}
		default:
			break;
		}
		return tlm::TLM_ACCEPTED;
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
				LOG("(%s) (Initiator socket) BEGIN_REQ received\n", m_name.c_str());
			}
			break;
		}
		case tlm::END_REQ:
		{
			// Handle END_REQ phase
			if (m_message)
			{
				LOG("(%s) (Initiator socket) END_REQ received\n", m_name.c_str());
			}
			if (trans.is_response_error()) 
			{
				if (m_message)
				{
					LOG("(%s) Transaction failed with response status: %s\n", m_name.c_str(), trans.get_response_string().c_str());
				}
			}
			else
			{
				switch (trans.get_command()) {
				case tlm::TLM_READ_COMMAND:
				{
					// Change the address of transaction to destination address
					copy_tlm_generic_payload(current_trans, trans);
					current_trans.set_address(regs[DMADESADDR(m_current_ch)].get_value());
					current_trans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
					current_trans.set_command(tlm::TLM_WRITE_COMMAND);
					e_DMA_forward.notify();

					break;
				}
				case tlm::TLM_WRITE_COMMAND:
				{
					if (m_message)
					{
						LOG("(%s) DMA transfers data from  0x%X to 0x%X\n", m_name.c_str(), regs[DMASRCADDR(m_current_ch)].get_value(), regs[DMADESADDR(m_current_ch)].get_value());
					}
					// Notifying DMA operation done !
					e_DMA_run_done.notify();
					break;
				}
				default:
					break;
				}
			}
			break;
		}
		default:
			break;
		};
		return tlm::TLM_ACCEPTED;
	}

	/**@brief
	* init_registers
	*
	* To initialize registers for DMAC model
	* 
	*/
	void init_registers()
	{
		for (unsigned int i = 0; i < DMA_MAX_CH; i++)
		{
			regs.add_register("DMADESADDR" + std::to_string(i), DMADESADDR(i), 0x00, 0xFFFFFFFF, i, READWRITE);
			regs.add_register("DMASRCADDR" + std::to_string(i), DMASRCADDR(i), 0x00, 0xFFFFFFFF, i, READWRITE);
			regs.add_register("DMADATALENGTH" + std::to_string(i), DMADATALENGTH(i), 0x00, 0xFFFFFFFF, i, READWRITE);
		}
		for (unsigned int i = 0; i < (DMA_MAX_CH/32); i++)
		{
			regs.add_register("DMAREQ" + std::to_string(i), DMAREQ(i), 0x00, 0xFFFFFFFF, i, READWRITE);
			regs.add_register("DMAACK" + std::to_string(i), DMAACK(i), 0x00, 0xFFFFFFFF, i, READWRITE);
			regs.add_register("DMAINT" + std::to_string(i), DMAINT(i), 0x00, 0xFFFFFFFF, i, READWRITE);
			regs.add_register("DMACHEN" + std::to_string(i), DMACHEN(i), 0x00, 0xFFFFFFFF, i, READWRITE);
			
			// Registration call back function
			regs.set_register_callback("DMAREQ" + std::to_string(i), std::bind(&DMAC::cb_DMAREQ, this,
				std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5));
		}

	}

	/**@brief
	* cb_DMAREQ
	*
	* The call back register function for DMAREQ register
	*
	*/
	void cb_DMAREQ(const std::string& name, uint32_t value, uint32_t old_value, uint32_t mask, uint32_t ch)
	{
		// Request DMA operation in here
		if (!m_running)
		{
			m_testmode = true;
			m_cur_reg_ch = ch;
			m_cur_reg_name = name;
			e_DMA_request.notify();
		}
		else
		{
			if (m_message)
			{
				LOG("(%s) DMA is running, cannot request DMA operation in this time !\n", m_name.c_str());
			}
		}
	}



	/* SC_METHOD and SC_THREAD are defined in here */
	
	/**@brief
	* mth_reset
	*
	* Impelmentation of the method when reset is active
	*
	*/
	void mth_reset()
	{
		if (rst.read())
		{
			m_current_ch = 0;
			m_running = false;
			m_testmode = false;
			regs.reset_regs();
			m_cur_reg_ch = 0;
			m_cur_reg_name.clear();
			e_DMA_forward.cancel();
			e_DMA_request.cancel();
			e_DMA_run.cancel();
			e_DMA_run_done.cancel();
		}
	}


	/**@brief
	* mth_request_signals
	*
	* Impelmentation of the method when requests signal are triggered by peripherals
	*
	*/
	void mth_request_signals()
	{
		// SC_THREAD is triggered only one at the same simulation time
		if (!m_running)
		{
			for (unsigned int i = 0; i < DMA_MAX_CH; i++) {
				bool is_enable = (bool)((regs[DMACHEN(i / 32)].get_value() >> i) & 0x01);
				if (is_enable)
				{
					if (DMA_req[i].read())
					{
						if (m_message)
						{
							LOG("(%s) DMA_req[%d] has changed to HIGH\n", m_name.c_str(), i);
						}
						port_req_ids.push_back(i);
					}
				}
			}
			if (!port_req_ids.empty())
			{
				m_running = true;
				e_DMA_request.notify();
			}
		}
		else
		{
			if (m_message)
			{
				LOG("(%s) DMA is running, cannot request DMA operation in this time !\n", m_name.c_str());
			}
		}
	}

	/**@brief
	* thr_priority_process
	*
	* Impelmentation of the thread to handle priority of DMA operation
	*
	*/
	void thr_priority_process()
	{
		while (true)
		{
			// waiting to next cycle
			wait(e_DMA_request);
			wait(clk.posedge_event());
			if (!m_testmode)
			{
				// using ports to trigger DMAC
				while (!port_req_ids.empty())
				{
					m_current_ch = port_req_ids.front();
					port_req_ids.pop_front(); // removing request id from list
					e_DMA_run.notify();	// Bus delay simulation time
					wait(e_DMA_run_done);

					// set output ack and int port
					wait(clk.posedge_event());
					DMA_ack[m_current_ch].write(true);
					DMA_int[m_current_ch].write(true);
					wait(clk.posedge_event());
					DMA_ack[m_current_ch].write(false);
					DMA_int[m_current_ch].write(false);
					// continuting with next channels
				}
				
			}
			else
			{
				// using register to trigger DMAC
				uint32_t value = regs[m_cur_reg_name].get_value();
				for (unsigned int i = 0; i < 32; i++)
				{
					if ((regs[DMACHEN(m_cur_reg_ch)].get_value() >> i) & 0x01)
					{
						if ((value >> i) & 0x01)
						{
							// set current channel
							m_running = true;
							m_current_ch = (m_cur_reg_ch == 0) ? i : (i * m_cur_reg_ch);
							e_DMA_run.notify();	// Bus delay simulation time
							wait(e_DMA_run_done);
							// continuting with next channels
							regs[DMAACK(m_cur_reg_ch)] = (0x01 << i) | regs[DMAACK(m_cur_reg_ch)].get_value();
							regs[DMAINT(m_cur_reg_ch)] = (0x01 << i) | regs[DMAINT(m_cur_reg_ch)].get_value();
						}
					}
				}
			}
			if (m_message)
			{
				LOG("(%s) DMA process is done\n", m_name.c_str());
			}
			// releasing flags
			m_running = false;
			m_testmode = false;
			
		}
	}

	/**@brief
	* thr_DMA_run_process
	*
	* Impelmentation of the thread to start DMA operation
	*
	*/
	void thr_DMA_run_process()
	{
		while (true)
		{
			wait(e_DMA_run);
			// DMA operation in here
			/*
				Initiator socket -> request trans -> get return data trans
			*/
			tlm::tlm_generic_payload trans;
			sc_core::sc_time delay = sc_core::SC_ZERO_TIME;
			unsigned char* data = new unsigned char[regs[DMADATALENGTH(m_current_ch)].get_value()];
			// Configuration transaction
			trans.set_command(tlm::TLM_READ_COMMAND);
			trans.set_address(regs[DMASRCADDR(m_current_ch)].get_value());
			trans.set_data_ptr(data);
			trans.set_data_length(regs[DMADATALENGTH(m_current_ch)].get_value());
			trans.set_streaming_width(regs[DMADATALENGTH(m_current_ch)].get_value()); // = data_length to indicate no streaming
			trans.set_byte_enable_ptr(0); // 0 indicates unused
			trans.set_dmi_allowed(false); // DMI not allowed
			trans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
			// Sending transaction using initiator socket
			tlm::tlm_phase fw_phase = tlm::BEGIN_REQ;
			tlm::tlm_sync_enum status;
			status = initiator_socket->nb_transport_fw(trans, fw_phase, delay);
		}
	}

	/**@brief
	* thr_DMA_forward_process
	*
	* Impelmentation of the thread to forward data from the source to the destination
	*
	*/
	void thr_DMA_forward_process()
	{
		while (true)
		{
			// waiting when receiving data from slave
			wait(e_DMA_forward);
			tlm::tlm_phase fw_phase = tlm::BEGIN_REQ;
			tlm::tlm_sync_enum status;
			sc_core::sc_time delay = sc_core::SC_ZERO_TIME;
			status = initiator_socket->nb_transport_fw(current_trans, fw_phase, delay);
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

	/**@brief
	* DMAC constructor
	*
	* @param name Reference to sc_module name
	*
	* @param message To enable message log
	*/
	DMAC(sc_core::sc_module_name name, bool message = false) :
		sc_core::sc_module(name)
		, m_name(name)
		, m_message(message)
		, target_socket("target_socket")
		, initiator_socket("initiator_socket")
		, m_current_ch(0)
		, m_running(false)
		, m_testmode(false)
	{
		target_socket.register_nb_transport_fw(this, &DMAC::nb_transport_fw);
		initiator_socket.register_nb_transport_bw(this, &DMAC::nb_transport_bw);
		// initialization registers
		init_registers();
		// Registration Method and thread
		SC_METHOD(mth_request_signals);
		for (int i = 0; i < DMA_MAX_CH; ++i) 
		{
			sensitive << DMA_req[i]; // Register each input port with the SC_METHOD
		}
		dont_initialize();

		SC_THREAD(mth_reset);
		sensitive << rst;

		SC_THREAD(thr_priority_process);
		sensitive << e_DMA_request;

		SC_THREAD(thr_DMA_run_process);
		sensitive << e_DMA_run;

		SC_THREAD(thr_DMA_forward_process);
		sensitive << e_DMA_forward;
	};

	/**@brief
	* DMAC destructor
	*/
	~DMAC() {};

};


#endif