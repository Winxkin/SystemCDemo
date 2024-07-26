// All systemc modules should include systemc.h header file
#include "test/02_Dmac/tp_02_01.h"

// sc_main in top level function like in C++ main


int sc_main(int argc, char* argv[]) {

    SoCPlatform m_SoCPlatform("SoCPlatform");
    sc_core::sc_start(10, sc_core::SC_NS);

    // Running test case here
    tp_02_01_test01(m_SoCPlatform);

    return 0;
}


