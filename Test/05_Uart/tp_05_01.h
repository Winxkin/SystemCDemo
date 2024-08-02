#include "top.h"
#include "Register.h"


inline void tp_05_01()
{
    SoCPlatform SoC("SoCPlatform");
    std::cout << "Running test case:  tp_05_01" << std::endl;

    sc_core::sc_start(20, sc_core::SC_NS);
    SoC.SentTransaction(REG_UARTINPUT, 0xff , tlm::TLM_WRITE_COMMAND);
    sc_core::sc_start(100, sc_core::SC_NS);
    SoC.set_output_ports("uart_cs", true);
    SoC.set_output_ports("uart_sin", true);
    SoC.set_output_ports("uart_nrw", true);
    sc_core::sc_start(15, sc_core::SC_NS);

}
