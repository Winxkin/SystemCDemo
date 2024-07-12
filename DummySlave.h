/*
 *	Author: Huan Nguyen-Duy
 *  Date: 12/07/2024
 */

#ifndef _DUMMYSLAVE_H
#define _DUMMYSLAVE_H
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
class DummySlave : public sc_core::sc_module
{
private:
    std::string m_name;
    RegisterInterface regs;

private:
    void InitializeRegister()
    {
        regs.add_register("DummySlave_RESULT", 0x00, 0);
        regs.set_register_callback("DummySlave_RESULT", std::bind(&DummySlave::cb_DummySlave_RESULT, this, std::placeholders::_1, std::placeholders::_2));
    }

    void cb_DummySlave_RESULT(const std::string& name, uint32_t value)
    {
        if (name == "DummySlave_RESULT")
        {
            if (value == 0x0)
            {
                std::cout << "-----------------------\n";
                std::cout << "      TM is Pass       \n";
                std::cout << "-----------------------\n";
                exit(0);
            }
            else
            {
                std::cout << "-----------------------\n";
                std::cout << "      TM is Fail       \n";
                std::cout << "-----------------------\n";
                exit(0);
            }
        }
    }


public:
    tlm_utils::simple_initiator_socket<DummySlave, BUSWIDTH> initiator_socket;
    tlm_utils::simple_target_socket<DummySlave, BUSWIDTH> target_socket;

    DummySlave(sc_core::sc_module_name name) :
        sc_core::sc_module(name),
        m_name(name),
        target_socket("target_socket")
    {
        target_socket.register_b_transport(this, &DummySlave::b_transport);
        InitializeRegister();
    }

    void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay) {
        // Handle transaction

        switch (trans.get_command()) {
        case tlm::TLM_WRITE_COMMAND:
        {   
            unsigned int wdata = 0;
            std::memcpy(&wdata, trans.get_data_ptr(), sizeof(wdata));
            regs[trans.get_address()] = wdata;
            std::cout << m_name << ": Received transaction with address 0x" << std::hex << trans.get_address() << " data: 0x" << std::hex << wdata << std::dec << std::endl;
            trans.set_response_status(tlm::TLM_OK_RESPONSE);
        }
        case tlm::TLM_READ_COMMAND:
        {
            unsigned int rdata = 0;
            rdata =(unsigned int)regs[trans.get_address()].get_value();
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