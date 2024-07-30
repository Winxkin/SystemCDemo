/******************************************************************
	address_decode_address_decode.h
	This file is generated from address_decode.v by V2SC
	(c) Copryight 2007 by Ali Haj Abutalebi & Leila Mahmoudi Ayough
	Mazdak and Alborz Design Automation
	email: info@mazdak-alborz.com
	website: www.mazdak-alborz.com
*******************************************************************/

#ifndef _ADDRESS_DECODE_ADDRESS_DECODE_H
#define _ADDRESS_DECODE_ADDRESS_DECODE_H

#include "systemc.h"

using namespace sc_core;
using namespace sc_dt;

SC_MODULE(address_decode) 
{
	//Interface Ports...
	sc_in<sc_uint<3> >	addr;
	sc_in<bool>	clk;
	sc_in<bool>	rst;
	sc_out<bool>	clk_div_en;
	sc_out<bool>	clr_int_en;
	sc_out<sc_uint<2> >	ser_if_select;
	sc_out<bool>	xmitdt_en;

	//Processes Declaration...
	void always_process_47();

	//Constructor Declaration...
	SC_CTOR(address_decode)
	{
		SC_METHOD(always_process_47);
		sensitive << addr;
	}
};

#endif
