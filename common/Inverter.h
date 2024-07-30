#ifndef _INVERTER_H
#define _INVERTER_H
#include <systemc.h>

SC_MODULE(Inverter) {
    sc_core::sc_in<bool> in;    // Input signal
    sc_core::sc_out<bool> out;  // Output signal (inverted)

    void do_invert() {
        out.write(!in.read());  // Invert the input signal
    }

    SC_CTOR(Inverter) {
        SC_METHOD(do_invert);   // Register the method with the SystemC kernel
        sensitive << in;        // Sensitize to changes in the input signal
    }
};

#endif