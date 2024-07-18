// All systemc modules should include systemc.h header file
#include <systemc>
#include <tlm>
#include <iostream>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include "bus.h"
#include "DummyMaster.h"
#include "DummySlave.h"

using namespace sc_core;
using namespace std;

class Initiator : public sc_core::sc_module {
public:
    tlm_utils::simple_initiator_socket<Initiator,32> initiator_socket;

    SC_CTOR(Initiator) : initiator_socket("initiator_socket") {
        //SC_THREAD(thread_process);
    }

    void thread_process() {
        tlm::tlm_generic_payload trans;
        sc_time delay = SC_ZERO_TIME;

        for (int i = 0; i < 4; ++i) {
            trans.set_address(i * 0x1000);
            trans.set_data_length(4);
            trans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

            std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]"  << "Initiator: Sending transaction with address 0x" << std::hex << i * 0x1000 << std::dec << std::endl;
            initiator_socket->b_transport(trans, delay);

            if (trans.is_response_error()) {
                std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]"  << "Initiator: Transaction failed with response status: " << trans.get_response_string() << std::endl;
            }
            else {
                std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]"  << "Initiator: Transaction succeeded" << std::endl;
            }
        }
    }
};

class Target : public sc_core::sc_module {
public:
    tlm_utils::simple_target_socket<Target,32> target_socket;
    
    std::string m_name;

    Target(sc_core::sc_module_name name) :
        sc_core::sc_module(name),
        m_name(name),
        target_socket("target_socket")
    {
        target_socket.register_b_transport(this, &Target::b_transport);
    }

    void b_transport(tlm::tlm_generic_payload& trans, sc_time& delay) {
        // Handle transaction
        uint32_t data = 0;

        switch (trans.get_command()) {
        case tlm::TLM_WRITE_COMMAND:
        {
            std::memcpy(&data, trans.get_data_ptr(), sizeof(data));
            std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << ": Received transaction with address 0x" << std::hex << trans.get_address() << " data: 0x" << std::hex << data << std::dec << std::endl;
            trans.set_response_status(tlm::TLM_OK_RESPONSE);
        }
        case tlm::TLM_READ_COMMAND:
        {
            unsigned int data = 0x11223344;
            std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]"  << m_name << ": Received transaction with address 0x" << std::hex << trans.get_address() << " data: 0x" << std::hex << data << std::dec << std::endl;
            std::memcpy(trans.get_data_ptr(), &data, trans.get_data_length());
            trans.set_response_status(tlm::TLM_OK_RESPONSE);
            break;
        }
        default:
            break;
        }
    }
};


// sc_main in top level function like in C++ main
int sc_main(int argc, char* argv[]) {

    sc_core::sc_clock sysclk("sysclk", 10, sc_core::SC_NS);

    BUS<APB,5> bus("bus_APB");
    bus.m_clk(sysclk);

    std::cout << "Simulation Time: " << sc_core::sc_time_stamp().to_default_time_units() << "SC_NS" << std::endl;
    
    return(0);
}