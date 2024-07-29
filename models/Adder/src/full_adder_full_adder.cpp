/******************************************************************
	full_adder_full_adder.cpp
	This file is generated from full_adder.v by V2SC
	(c) Copryight 2007 by Ali Haj Abutalebi & Leila Mahmoudi Ayough
	Mazdak and Alborz Design Automation
	email: info@mazdak-alborz.com
	website: www.mazdak-alborz.com
*******************************************************************/

#include "Adder/inc/full_adder_full_adder.h"
#include <systemc>
using namespace sc_dt;

void full_adder::assign_process_cout_sum_7() 
{
	sc_dt::sc_uint<2> coutsum_concat;
	coutsum_concat = a.read() + b.read() + cin.read();
	cout = coutsum_concat[1];
	sum = coutsum_concat[0];
}
