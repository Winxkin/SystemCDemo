/******************************************************************
	uart_top_uart_top.h
	This file is generated from uart_top.v by V2SC
	(c) Copryight 2007 by Ali Haj Abutalebi & Leila Mahmoudi Ayough
	Mazdak and Alborz Design Automation
	email: info@mazdak-alborz.com
	website: www.mazdak-alborz.com
*******************************************************************/

#ifndef _UART_TOP_UART_TOP_H
#define _UART_TOP_UART_TOP_H

#include "systemc.h"
#include "address_decode_address_decode.h"
#include "clock_divider_clock_divider.h"
#include "cpu_interface_cpu_interface.h"
#include "serial_interface_serial_interface.h"

using namespace sc_core;
using namespace sc_dt;

SC_MODULE(uart_top) 
{
	//Interface Ports...
	sc_in<sc_uint<3> >	addr;
	sc_in<bool>	clk;
	sc_in<bool>	cs;
	sc_in<sc_uint<8> >	data_in;
	sc_in<bool>	nrw;
	sc_in<bool>	rst;
	sc_in<bool>	sin;
	sc_out<sc_uint<8> >	data_out;
	sc_out<bool>	int2;
	sc_out<bool>	sout;

	//Internal Signals...
	sc_signal<bool>	clear_flags;
	sc_signal<bool>	clk_div_en;
	sc_signal<bool>	clr_int_en;
	sc_signal<sc_uint<8> >	div_data;
	sc_signal<bool>	enable_rcv_clk;
	sc_signal<bool>	enable_write;
	sc_signal<bool>	enable_xmit_clk;
	sc_signal<bool>	sample;
	sc_signal<sc_uint<8> >	ser_if_data;
	sc_signal<sc_uint<2> >	ser_if_select;
	sc_signal<bool>	start_xmit;
	sc_signal<bool>	xmitdt_en;

	//Instantiated Modules Object Declaration...
	address_decode	I3;
	clock_divider	I2;
	cpu_interface	I1;
	serial_interface	I4;

	//Local Temporary Signals...
	sc_signal<bool> addr_0;

	//Signal Handler...
	void signal_handler() 
	{
		addr_0 = addr.read()[0];
	}

	//Constructor Declaration...
	SC_CTOR(uart_top) :
		I3("I3") ,
		I2("I2") ,
		I1("I1") ,
		I4("I4")
	{
		I3.addr(addr);
		I3.clk(clk);
		I3.rst(rst);
		I3.clk_div_en(clk_div_en);
		I3.clr_int_en(clr_int_en);
		I3.ser_if_select(ser_if_select);
		I3.xmitdt_en(xmitdt_en);

		I2.addr(addr_0);
		I2.clk(clk);
		I2.clk_div_en(clk_div_en);
		I2.data_in(data_in);
		I2.enable_rcv_clk(enable_rcv_clk);
		I2.enable_write(enable_write);
		I2.enable_xmit_clk(enable_xmit_clk);
		I2.rst(rst);
		I2.div_data(div_data);
		I2.sample(sample);

		I1.clk(clk);
		I1.clk_div_en(clk_div_en);
		I1.clr_int_en(clr_int_en);
		I1.cs(cs);
		I1.div_data(div_data);
		I1.nrw(nrw);
		I1.rst(rst);
		I1.ser_if_data(ser_if_data);
		I1.xmitdt_en(xmitdt_en);
		I1.clear_flags(clear_flags);
		I1.data_out(data_out);
		I1.enable_write(enable_write);
		I1.start_xmit(start_xmit);

		I4.clear_flags(clear_flags);
		I4.clk(clk);
		I4.data_in(data_in);
		I4.enable_write(enable_write);
		I4.rst(rst);
		I4.sample(sample);
		I4.ser_if_select(ser_if_select);
		I4.sin(sin);
		I4.start_xmit(start_xmit);
		I4.xmitdt_en(xmitdt_en);
		I4.enable_rcv_clk(enable_rcv_clk);
		I4.enable_xmit_clk(enable_xmit_clk);
		I4.int2(int2);
		I4.ser_if_data(ser_if_data);
		I4.sout(sout);

		SC_METHOD(signal_handler);
		sensitive << addr;
	}
};

#endif
