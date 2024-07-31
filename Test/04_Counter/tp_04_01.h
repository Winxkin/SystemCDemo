#include "top.h"
#include "Register.h"


inline void tp_04_01()
{
    /*
        Using to test Counter module
    */
    SoCPlatform SoC("SoCPlatform");
    std::cout << "Running test case:  tp_04_01" << std::endl;
    sc_core::sc_start(20, sc_core::SC_NS);
    SoC.set_output_ports("counter_load", true);
    sc_core::sc_start(50, sc_core::SC_NS);
    //SoC.SentTransaction(REG_COUNTERINPUT, 0x00, tlm::TLM_WRITE_COMMAND);
    SoC.set_output_ports("counter_load", true);
    sc_core::sc_start(5, sc_core::SC_NS);
    SoC.set_output_ports("counter_load", false);
    SoC.set_output_ports("counter_start", true);
    sc_core::sc_start(200, sc_core::SC_NS);
    SoC.set_output_ports("counter_start", false);
    sc_core::sc_start(50, sc_core::SC_NS);
    SoC.SentTransaction(REG_COUNTEROUTPUT, 0x0, tlm::TLM_READ_COMMAND);
    sc_core::sc_start(100, sc_core::SC_NS);
    unsigned int rdata = SoC.get_received_32bit_data();
    
    if (rdata != 20)
    {
        SoC.SentTransaction(REG_DUMMYRESULT, FAIL, tlm::TLM_WRITE_COMMAND);
        sc_core::sc_start(100, sc_core::SC_NS);
    }

    SoC.SentTransaction(REG_DUMMYRESULT, PASS, tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(100, sc_core::SC_NS);
    
}

