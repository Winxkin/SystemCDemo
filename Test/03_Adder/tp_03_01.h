#include "top.h"
#include "Register.h"


inline void tp_03_01(SoCPlatform& SoC)
{
    /*
        Using to test the arbitration of bus MMIO using address access
    */

    std::cout << "Running test case:  tp_03_01" << std::endl;

    unsigned int a = 0x01;
    unsigned int b = 0x02;
    unsigned int cin = 0x00;
    unsigned int mixreg =  (cin << 15) | (b << 8) | a;

    SoC.SentTransaction(REG_ADDERINPUT, mixreg, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);

    SoC.SentTransaction(REG_ADDEROUTPUT,0x0, tlm::TLM_READ_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
    unsigned int rdata = SoC.get_received_32bit_data(); 
    sc_core::sc_start(5, sc_core::SC_NS);
    if (rdata != 0x03)
    {
        SoC.SentTransaction(REG_DUMMYRESULT, FAIL, tlm::TLM_WRITE_COMMAND);
        sc_core::sc_start(5, sc_core::SC_NS);
    }

    a = 0x0f;
    b = 0x01;
   mixreg = (b << 8) | a;
    sc_core::sc_start(5, sc_core::SC_NS);

    SoC.SentTransaction(REG_ADDERINPUT, mixreg, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);

    SoC.SentTransaction(REG_ADDEROUTPUT, 0x0, tlm::TLM_READ_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);

    rdata = SoC.get_received_32bit_data();

    sc_core::sc_start(5, sc_core::SC_NS);
    if (rdata != 0x8000)
    {
        SoC.SentTransaction(REG_DUMMYRESULT, FAIL, tlm::TLM_WRITE_COMMAND);
        sc_core::sc_start(5, sc_core::SC_NS);
    }

    SoC.SentTransaction(REG_DUMMYRESULT, PASS, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(5, sc_core::SC_NS);
}

