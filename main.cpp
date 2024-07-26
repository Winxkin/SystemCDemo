// All systemc modules should include systemc.h header file
#include "top.h"

using namespace sc_core;
using namespace std;


// sc_main in top level function like in C++ main

int sc_main(int argc, char* argv[]) {

    SoCPlatform m_SoCPlatform("SoCPlatform");
    sc_core::sc_start(10, sc_core::SC_NS);

    sc_core::sc_start(5, sc_core::SC_NS);
    m_SoCPlatform.m_dummymaster.SentTransaction(0x3000 + DMASRCADDR(1), 0x4000, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    m_SoCPlatform.m_dummymaster.SentTransaction(0x3000 + DMADESADDR(1), 0x0000, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    m_SoCPlatform.m_dummymaster.SentTransaction(0x3000 + DMADATALENGTH(1), 32, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    m_SoCPlatform.m_dummymaster.SentTransaction(0x3000 + DMACHEN(0), 0xffffffff, tlm::TLM_WRITE_COMMAND);



    sc_core::sc_start(5, sc_core::SC_NS);
    m_SoCPlatform.m_dummymaster.SentTransaction(0x3000 + DMASRCADDR(2), 0x5000, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    m_SoCPlatform.m_dummymaster.SentTransaction(0x3000 + DMADESADDR(2), 0x0000, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    m_SoCPlatform.m_dummymaster.SentTransaction(0x3000 + DMADATALENGTH(2), 32 * 4, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);

    sc_core::sc_start(5, sc_core::SC_NS);
    m_SoCPlatform.m_dummymaster.SentTransaction(0x3000 + DMASRCADDR(10), 0x5000, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    m_SoCPlatform.m_dummymaster.SentTransaction(0x3000 + DMADESADDR(10), 0x0000 + 32, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    m_SoCPlatform.m_dummymaster.SentTransaction(0x3000 + DMADATALENGTH(10), 32, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);

    sc_core::sc_start(5, sc_core::SC_NS);
    m_SoCPlatform.m_dummymaster.SentTransaction(0x3000 + DMASRCADDR(11), 0x6000, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    m_SoCPlatform.m_dummymaster.SentTransaction(0x3000 + DMADESADDR(11), 0x0000 + 2 * 32, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    m_SoCPlatform.m_dummymaster.SentTransaction(0x3000 + DMADATALENGTH(11), 32, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);

    sc_core::sc_start(5, sc_core::SC_NS);
    m_SoCPlatform.m_dummymaster.SentTransaction(0x3000 + DMASRCADDR(20), 0x7000, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    m_SoCPlatform.m_dummymaster.SentTransaction(0x3000 + DMADESADDR(20), 0x0000 + 3 * 32, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    m_SoCPlatform.m_dummymaster.SentTransaction(0x3000 + DMADATALENGTH(20), 32, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);

    m_SoCPlatform.dma_req[1].write(true);
    //dma_req[2].write(true);
    m_SoCPlatform.dma_req[10].write(true);
    m_SoCPlatform.dma_req[11].write(true);
    m_SoCPlatform.dma_req[20].write(true);
    
    sc_core::sc_start(100, sc_core::SC_NS);
    m_SoCPlatform.m_ram.dump_memory(0x0000, 32 * 4);
    m_SoCPlatform.dma_req[1].write(false);
    //dma_req[2].write(true);
    m_SoCPlatform.dma_req[10].write(false);
    m_SoCPlatform.dma_req[11].write(false);
    m_SoCPlatform.dma_req[20].write(false);

    sc_core::sc_start(5, sc_core::SC_NS);
    m_SoCPlatform.m_dummymaster.SentTransaction(0x5000 + DMASRCADDR(10), 0x0000, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    m_SoCPlatform.m_dummymaster.SentTransaction(0x5000 + DMADESADDR(10), 0x4000 + 32, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    m_SoCPlatform.m_dummymaster.SentTransaction(0x5000 + DMADATALENGTH(10), 32, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);

    sc_core::sc_start(5, sc_core::SC_NS);
    m_SoCPlatform.m_dummymaster.SentTransaction(0x5000 + DMASRCADDR(11), 0x0000 + 2 * 32, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    m_SoCPlatform.m_dummymaster.SentTransaction(0x5000 + DMADESADDR(11), 0x5000, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    m_SoCPlatform.m_dummymaster.SentTransaction(0x5000 + DMADATALENGTH(11), 32, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);

    sc_core::sc_start(5, sc_core::SC_NS);
    m_SoCPlatform.m_dummymaster.SentTransaction(0x5000 + DMASRCADDR(20), 0x0000 + 3 * 32, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    m_SoCPlatform.m_dummymaster.SentTransaction(0x5000 + DMADESADDR(20), 0x6000, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    m_SoCPlatform.m_dummymaster.SentTransaction(0x5000 + DMADATALENGTH(20), 32, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    m_SoCPlatform.m_dummymaster.SentTransaction(0x5000 + DMASRCADDR(1), 0x0000, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    m_SoCPlatform.m_dummymaster.SentTransaction(0x5000 + DMADESADDR(1), 0x7000, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    m_SoCPlatform.m_dummymaster.SentTransaction(0x5000 + DMADATALENGTH(1), 32, tlm::TLM_WRITE_COMMAND);

    
    
    return 0;
}


