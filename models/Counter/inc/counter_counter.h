/******************************************************************
	counter_counter.h
	This file is generated from counter.v by V2SC
	(c) Copryight 2007 by Ali Haj Abutalebi & Leila Mahmoudi Ayough
	Mazdak and Alborz Design Automation
	email: info@mazdak-alborz.com
	website: www.mazdak-alborz.com
*******************************************************************/

#ifndef _COUNTER_COUNTER_H
#define _COUNTER_COUNTER_H

#include <systemc.h>

using namespace sc_core;
using namespace sc_dt;

SC_MODULE(counter) 
{
	//Interface Ports...
	sc_in<bool>	clock;
	sc_in<bool>	load;
	sc_in<bool>	clear;
	sc_in<bool> start;
	sc_in<sc_uint<8> >	din;
	sc_out<sc_uint<8> >	dout;

	//Internal Signals...
	sc_signal<sc_uint<8> >	countval;

	//Processes Declaration...
	void assign_process_dout_10();
	void always_process_12();

	//Constructor Declaration...
	SC_CTOR(counter)
	{
		SC_METHOD(assign_process_dout_10);
		sensitive << countval;

		SC_METHOD(always_process_12);
		sensitive << clock.pos();
	}
};

#endif
