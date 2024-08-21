/******************************************************************
	test_adder_test_adder.cpp
	This file is generated from test_adder.v by V2SC
	(c) Copryight 2007 by Ali Haj Abutalebi & Leila Mahmoudi Ayough
	Mazdak and Alborz Design Automation
	email: info@mazdak-alborz.com
	website: www.mazdak-alborz.com
*******************************************************************/

#include "test_adder_test_adder.h"

void test_adder::initial_process_9() 
{
	a = 0;
	b = 0;
	cin = 0;
	wait(5,SC_NS);
	a = 4;
	b = 8;
	cin = 0;
	wait(5,SC_NS);
	a = 6;
	b = 3;
	cin = 1;
	wait(5,SC_NS);
	a = 5;
	b = 12;
	cin = 0;
	wait(5,SC_NS);
	a = 8;
	b = 8;
	cin = 0;
	wait(5,SC_NS);
	a = 9;
	b = 1;
	cin = 1;
	wait(5,SC_NS);
	sc_stop();
}
