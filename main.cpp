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



// Hello_world is module name
SC_MODULE(hello_world) {
    SC_CTOR(hello_world) {
        // Nothing in constructor 
    }
    void say_hello() {
        //Print "Hello World" to the console.
        cout << "Hello World.\n";
    }
};

// sc_main in top level function like in C++ main
int sc_main(int argc, char* argv[]) {
    hello_world hello("HELLO");
    // Print the hello world
    hello.say_hello();
    
    
    BUS<APB,5> bus("bus_APB");
    DummyMaster<32> m_dummymaster("dummy_master");

    m_dummymaster.initiator_socket.bind(bus.target_sockets);

    Initiator Initiator1("Initiator1");
    
    Initiator1.initiator_socket.bind(bus.target_sockets);
    //Initiator Initiator2("Initiator2");
    Target Target1("Target1");
    Target Target2("Target2");
    Target Target3("Target3");
    Target Target4("Target4");

    DummySlave m_dummyslave("Dummy_slave");

    bus.mapping_target_sockets(0, 0x0000, 0x1000).bind(Target1.target_socket);
    bus.mapping_target_sockets(1, 0x1000, 0x1000).bind(Target2.target_socket);
    bus.mapping_target_sockets(2, 0x2000, 0x1000).bind(Target3.target_socket);
    bus.mapping_target_sockets(3, 0x3000, 0x1000).bind(Target4.target_socket);
    bus.mapping_target_sockets(4, 0x4000, 0x1000).bind(m_dummyslave.target_socket);

    m_dummyslave.initiator_socket.bind(bus.target_sockets);

    sc_core::sc_start();

    m_dummymaster.Write_reg(0x0000, 0x11);
    sc_core::sc_start(12, sc_core::SC_NS);

    //m_dummymaster.Write_reg(0x0100, 0x11);
    //sc_core::sc_start(11, sc_core::SC_NS);

    //m_dummymaster.Write_reg(0x1000, 0x22);
    //sc_core::sc_start(11, sc_core::SC_NS);

    //m_dummymaster.Write_reg(0x1200, 0x22);
    //sc_core::sc_start(11, sc_core::SC_NS);

    //m_dummymaster.Write_reg(0x2000, 0x33);
    //sc_core::sc_start(5, sc_core::SC_NS);

    //m_dummymaster.Write_reg(0x2200, 0x33);
    //sc_core::sc_start(5, sc_core::SC_NS);

    //m_dummymaster.Write_reg(0x3000, 0x44);
    //sc_core::sc_start(5, sc_core::SC_NS);

    //m_dummymaster.Write_reg(0x3300, 0x44);
    //sc_core::sc_start(5, sc_core::SC_NS);
    
    //m_dummymaster.Read_reg(0x0000);
    //m_dummymaster.Read_reg(0x1000);
    //m_dummymaster.Read_reg(0x2000);
    //m_dummymaster.Read_reg(0x3000);

    //m_dummymaster.Write_reg(0x4000, 0xff);

    std::cout << "Simulation Time: " << sc_core::sc_time_stamp().to_default_time_units() << "SC_NS" << std::endl;
    
    return(0);
}