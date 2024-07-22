#ifndef _TARGET_H
#define _TARGET_H
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

template<unsigned int BUSWIDTH = 32>
class Target : public sc_core::sc_module {

private:
    std::string m_name;

    tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_core::sc_time& delay) {
		switch (phase)
		{
		case tlm::BEGIN_REQ:
		{
			// Handle BEGIN_REQ phase
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << " BEGIN_REQ received" << std::endl;

			uint32_t data = 0;

			switch (trans.get_command()) {
			case tlm::TLM_WRITE_COMMAND:
			{
				std::memcpy(&data, trans.get_data_ptr(), sizeof(data));
				std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << ": Received transaction with address 0x" << std::hex << trans.get_address() << " data: 0x" << std::hex << data << std::dec << std::endl;
				trans.set_response_status(tlm::TLM_OK_RESPONSE);
				break;
			}
			case tlm::TLM_READ_COMMAND:
			{
				unsigned int data = 0x11223344;
				std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << ": Received transaction with address 0x" << std::hex << trans.get_address() << std::endl;
				std::memcpy(trans.get_data_ptr(), &data, trans.get_data_length());
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

		}
		case tlm::END_REQ:
		{
			// Handle END_REQ phase (shouldn't happen here)
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << " END_REQ received" << std::endl;
			break;
		}
		default:
			break;
		}
        return tlm::TLM_ACCEPTED;
    }


public:
    tlm_utils::simple_target_socket<Target, BUSWIDTH> target_socket;


	Target(sc_core::sc_module_name name) :
		sc_core::sc_module(name)
		,m_name(name)
		,target_socket("target_socket")
		{
			target_socket.register_nb_transport_fw(this, &Target::nb_transport_fw);
		};

};

#endif