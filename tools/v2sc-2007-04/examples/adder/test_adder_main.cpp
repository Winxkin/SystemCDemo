/******************************************************************
	test_adder_main.cpp
	This file is generated from test_adder.v by V2SC
	(c) Copryight 2007 by Ali Haj Abutalebi & Leila Mahmoudi Ayough
	Mazdak and Alborz Design Automation
	email: info@mazdak-alborz.com
	website: www.mazdak-alborz.com
*******************************************************************/

#include "systemc.h"
#include "test_adder_test_adder.h"

int sc_main(int ac, char *av[])
{
	//Instantiated Top Module...
	test_adder	dut("dut");

	//VCD File...
	sc_trace_file *tf = sc_create_vcd_trace_file("test_adder");
	sc_trace(tf, dut.a, "a");
	sc_trace(tf, dut.b, "b");
	sc_trace(tf, dut.cin, "cin");
	sc_trace(tf, dut.sum, "sum");
	sc_trace(tf, dut.cout, "cout");

	//Start Simulation...
	sc_start(-1);

	sc_close_vcd_trace_file(tf);

	return 0;
}
