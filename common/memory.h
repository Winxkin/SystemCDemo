/*
 *	Author: Huan Nguyen-Duy
 *  Date: 12/07/2024
 */

#ifndef _MEMORY_H
#define _MEMORY_H
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
#include <iomanip>

template<unsigned int BUSWIDTH = 32>
class RAM : public sc_core::sc_module
{
private:
    unsigned char* data;
    sc_dt::uint64 size;
	std::string m_name;
	bool m_message;
public:
    tlm_utils::simple_target_socket<RAM, BUSWIDTH> socket;

    RAM(sc_core::sc_module_name name, sc_dt::uint64 size, bool message = false) :
        sc_module(name)
		, m_name(name)
        , socket("socket")
        , size(size)
		, m_message(message)
	{
        socket.register_nb_transport_fw(this, &RAM::nb_transport_fw);
        data = new unsigned char[size];
		std::memset(data, 0, size);
    }

	~RAM() {
		delete[] data;
	}

	tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_core::sc_time& delay) 
	{
		tlm::tlm_phase next_phase = tlm::END_REQ;

		switch (phase)
		{
		case tlm::BEGIN_REQ:
		{
			// Handle BEGIN_REQ phase

			// Check size 
			if (trans.get_address() + trans.get_data_length() > size)
			{
				trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE); // return respond error.
				std::cout << m_name << "	Error: Out of range size !" << std::endl;
			}
			else
			{
				// handling data
				switch (trans.get_command()) {
				case tlm::TLM_WRITE_COMMAND:
				{
					memcpy(&data[trans.get_address()], trans.get_data_ptr(), trans.get_data_length());
					break;
				}
				case tlm::TLM_READ_COMMAND:
				{
					memcpy(trans.get_data_ptr(), &data[trans.get_address()], trans.get_data_length());
					break;
				}
				default:
					break;
				}
			}
			/*
				Return data and END_REQ to bus
			*/
			socket->nb_transport_bw(trans, next_phase, delay);

		}
		case tlm::END_REQ:
		{
			// Handle END_REQ phase (shouldn't happen here)
			if (m_message)
			{
				std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << " END_REQ received" << std::endl;
			}
			break;
		}
		default:
			break;
		}
		return tlm::TLM_ACCEPTED;
	}

	void dump_memory(sc_dt::uint64 offset, unsigned int len) {
		if (offset + len > size) {
			std::cerr << m_name << "	Error: Address out of bounds" << std::endl;
			return;
		}

		for (unsigned int i = 0; i < len; i += 8) {
			std::cout << "[" << std::hex << std::setw(8) << std::setfill('0') << offset + i << "] ";
			for (unsigned int j = 0; j < 8 && (i + j) < len; ++j) {
				std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data[offset + i + j]) << " ";
			}
			std::cout << std::endl;
		}
	}

	std::string get_name()
	{
		return m_name;
	}

};
#endif