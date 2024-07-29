#include "top.h"
#include "Register.h"


inline void tp_04_01(SoCPlatform& SoC)
{
    /*
        Using to test the arbitration of bus MMIO using address access
    */

    std::cout << "Running test case:  tp_04_01" << std::endl;
    SoC.set_output_ports("counter_load", true);
    sc_core::sc_start(5, sc_core::SC_NS);
    //SoC.SentTransaction(REG_COUNTERINPUT, 0x00, tlm::TLM_WRITE_COMMAND);
    SoC.set_output_ports("counter_load", true);
    sc_core::sc_start(5, sc_core::SC_PS);
    SoC.set_output_ports("counter_load", false);
    sc_core::sc_start(100, sc_core::SC_PS);
    //SoC.SentTransaction(REG_COUNTEROUTPUT, 0x0, tlm::TLM_READ_COMMAND);
    //sc_core::sc_start(5, sc_core::SC_NS);
    //unsigned int rdata = SoC.get_received_32bit_data();
    //sc_core::sc_start(5, sc_core::SC_NS);
    
}

