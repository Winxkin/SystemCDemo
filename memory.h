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

template<unsigned int BUSWIDTH = 32>
class Memory : public sc_core::sc_module
{
private:
    

public:
	tlm_utils::simple_target_socket<DummySlave, BUSWIDTH> target_socket;

    Memory(sc_core::sc_module_name name) :
        sc_core::sc_module(name),
        m_name(name),
        target_socket("target_socket")
    {
        target_socket.register_b_transport(this, &DummySlave::b_transport);
    }

    void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay) {
        // Handle transaction

        switch (trans.get_command()) {
        case tlm::TLM_WRITE_COMMAND:
        {
            unsigned int wdata = 0;
            std::memcpy(&wdata, trans.get_data_ptr(), sizeof(wdata));
            
            std::cout << m_name << ": Received transaction with address 0x" << std::hex << trans.get_address() << " data: 0x" << std::hex << wdata << std::dec << std::endl;
            trans.set_response_status(tlm::TLM_OK_RESPONSE);
        }
        case tlm::TLM_READ_COMMAND:
        {
            unsigned int rdata = 0;
            
            std::cout << m_name << ": Received transaction with address 0x" << std::hex << trans.get_address() << " data: 0x" << std::hex << rdata << std::dec << std::endl;
            std::memcpy(trans.get_data_ptr(), &rdata, trans.get_data_length());
            trans.set_response_status(tlm::TLM_OK_RESPONSE);
            break;
        }
        default:
            break;
        }
    }

};
#endif