#pragma once
/*
 *	Author: Huan Nguyen-Duy
 *  Date: 26/07/2024
 */

#include "../../SocPlatform/top.h"
#include "Register.h"

inline bool tp_02_01_test01(SoCPlatform &SoC)
{
	bool result = true;

    std::cout << "Running test case:  tp_02_01_test01" << std::endl;

    SoC.m_dummymaster.SentTransaction(REG_DMACHEN(0), 0xffff, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);

    // Define DMA source address, destination address for each channel
    SoC.m_dummymaster.SentTransaction(REG_DMADESADDR(0), BASE_TARGET1, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    SoC.m_dummymaster.SentTransaction(REG_DMASRCADDR(0), BASE_RAM1, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    SoC.m_dummymaster.SentTransaction(REG_DMADATALENGTH(0), 32, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);


    SoC.m_dummymaster.SentTransaction(REG_DMADESADDR(1), BASE_TARGET2, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    SoC.m_dummymaster.SentTransaction(REG_DMASRCADDR(1), BASE_RAM1, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    SoC.m_dummymaster.SentTransaction(REG_DMADATALENGTH(1), 32, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);

    SoC.m_dummymaster.SentTransaction(REG_DMADESADDR(2), BASE_TARGET3, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    SoC.m_dummymaster.SentTransaction(REG_DMASRCADDR(2), BASE_RAM1, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    SoC.m_dummymaster.SentTransaction(REG_DMADATALENGTH(2), 32, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);

    SoC.m_dummymaster.SentTransaction(REG_DMADESADDR(3), BASE_TARGET4, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    SoC.m_dummymaster.SentTransaction(REG_DMASRCADDR(3), BASE_RAM1, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    SoC.m_dummymaster.SentTransaction(REG_DMADATALENGTH(3), 32, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    
    
	
	return result;
}