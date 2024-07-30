#pragma once
/*
 *	Author: Huan Nguyen-Duy
 *  Date: 26/07/2024
 */

#include "top.h"
#include "Register.h"

inline void tp_02_01(SoCPlatform &SoC)
{
    /*
        Using to test priority handling and DMA operation that is triggered by ports
    */
	unsigned int result = 0x0;

    std::cout << "Running test case:  tp_02_01" << std::endl;
    SoC.monitor_ports(true);
    sc_core::sc_start(20, sc_core::SC_NS);
    SoC.SentTransaction(BASE_TARGET4, 0xffff, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(100, sc_core::SC_NS);

    
    SoC.SentTransaction(REG_DMACHEN(0), 0xffff, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(100, sc_core::SC_NS);

    // Define DMA source address, destination address for each channel
    SoC.SentTransaction(REG_DMADESADDR(0), BASE_RAM1, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(100, sc_core::SC_NS);
    SoC.SentTransaction(REG_DMASRCADDR(0), BASE_TARGET1, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(100, sc_core::SC_NS);
    SoC.SentTransaction(REG_DMADATALENGTH(0), 32, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(100, sc_core::SC_NS);


    SoC.SentTransaction(REG_DMADESADDR(1), BASE_TARGET1, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(100, sc_core::SC_NS);
    SoC.SentTransaction(REG_DMASRCADDR(1), BASE_RAM1, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(100, sc_core::SC_NS);
    SoC.SentTransaction(REG_DMADATALENGTH(1), 32, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(100, sc_core::SC_NS);

    SoC.SentTransaction(REG_DMADESADDR(2), BASE_TARGET3, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(100, sc_core::SC_NS);
    SoC.SentTransaction(REG_DMASRCADDR(2), BASE_RAM1, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(100, sc_core::SC_NS);
    SoC.SentTransaction(REG_DMADATALENGTH(2), 32, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(100, sc_core::SC_NS);

    SoC.SentTransaction(REG_DMADESADDR(3), BASE_TARGET4, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(100, sc_core::SC_NS);
    SoC.SentTransaction(REG_DMASRCADDR(3), BASE_RAM1, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(100, sc_core::SC_NS);
    SoC.SentTransaction(REG_DMADATALENGTH(3), 32, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(100, sc_core::SC_NS);
    
    SoC.set_output_ports("DMA_req0", true);
    SoC.set_output_ports("DMA_req1", true);
    sc_core::sc_start(10, sc_core::SC_NS);
    SoC.set_output_ports("DMA_req2", true);
    sc_core::sc_start(10, sc_core::SC_NS);
    SoC.set_output_ports("DMA_req3", true);
    sc_core::sc_start(5000, sc_core::SC_NS);
    SoC.dump_memory("ram1", 0x00, 64);
    sc_core::sc_start(100, sc_core::SC_NS);

    /*Set dummy result to pass*/
    SoC.SentTransaction(REG_DUMMYRESULT, PASS, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(100, sc_core::SC_NS);

}

