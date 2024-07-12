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

template<unsigned int BUSWIDTH = 32>
class DummyMaster : public sc_core::sc_module
{
private:
	std::string m_name;
public:
	tlm_utils::simple_initiator_socket<DummyMaster, BUSWIDTH> initiator_socket;

	DummyMaster(sc_core::sc_module_name name) :
		sc_core::sc_module(name),
		initiator_socket("initiator_socket"),
		m_name(name)
	{

	}

	void Write_reg(unsigned int addr, uint32_t data)
	{
		tlm::tlm_generic_payload trans;
		unsigned char* _data = reinterpret_cast<unsigned char*>(&data);
		sc_core::sc_time delay = sc_core::SC_ZERO_TIME;
		trans.set_command(tlm::TLM_WRITE_COMMAND);
		trans.set_address(addr);
		trans.set_data_length(sizeof(data));
		trans.set_data_ptr(_data);
		trans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

		std::cout << m_name << ": Sending transaction with address 0x" << std::hex << addr << std::dec << std::endl;
		initiator_socket->b_transport(trans, delay);

		if (trans.is_response_error()) {
			std::cout << m_name << ": Transaction failed with response status: " << trans.get_response_string() << std::endl;
		}
		else {
			std::cout << m_name << ": Transaction succeeded" << std::endl;
		}
	}

	unsigned int Read_reg(unsigned int addr)
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

		std::cout << m_name << ": Sending transaction with address 0x" << std::hex << addr << std::dec << std::endl;
		initiator_socket->b_transport(trans, delay);
		// Check the response status
		if (trans.is_response_error()) {
			SC_REPORT_ERROR("TLM-2", "Response error from b_transport");
			return -1;
		}
		else {
			// Print the read value
			std::cout << m_name << ": Read data: 0x" << std::hex << data << std::endl;
			return data;
		}

	}

};


#endif