/******************************************************************
	full_adder_full_adder.h
	This file is generated from full_adder.v by V2SC
	(c) Copryight 2007 by Ali Haj Abutalebi & Leila Mahmoudi Ayough
	Mazdak and Alborz Design Automation
	email: info@mazdak-alborz.com
	website: www.mazdak-alborz.com
*******************************************************************/

#ifndef _FULL_ADDER_FULL_ADDER_H
#define _FULL_ADDER_FULL_ADDER_H

#include <systemc>

using namespace sc_core;

SC_MODULE(full_adder) 
{
	//Interface Ports...
	sc_out<bool>	sum;
	sc_out<bool>	cout;
	sc_in<bool>	a;
	sc_in<bool>	b;
	sc_in<bool>	cin;

	//Processes Declaration...
	void assign_process_cout_sum_7();

	//Constructor Declaration...
	SC_CTOR(full_adder)
	{
		SC_METHOD(assign_process_cout_sum_7);
		sensitive << a << b << cin;
	}
};

#endif
