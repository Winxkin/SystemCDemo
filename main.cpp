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
#include "common/DMAC.h"

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

int sc_main(int argc, char* argv[]) {

    sc_core::sc_clock sysclk("sysclk", 10, sc_core::SC_PS);
    sc_signal<bool> rst;
    sc_signal<bool> dma_req[256];
    sc_signal<bool> dma_ack[256];
    sc_signal<bool> dma_int[256];


    BUS<APB,6> bus("bus_APB", false);
    bus.m_clk(sysclk);
    DummyMaster<32> m_dummymaster("DummyMaster");
    DMAC<32> m_dmac("Dmac");

    m_dmac.clk.bind(sysclk);
    m_dmac.rst.bind(rst);


    for (unsigned int i = 0; i < 256; i++)
    {
        m_dmac.DMA_req[i].bind(dma_req[i]);
        m_dmac.DMA_int[i].bind(dma_int[i]);
        m_dmac.DMA_ack[i].bind(dma_ack[i]);
    }

    for (unsigned int i = 0; i < 256; i++)
    {
        dma_req[i].write(false);
    }

    Target target1("Target1");
    Target target2("Target2");
    Target target3("Target3");
    Target target4("Target4");
    RAM ram("ram1", 1024);
    m_dummymaster.initiator_socket.bind(bus.target_sockets);
    m_dmac.initiator_socket.bind(bus.target_sockets);

    bus.mapping_target_sockets(0, 0x0000, 0x1000).bind(target1.target_socket);
    bus.mapping_target_sockets(1, 0x2000, 0x3000).bind(ram.socket);
    bus.mapping_target_sockets(2, 0x5000, 0x1000).bind(m_dmac.target_socket);
    bus.mapping_target_sockets(3, 0x6000, 0x1000).bind(target2.target_socket);
    bus.mapping_target_sockets(4, 0x7000, 0x1000).bind(target3.target_socket);
    bus.mapping_target_sockets(5, 0x8000, 0x1000).bind(target4.target_socket);

    sc_core::sc_start(5, sc_core::SC_NS);
    
    unsigned char* dta = new unsigned char[32];

    for (unsigned int i = 0; i < 32; i++)
    {
        dta[i] = i;
    }
    //m_dummymaster.Sentcustomtransaction(0x2000, dta, 32, tlm::TLM_WRITE_COMMAND);

   // sc_core::sc_start(10000, sc_core::SC_NS);

    // ram.dump_memory(0, 1024);
    
    sc_core::sc_start(5, sc_core::SC_NS);
    m_dummymaster.SentTransaction(0x5000 + DMASRCADDR(1), 0x0000, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    m_dummymaster.SentTransaction(0x5000 + DMADESADDR(1), 0x2000, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    m_dummymaster.SentTransaction(0x5000 + DMADATALENGTH(1), 32, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    m_dummymaster.SentTransaction(0x5000 + DMACHEN(0), 0xffffffff, tlm::TLM_WRITE_COMMAND);



    sc_core::sc_start(5, sc_core::SC_NS);
    m_dummymaster.SentTransaction(0x5000 + DMASRCADDR(2), 0x2000, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    m_dummymaster.SentTransaction(0x5000 + DMADESADDR(2), 0x0000, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    m_dummymaster.SentTransaction(0x5000 + DMADATALENGTH(2), 32*4, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);

    sc_core::sc_start(5, sc_core::SC_NS);
    m_dummymaster.SentTransaction(0x5000 + DMASRCADDR(3), 0x6000, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    m_dummymaster.SentTransaction(0x5000 + DMADESADDR(3), 0x2000 + 32, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    m_dummymaster.SentTransaction(0x5000 + DMADATALENGTH(3), 32, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);

    sc_core::sc_start(5, sc_core::SC_NS);
    m_dummymaster.SentTransaction(0x5000 + DMASRCADDR(4), 0x7000, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    m_dummymaster.SentTransaction(0x5000 + DMADESADDR(4), 0x2000 + 2*32, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    m_dummymaster.SentTransaction(0x5000 + DMADATALENGTH(4), 32, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);

    sc_core::sc_start(5, sc_core::SC_NS);
    m_dummymaster.SentTransaction(0x5000 + DMASRCADDR(5), 0x8000, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    m_dummymaster.SentTransaction(0x5000 + DMADESADDR(5), 0x2000 + 3*32, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    m_dummymaster.SentTransaction(0x5000 + DMADATALENGTH(5), 32, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);


    m_dummymaster.SentTransaction(0x5000 + DMAREQ(0), 0x3A, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(100, sc_core::SC_NS);
    ram.dump_memory(0x0000, 32 * 4);
   

    std::cout << "Simulation Time: " << sc_core::sc_time_stamp().to_default_time_units() << "SC_NS" << std::endl;
    
    return(0);
}


