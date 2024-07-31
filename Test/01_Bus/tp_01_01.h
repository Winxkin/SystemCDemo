#include "top.h"
#include "Register.h"


inline void tp_01_01()
{
    /*
        Using to test the arbitration of bus MMIO using address access
    */

    SoCPlatform SoC("SoCPlatform");

    sc_core::sc_start(20, sc_core::SC_NS);
    std::cout << "Running test case:  tp_01_01" << std::endl;

    SoC.SentTransaction(BASE_TARGET1, 0xffff, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(2, sc_core::SC_NS);
    // Checking for requesting transaction in bus MMIO when other transaction is process
    SoC.SentTransaction(BASE_TARGET2, 0xffff, tlm::TLM_WRITE_COMMAND);  // this transaction is ignore
    sc_core::sc_start(100, sc_core::SC_NS);
    // In the case two transaction are request at the same time
    SoC.SentTransaction(BASE_TARGET3, 0xffff, tlm::TLM_WRITE_COMMAND);
    SoC.SentTransaction(BASE_TARGET4, 0xaaaa, tlm::TLM_WRITE_COMMAND);  // this transaction is ignore
    sc_core::sc_start(100, sc_core::SC_NS);

    SoC.SentTransaction(BASE_TARGET4, 0xaaaa, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(100, sc_core::SC_NS);
    SoC.SentTransaction(BASE_TARGET4, 0x0, tlm::TLM_READ_COMMAND); // To check transaction request with read command
    sc_core::sc_start(100, sc_core::SC_NS);
    unsigned int rdata = SoC.get_received_32bit_data(); // get [0x01] [0x02] [0x03] [0x04] 
    if (rdata != 0x03020100)
    {
        SoC.SentTransaction(REG_DUMMYRESULT, FAIL, tlm::TLM_WRITE_COMMAND);
        sc_core::sc_start(100, sc_core::SC_NS);
    }
    sc_core::sc_start(100, sc_core::SC_NS);
    SoC.SentTransaction(REG_DUMMYRESULT, PASS, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(100, sc_core::SC_NS);
}

