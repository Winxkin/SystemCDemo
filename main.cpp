// All systemc modules should include systemc.h header file
#include <systemc>
#include <tlm>
#include <iostream>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include "bus.h"

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

            std::cout << "Initiator: Sending transaction with address 0x" << std::hex << i * 0x1000 << std::dec << std::endl;
            initiator_socket->b_transport(trans, delay);

            if (trans.is_response_error()) {
                std::cout << "Initiator: Transaction failed with response status: " << trans.get_response_string() << std::endl;
            }
            else {
                std::cout << "Initiator: Transaction succeeded" << std::endl;
            }
        }
    }
};

class Target : public sc_core::sc_module {
public:
    tlm_utils::simple_target_socket<Target,32> target_socket;

    SC_CTOR(Target) : target_socket("target_socket") {
        target_socket.register_b_transport(this, &Target::b_transport);
    }

    void b_transport(tlm::tlm_generic_payload& trans, sc_time& delay) {
        // Handle transaction
        std::cout << "Target: Received transaction with address 0x" << std::hex << trans.get_address() << std::dec << std::endl;
        trans.set_response_status(tlm::TLM_OK_RESPONSE);
    }
};

class Targett : public sc_core::sc_module {
public:
    tlm_utils::simple_target_socket<Targett, 32> target_socket;

    SC_CTOR(Targett) : target_socket("target_socket") {
        target_socket.register_b_transport(this, &Targett::b_transport);
    }

    void b_transport(tlm::tlm_generic_payload& trans, sc_time& delay) {
        // Handle transaction
        std::cout << "Targett: Received transaction with address 0x" << std::hex << trans.get_address() << std::dec << std::endl;
        trans.set_response_status(tlm::TLM_OK_RESPONSE);
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
    BUS<APB,10> bus("bus_APB");

    Initiator Initiator1("Initiator1");
    Initiator Initiator2("Initiator2");
    Target Target1("Target1");
    Targett Target2("Target2");
    Target Target3("Target3");
    Target Target4("Target4");

    Initiator1.initiator_socket.bind(bus.target_sockets);
    Initiator2.initiator_socket.bind(bus.target_sockets);
    bus.mapping_target_sockets(0, 0x0000, 0x1000).bind(Target1.target_socket);
    bus.mapping_target_sockets(1, 0x1000, 0x1000).bind(Target2.target_socket);
    bus.mapping_target_sockets(2, 0x2000,0x1000).bind(Target3.target_socket);
    bus.mapping_target_sockets(3, 0x4000, 0x1000).bind(Target4.target_socket);

    Initiator1.thread_process();

    return(0);
}