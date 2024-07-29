/******************************************************************
	adder_four_bit_adder.h
	This file is generated from adder.v by V2SC
	(c) Copryight 2007 by Ali Haj Abutalebi & Leila Mahmoudi Ayough
	Mazdak and Alborz Design Automation
	email: info@mazdak-alborz.com
	website: www.mazdak-alborz.com
*******************************************************************/

#ifndef _ADDER_FOUR_BIT_ADDER_H
#define _ADDER_FOUR_BIT_ADDER_H

#include <systemc.h>
#include "full_adder_full_adder.h"

using namespace sc_core;

SC_MODULE(four_bit_adder) 
{
	//Interface Ports...
	sc_out<sc_uint<4> >	sum;
	sc_out<bool>	cout;
	sc_in<sc_uint<4> >	a;
	sc_in<sc_uint<4> >	b;
	sc_in<bool>	cin;

	//Internal Signals...
	sc_signal<bool>	s1;
	sc_signal<bool>	s2;
	sc_signal<bool>	s3;

	//Instantiated Modules Object Declaration...
	full_adder	m0;
	full_adder	m1;
	full_adder	m2;
	full_adder	m3;

	//Local Temporary Signals...
	sc_signal<bool> sum_0;
	sc_signal<bool> a_0;
	sc_signal<bool> b_0;
	sc_signal<bool> sum_1;
	sc_signal<bool> a_1;
	sc_signal<bool> b_1;
	sc_signal<bool> sum_2;
	sc_signal<bool> a_2;
	sc_signal<bool> b_2;
	sc_signal<bool> sum_3;
	sc_signal<bool> a_3;
	sc_signal<bool> b_3;

	//Signal Handler...
	void signal_handler() 
	{
		sc_uint<4> sum_tmp(sum.read());
		sum_tmp[0] = sum_0.read();
		sum = sum_tmp;
		a_0 = a.read()[0];
		b_0 = b.read()[0];
		sum_tmp[1] = sum_1.read();
		sum = sum_tmp;
		a_1 = a.read()[1];
		b_1 = b.read()[1];
		sum_tmp[2] = sum_2.read();
		sum = sum_tmp;
		a_2 = a.read()[2];
		b_2 = b.read()[2];
		sum_tmp[3] = sum_3.read();
		sum = sum_tmp;
		a_3 = a.read()[3];
		b_3 = b.read()[3];
	}

	//Constructor Declaration...
	SC_CTOR(four_bit_adder) :
		m0("M0") ,
		m1("M1") ,
		m2("M2") ,
		m3("M3")
	{
		m0(sum_0,s1,a_0,b_0,cin);

		m1(sum_1,s2,a_1,b_1,s1);

		m2(sum_2,s3,a_2,b_2,s2);

		m3(sum_3,cout,a_3,b_3,s3);

		SC_METHOD(signal_handler);
		sensitive << sum_0 << a << b << sum_1 << sum_2 << sum_3;
	}
};

#endif
