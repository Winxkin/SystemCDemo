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
#include "commondef.h"

template<unsigned int BUSWIDTH = 32>
class Target : public sc_core::sc_module {

private:
    std::string m_name;

private:

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

			LOG("(%s) BEGIN_REQ received\n", m_name.c_str());

			unsigned char* data = new unsigned char[trans.get_data_length()];

			switch (trans.get_command()) {
			case tlm::TLM_WRITE_COMMAND:
			{
				std::memcpy(data, trans.get_data_ptr(), trans.get_data_length());

				LOG("(%s) Received transaction with address 0x%X  data: ", m_name.c_str(), trans.get_address());

				for (unsigned int i = 0; i < trans.get_data_length(); i++)
				{
					std::cout << " [0x" << std::hex << (unsigned int)data[i] << "]";
				}
				std::cout << std::endl;

				trans.set_response_status(tlm::TLM_OK_RESPONSE);
				break;
			}
			case tlm::TLM_READ_COMMAND:
			{
				for (unsigned int i = 0; i < trans.get_data_length(); i++)
				{
					data[i] = (unsigned char) i;
				}

				LOG("(%s) Received transaction with address 0x%X", m_name.c_str(), trans.get_address());
				
				std::memcpy(trans.get_data_ptr(), data, trans.get_data_length());
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
			LOG("(%s) END_REQ received\n", m_name.c_str());
			break;
		}
		default:
			break;
		}
        return tlm::TLM_ACCEPTED;
    }


public:
    tlm_utils::simple_target_socket<Target, BUSWIDTH> target_socket;

	/**@brief
	* Target
	*
	* Target Constructure
	* @param name Reference to sc_module name
	*/
	Target(sc_core::sc_module_name name) :
		sc_core::sc_module(name)
		,m_name(name)
		,target_socket("target_socket")
		{
			target_socket.register_nb_transport_fw(this, &Target::nb_transport_fw);
		};

};

#endif