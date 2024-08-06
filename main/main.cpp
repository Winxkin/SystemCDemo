// All systemc modules should include systemc.h header file
/*
#include "TestList.h"

// sc_main in top level function like in C++ main


int sc_main(int argc, char* argv[]) {

    // Running test case here
    tp_05_01();
    return 0;
}
*/

#include <systemc>
#include <iostream>
#include "RISCV32\commondef.h"
#include "TestList.h"

using namespace riscv32;

int sc_main(int argc, char* argv[]) {
    

    tp06_01_u_type_test();
    return 0;
}



