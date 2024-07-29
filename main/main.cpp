// All systemc modules should include systemc.h header file
#include "TestList.h"

// sc_main in top level function like in C++ main


int sc_main(int argc, char* argv[]) {

    SoCPlatform m_SoCPlatform("SoCPlatform");

    // Running test case here
    tp_04_01(m_SoCPlatform);

    return 0;
}


