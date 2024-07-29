#pragma once
/*
 *	Author: Huan Nguyen-Duy
 *  Date: 26/07/2024
 */

#include "top.h"
#include "Register.h"


inline void tp_02_02(SoCPlatform& SoC)
{
    /*
        Using to test priority handling and DMA operation that is triggered by register setting
    */
    unsigned int result = 0x0;

    std::cout << "Running test case:  tp_02_02" << std::endl;
    SoC.monitor_ports(true);


    SoC.SentTransaction(REG_DMACHEN(0), 0xffff, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);

    // Define DMA source address, destination address for each channel
    SoC.SentTransaction(REG_DMADESADDR(0), BASE_RAM1, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    SoC.SentTransaction(REG_DMASRCADDR(0), BASE_TARGET1, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    SoC.SentTransaction(REG_DMADATALENGTH(0), 32, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);


    SoC.SentTransaction(REG_DMADESADDR(1), BASE_TARGET1, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    SoC.SentTransaction(REG_DMASRCADDR(1), BASE_RAM1, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    SoC.SentTransaction(REG_DMADATALENGTH(1), 32, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);

    SoC.SentTransaction(REG_DMADESADDR(2), BASE_TARGET3, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    SoC.SentTransaction(REG_DMASRCADDR(2), BASE_RAM1, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    SoC.SentTransaction(REG_DMADATALENGTH(2), 32, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);

    SoC.SentTransaction(REG_DMADESADDR(3), BASE_TARGET4, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    SoC.SentTransaction(REG_DMASRCADDR(3), BASE_RAM1, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    SoC.SentTransaction(REG_DMADATALENGTH(3), 32, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);

    SoC.SentTransaction(REG_DMAREQ(0), 0x07, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(10, sc_core::SC_NS);
    SoC.dump_memory("ram1", 0x00, 64);
    sc_core::sc_start(5, sc_core::SC_NS);

    SoC.SentTransaction(REG_DMAACK(0), 0x01, tlm::TLM_READ_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    unsigned int value = 0;
    value = SoC.get_received_32bit_data();

    if (value != 0x07)
    {
        SoC.SentTransaction(REG_DUMMYRESULT, 0x00, tlm::TLM_WRITE_COMMAND);
        sc_core::sc_start(5, sc_core::SC_NS);
    }
    sc_core::sc_start(5, sc_core::SC_NS);
    SoC.SentTransaction(REG_DMAINT(0), 0x01, tlm::TLM_READ_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    value = SoC.get_received_32bit_data();
    if (value != 0x07)
    {
        SoC.SentTransaction(REG_DUMMYRESULT, 0x00, tlm::TLM_WRITE_COMMAND);
        sc_core::sc_start(5, sc_core::SC_NS);
    }
    sc_core::sc_start(5, sc_core::SC_NS);

    /*Set dummy result to pass*/
    SoC.SentTransaction(REG_DUMMYRESULT, PASS, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);

}