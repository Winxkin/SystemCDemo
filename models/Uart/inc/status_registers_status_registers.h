/******************************************************************
	status_registers_status_registers.h
	This file is generated from status_registers.v by V2SC
	(c) Copryight 2007 by Ali Haj Abutalebi & Leila Mahmoudi Ayough
	Mazdak and Alborz Design Automation
	email: info@mazdak-alborz.com
	website: www.mazdak-alborz.com
*******************************************************************/

#ifndef _STATUS_REGISTERS_STATUS_REGISTERS_H
#define _STATUS_REGISTERS_STATUS_REGISTERS_H

#include "systemc.h"

using namespace sc_core;
using namespace sc_dt;

SC_MODULE(status_registers) 
{
	//Interface Ports...
	sc_in<bool>	clear_flags;
	sc_in<bool>	clk;
	sc_in<bool>	done_rcving;
	sc_in<bool>	done_xmitting;
	sc_in<bool>	rcving;
	sc_in<bool>	rst;
	sc_in<bool>	xmitting;
	sc_out<bool>	int2;
	sc_out<sc_uint<8> >	status;

	//Internal Signals...
	sc_signal<bool>	xmitting_reg;
	sc_signal<bool>	done_xmitting_reg;
	sc_signal<bool>	rcving_reg;
	sc_signal<bool>	done_rcving_reg;

	//Processes Declaration...
	void always_process_55();
	void assign_process_int2_82();
	void assign_process_complex_status_84();

	//Constructor Declaration...
	SC_CTOR(status_registers)
	{
		SC_METHOD(always_process_55);
		sensitive << clk.pos() << rst.neg();

		SC_METHOD(assign_process_int2_82);
		sensitive << done_xmitting_reg << done_rcving_reg;

		SC_METHOD(assign_process_complex_status_84);
		sensitive << done_rcving_reg << done_xmitting_reg << rcving_reg << xmitting_reg;
	}
};

#endif
