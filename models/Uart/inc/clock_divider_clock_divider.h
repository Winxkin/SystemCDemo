/******************************************************************
	clock_divider_clock_divider.h
	This file is generated from clock_divider.v by V2SC
	(c) Copryight 2007 by Ali Haj Abutalebi & Leila Mahmoudi Ayough
	Mazdak and Alborz Design Automation
	email: info@mazdak-alborz.com
	website: www.mazdak-alborz.com
*******************************************************************/

#ifndef _CLOCK_DIVIDER_CLOCK_DIVIDER_H
#define _CLOCK_DIVIDER_CLOCK_DIVIDER_H

#include "systemc.h"

using namespace sc_core;
using namespace sc_dt;

SC_MODULE(clock_divider) 
{
	//Interface Ports...
	sc_in<bool>	addr;
	sc_in<bool>	clk;
	sc_in<bool>	clk_div_en;
	sc_in<sc_uint<8> >	data_in;
	sc_in<bool>	enable_rcv_clk;
	sc_in<bool>	enable_write;
	sc_in<bool>	enable_xmit_clk;
	sc_in<bool>	rst;
	sc_out<sc_uint<8> >	div_data;
	sc_out<bool>	sample;

	//Internal Signals...
	sc_signal<sc_uint<16> >	div_msb_lsb;
	sc_signal<sc_uint<16> >	clk_cnt;

	//Processes Declaration...
	void always_process_57();
	void always_process_95();

	//Constructor Declaration...
	SC_CTOR(clock_divider)
	{
		SC_METHOD(always_process_57);
		sensitive << clk.pos() << rst.neg();

		SC_METHOD(always_process_95);
		sensitive << addr << div_msb_lsb << clk_cnt;
	}
};

#endif
