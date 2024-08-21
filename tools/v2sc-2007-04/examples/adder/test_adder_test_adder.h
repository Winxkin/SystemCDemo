/******************************************************************
	test_adder_test_adder.h
	This file is generated from test_adder.v by V2SC
	(c) Copryight 2007 by Ali Haj Abutalebi & Leila Mahmoudi Ayough
	Mazdak and Alborz Design Automation
	email: info@mazdak-alborz.com
	website: www.mazdak-alborz.com
*******************************************************************/

#ifndef _TEST_ADDER_TEST_ADDER_H
#define _TEST_ADDER_TEST_ADDER_H

#include "systemc.h"
#include "adder_four_bit_adder.h"

SC_MODULE(test_adder) 
{
	//Internal Signals...
	sc_signal<sc_uint<4> >	a;
	sc_signal<sc_uint<4> >	b;
	sc_signal<bool>	cin;
	sc_signal<sc_uint<4> >	sum;
	sc_signal<bool>	cout;

	//Processes Declaration...
	void initial_process_9();

	//Instantiated Modules Object Declaration...
	four_bit_adder	top;

	//Constructor Declaration...
	SC_CTOR(test_adder) :
		top("TOP")
	{
		top(sum,cout,a,b,cin);

		SC_THREAD(initial_process_9);
	}
};

#endif
