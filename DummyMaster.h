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

template<unsigned int BUSWIDTH = 32>
class DummyMaster : public sc_core::sc_module
{
private:
	std::string m_name;
	tlm::tlm_generic_payload current_trans;

private:

	tlm::tlm_sync_enum nb_transport_bw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_core::sc_time& delay) {
		switch (phase)
		{
		case tlm::BEGIN_REQ:
		{
			// Handle BEGIN_REQ phase
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << "	BEGIN_REQ received" << std::endl;
			break;
		}
		case tlm::END_REQ:
		{
			// Handle END_REQ phase (shouldn't happen here)
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << "	END_REQ received" << std::endl;
			if (trans.is_response_error()) {
				std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << ": Transaction failed with response status: " << trans.get_response_string() << std::endl;
			}
			else {
				switch (trans.get_command()) {
				case tlm::TLM_READ_COMMAND:
				{
					unsigned int wdata = 0;
					std::memcpy(&wdata, trans.get_data_ptr(), sizeof(wdata));
					std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << ": Received data: 0x" << std::hex << wdata << std::dec << std::endl;
					trans.set_response_status(tlm::TLM_OK_RESPONSE);
					break;
				}
				default:
					break;
				}
				std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << ": Transaction succeeded" << std::endl;
			}
			break;
		}
		default:
			break;
		};
		return tlm::TLM_ACCEPTED;
	}

public:
	tlm_utils::simple_initiator_socket<DummyMaster, BUSWIDTH> initiator_socket;

	DummyMaster(sc_core::sc_module_name name) :
		sc_core::sc_module(name),
		initiator_socket("initiator_socket"),
		m_name(name)
	{
		initiator_socket.register_nb_transport_bw(this, &DummyMaster::nb_transport_bw);
	}

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

			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << ": Sending Write transaction with address 0x" << std::hex << addr 
				<< "	,data: 0x" << std::hex << data << std::endl;
			
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

			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << ": Sending Read transaction with address 0x" << std::hex << addr << std::dec << std::endl;
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