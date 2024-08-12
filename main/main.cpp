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
#include "RISCV32\decode_inst.h"
#include "TestList.h"
#include "core/RISCV32/riscv32.h"



int sc_main(int argc, char* argv[]) {
    

    riscv32 m_riscv32("riscv32");
    sc_core::sc_clock m_sysclk("clk", 10, sc_core::SC_NS);
    sc_core::sc_signal<bool> rst;
    m_riscv32.clk.bind(m_sysclk);
    m_riscv32.rst.bind(rst);
    sc_core::sc_start(2, sc_core::SC_NS);
    m_riscv32.start_cpu();
    sc_core::sc_start(100, sc_core::SC_SEC);
    return 0;
}



