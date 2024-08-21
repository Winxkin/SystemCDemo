/*
 *	Author: Huan Nguyen-Duy
 *  Date: 12/07/2024
 */

#ifndef _DUMMYMASTER_H
#define _DUMMYMASTER_H
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
#include "bus.h"
#include "commondef.h"

template<unsigned int BUSWIDTH = 32>
class DummyMaster : public sc_core::sc_module
{
private:
	std::string m_name;
	bool m_message;
	struct localdata
	{
		unsigned char* m_data;
		unsigned int m_length;
	};

	localdata tempdata;
	sc_core::sc_event e_reset;

private:

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
				LOG("(%s) BEGIN_REQ received\n", m_name.c_str());
			}
			break;
		}
		case tlm::END_REQ:
		{
			// Handle END_REQ phase (shouldn't happen here)
			if (m_message)
			{
				LOG("(%s) END_REQ received\n", m_name.c_str());
			}
			if (trans.is_response_error()) {
				if (m_message)
				{
					LOG("(%s) Transaction failed with response status: %s\n", m_name.c_str(), trans.get_response_string().c_str());
				}
			}
			else {
				switch (trans.get_command()) {
				case tlm::TLM_READ_COMMAND:
				{
					tempdata.m_data = new unsigned char[trans.get_data_length()];
					std::memcpy(tempdata.m_data, trans.get_data_ptr(), trans.get_data_length());

					// dump message
					if (m_message)
					{
						LOG("(%s) Received data: ", m_name.c_str());
						for (unsigned int i = 0; i < trans.get_data_length(); i++)
						{
							std::cout << "[0x" << std::hex << (unsigned int)tempdata.m_data[i] << "]	";
						}
					}
					std::cout << std::endl;

					trans.set_response_status(tlm::TLM_OK_RESPONSE);
					break;
				}
				default:
					break;
				}
				if (m_message)
				{
					LOG("(%s) Transaction succeeded\n", m_name.c_str());
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
	 * resetsystem
	 * To triggered reset signal
	 */
	void resetsystem()
	{
		e_reset.notify();
	}

	/**@brief
	 * resetsystem
	 * Reset thread handling
	 */
	void reset_process()
	{
		while (true)
		{
			wait(e_reset);
			rst.write(true);
			wait(clk.posedge_event());
			rst.write(false);
		}
	}

public:
	tlm_utils::simple_initiator_socket<DummyMaster, BUSWIDTH> initiator_socket;
	sc_core::sc_in<bool> clk;
	sc_core::sc_out<bool> rst;

	/**@brief
	 * DummyMaster
	 *
	 * The constructor of DummyMaster
	 *
	 */
	DummyMaster(sc_core::sc_module_name name, bool message = false) :
		 sc_core::sc_module(name)
		 , initiator_socket("initiator_socket")
		 , m_name(name)
		 , m_message(message)
		 {
			initiator_socket.register_nb_transport_bw(this, &DummyMaster::nb_transport_bw);

			SC_THREAD(reset_process);
			sensitive << e_reset;
		 }

	/**@brief
	 * get_received_data
	 * 
	 * Using to get data from the returned transaction
	 *
	 * @return the array of data
	 * 
	 */
	unsigned char* get_received_data()
	{
		return tempdata.m_data;
	}

	/**@brief
	 * get_received_data
	 *
	 * Using to get data from the returned transaction
	 *
	 * @return the 32-bit data
	 *
	 */
	unsigned int get_received_32bit_data()
	{
		return *reinterpret_cast<unsigned int*>(tempdata.m_data);
	}

	/**@brief
	 * SentTransaction
	 *
	 * Implements the sent a transaction with 32 bit data to bus MMIO
	 *
	 * @param addr  Reference to the address of the slave
	 *
	 * @param data	Reference to 32-bit data
	 *
	 * @param cmd   Reference to tlm command
	 *				-TLM_READ_COMMAND  : TLM read requset
	 *				-TLM_WRITE_COMMAND : TLM write requset
	 *				-TLM_IGNORE_COMMAND: TLM ignore requset
	 */
	void SentTransaction(unsigned int addr, uint32_t data, tlm::tlm_command cmd)
	{
		switch (cmd) {
		case tlm::TLM_WRITE_COMMAND:
		{
			tlm::tlm_generic_payload trans;
			unsigned char* _data = reinterpret_cast<unsigned char*>(&data);
			sc_core::sc_time delay = sc_core::SC_ZERO_TIME;
			trans.set_command(tlm::TLM_WRITE_COMMAND);
			trans.set_address(addr);
			trans.set_data_length(sizeof(data));
			trans.set_data_ptr(_data);
			trans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
			if (m_message)
			{
				LOG("(%s) Sending Write transaction with address 0x%X, data: 0x%X\n", m_name.c_str(), addr, data);
			}
			
			tlm::tlm_phase phase = tlm::BEGIN_REQ;
			tlm::tlm_sync_enum status;
			status = initiator_socket->nb_transport_fw(trans, phase, delay);

			break;
			
		}
		case tlm::TLM_READ_COMMAND:
		{
			tlm::tlm_generic_payload trans;
			unsigned int data;
			sc_core::sc_time delay = sc_core::SC_ZERO_TIME;

			trans.set_command(tlm::TLM_READ_COMMAND);
			trans.set_address(addr);
			trans.set_data_ptr(reinterpret_cast<unsigned char*>(&data));
			trans.set_data_length(sizeof(data));
			trans.set_streaming_width(sizeof(data)); // = data_length to indicate no streaming
			trans.set_byte_enable_ptr(0); // 0 indicates unused
			trans.set_dmi_allowed(false); // DMI not allowed
			trans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
			if (m_message)
			{
				LOG("(%s) Sending Read transaction with address 0x%X, data: 0x%X\n", m_name.c_str(), addr, data);
			}
			tlm::tlm_phase phase = tlm::BEGIN_REQ;
			tlm::tlm_sync_enum status;
			status = initiator_socket->nb_transport_fw(trans, phase, delay);

			break;
		}
		default:
			break;
		}
	}

	/**@brief
	 * Sentcustomtransaction
	 *
	 * Implements the sent a custom transaction to bus MMIO
	 *
	 * @param addr  Reference to the address of the slave
	 *
	 * @param data	Reference to the pointer of array data
	 * 
	 * @param data_length Reference to the length of data
	 *
	 * @param cmd   Reference to tlm command
	 *				-TLM_READ_COMMAND  : TLM read requset
	 *				-TLM_WRITE_COMMAND : TLM write requset
	 *				-TLM_IGNORE_COMMAND: TLM ignore requset
	 */
	void Sentcustomtransaction(unsigned int addr, unsigned char* data, unsigned int data_length, tlm::tlm_command cmd)
	{
		switch (cmd) {
		case tlm::TLM_WRITE_COMMAND:
		{
			tlm::tlm_generic_payload trans;
			sc_core::sc_time delay = sc_core::SC_ZERO_TIME;
			trans.set_command(tlm::TLM_WRITE_COMMAND);
			trans.set_address(addr);
			trans.set_data_length(data_length);
			trans.set_data_ptr(data);
			trans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

			if (m_message)
			{
				LOG("(%s) Sending Write transaction with address 0x%X\n", m_name.c_str(), addr);
			}
			tlm::tlm_phase phase = tlm::BEGIN_REQ;
			tlm::tlm_sync_enum status;
			status = initiator_socket->nb_transport_fw(trans, phase, delay);

			break;

		}
		case tlm::TLM_READ_COMMAND:
		{
			tlm::tlm_generic_payload trans;
			sc_core::sc_time delay = sc_core::SC_ZERO_TIME;

			trans.set_command(tlm::TLM_READ_COMMAND);
			trans.set_address(addr);
			trans.set_data_ptr(data);
			trans.set_data_length(data_length);
			trans.set_streaming_width(data_length); // = data_length to indicate no streaming
			trans.set_byte_enable_ptr(0); // 0 indicates unused
			trans.set_dmi_allowed(false); // DMI not allowed
			trans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
			if (m_message)
			{
				LOG("(%s) Sending Write transaction with address 0x%X\n", m_name.c_str(), addr);
			}
			tlm::tlm_phase phase = tlm::BEGIN_REQ;
			tlm::tlm_sync_enum status;
			status = initiator_socket->nb_transport_fw(trans, phase, delay);

			break;
		}
		default:
			break;
		}
	}
};


#endif