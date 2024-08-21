/******************************************************************
	cpu_interface_cpu_interface.h
	This file is generated from cpu_interface.v by V2SC
	(c) Copryight 2007 by Ali Haj Abutalebi & Leila Mahmoudi Ayough
	Mazdak and Alborz Design Automation
	email: info@mazdak-alborz.com
	website: www.mazdak-alborz.com
*******************************************************************/

#ifndef _CPU_INTERFACE_CPU_INTERFACE_H
#define _CPU_INTERFACE_CPU_INTERFACE_H

#include "systemc.h"
#include "control_operation_control_operation.h"

SC_MODULE(cpu_interface) 
{
	//Interface Ports...
	sc_in<bool>	clk;
	sc_in<bool>	clk_div_en;
	sc_in<bool>	clr_int_en;
	sc_in<bool>	cs;
	sc_in<sc_uint<8> >	div_data;
	sc_in<bool>	nrw;
	sc_in<bool>	rst;
	sc_in<sc_uint<8> >	ser_if_data;
	sc_in<bool>	xmitdt_en;
	sc_out<bool>	clear_flags;
	sc_out<sc_uint<8> >	data_out;
	sc_out<bool>	enable_write;
	sc_out<bool>	start_xmit;

	//Processes Declaration...
	void assign_process_data_out_82();

	//Instantiated Modules Object Declaration...
	control_operation<>	I0;

	//Constructor Declaration...
	SC_CTOR(cpu_interface) :
		I0("I0")
	{
		I0.clk(clk);
		I0.clr_int_en(clr_int_en);
		I0.cs(cs);
		I0.nrw(nrw);
		I0.rst(rst);
		I0.xmitdt_en(xmitdt_en);
		I0.clear_flags(clear_flags);
		I0.enable_write(enable_write);
		I0.start_xmit(start_xmit);

		SC_METHOD(assign_process_data_out_82);
		sensitive << clk_div_en << div_data << ser_if_data;
	}
};

#endif
