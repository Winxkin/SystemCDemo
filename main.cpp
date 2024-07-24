// All systemc modules should include systemc.h header file
#include <systemc>
#include <tlm>
#include <iostream>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include "common/bus.h"
#include "common/DummyMaster.h"
#include "common/DummySlave.h"
#include "common/target.h"
#include "common/memory.h"

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


// sc_main in top level function like in C++ main
/*
int sc_main(int argc, char* argv[]) {

    sc_core::sc_clock sysclk("sysclk", 10, sc_core::SC_NS);

    BUS<APB,5> bus("bus_APB", false);
    bus.m_clk(sysclk);
    DummyMaster<32> m_dummymaster("DummyMaster");
    Target target1("Target1");
    Target target2("Target2");
    Target target3("Target3");
    Target target4("Target4");
    RAM ram("ram1", 1024);
    m_dummymaster.initiator_socket.bind(bus.target_sockets);
    bus.mapping_target_sockets(0, 0x0000, 0x1000).bind(target1.target_socket);
    bus.mapping_target_sockets(1, 0x1000, 0x1000).bind(target2.target_socket);
    bus.mapping_target_sockets(2, 0x2000, 0x1000).bind(target3.target_socket);
    bus.mapping_target_sockets(3, 0x3000, 0x1000).bind(target4.target_socket);
    bus.mapping_target_sockets(4, 0x4000, 0x3000).bind(ram.socket);

    sc_core::sc_start(100, sc_core::SC_NS);
    
    unsigned char* dta = new unsigned char[255];

    for (unsigned int i = 0; i < 255; i++)
    {
        dta[i] = i;
    }
    m_dummymaster.Sentcustomtransaction(0x4000, dta, 255, tlm::TLM_WRITE_COMMAND);

    sc_core::sc_start(10000, sc_core::SC_NS);

    ram.dump_memory(0, 1024);
    unsigned char* readdata = new unsigned char[8];
    m_dummymaster.Sentcustomtransaction(0x4000, readdata, 8, tlm::TLM_READ_COMMAND);
    sc_core::sc_start(100, sc_core::SC_NS);
    readdata = m_dummymaster.get_received_data();

    std::cout << "Simulation Time: " << sc_core::sc_time_stamp().to_default_time_units() << "SC_NS" << std::endl;
    
    return(0);
}
*/

SC_MODULE(MyModule) {
    sc_in<bool> sig_a;
    sc_in<bool> sig_b;

    SC_CTOR(MyModule) {
        SC_METHOD(monitor_signals);
        sensitive << sig_a << sig_b; // Sensitive to both signals
    }

    void monitor_signals() {
        std::cout << "monitor_signals called at time " << sc_time_stamp() << std::endl;
        std::cout << "sig_a: " << sig_a.read() << ", sig_b: " << sig_b.read() << std::endl;
    }
};

int sc_main(int argc, char* argv[]) {
    sc_signal<bool> signal_a, signal_b;

    MyModule my_module("my_module");
    my_module.sig_a(signal_a);
    my_module.sig_b(signal_b);

    // Initialize signals
    signal_a = false;
    signal_b = false;

    sc_start(1, SC_NS);

    // Change both signals at the same time
    signal_a = true;
    signal_b = true;

    sc_start(1, SC_NS);

    signal_a = true;
    signal_b = false;

    sc_start(1, SC_NS);

    signal_a = false;
    signal_b = true;

    sc_start(1, SC_NS);

    signal_a = false;
    signal_b = false;

    sc_start(1, SC_NS);

    return 0;
}