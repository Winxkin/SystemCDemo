/******************************************************************
	serial_interface_serial_interface.h
	This file is generated from serial_interface.v by V2SC
	(c) Copryight 2007 by Ali Haj Abutalebi & Leila Mahmoudi Ayough
	Mazdak and Alborz Design Automation
	email: info@mazdak-alborz.com
	website: www.mazdak-alborz.com
*******************************************************************/

#ifndef _SERIAL_INTERFACE_SERIAL_INTERFACE_H
#define _SERIAL_INTERFACE_SERIAL_INTERFACE_H

#include "systemc.h"
#include "status_registers_status_registers.h"
#include "xmit_rcv_control_xmit_rcv_control.h"

SC_MODULE(serial_interface) 
{
	//Interface Ports...
	sc_in<bool>	clear_flags;
	sc_in<bool>	clk;
	sc_in<sc_uint<8> >	data_in;
	sc_in<bool>	enable_write;
	sc_in<bool>	rst;
	sc_in<bool>	sample;
	sc_in<sc_uint<2> >	ser_if_select;
	sc_in<bool>	sin;
	sc_in<bool>	start_xmit;
	sc_in<bool>	xmitdt_en;
	sc_out<bool>	enable_rcv_clk;
	sc_out<bool>	enable_xmit_clk;
	sc_out<bool>	int2;
	sc_out<sc_uint<8> >	ser_if_data;
	sc_out<bool>	sout;

	//Internal Signals...
	sc_signal<bool>	done_rcving;
	sc_signal<bool>	done_xmitting;
	sc_signal<sc_uint<3> >	rcv_bit_cnt;
	sc_signal<bool>	rcving;
	sc_signal<bool>	read_bit;
	sc_signal<sc_uint<8> >	recvdt;
	sc_signal<sc_uint<8> >	status;
	sc_signal<sc_uint<8> >	xmitdt;
	sc_signal<bool>	xmitting;
	sc_signal<sc_uint<8> >	zeros;
	sc_signal<sc_uint<8> >	MW_ser_out_muxdin0l;
	sc_signal<sc_uint<8> >	MW_ser_out_muxdin1l;
	sc_signal<sc_uint<8> >	MW_ser_out_muxdin2l;
	sc_signal<sc_uint<8> >	MW_ser_out_muxdin3l;
	sc_signal<sc_uint<8> >	MW_ser_out_muxdtempl;
	sc_signal<sc_uint<8> >	xmitdt_reg;
	sc_signal<sc_uint<8> >	recvdt_reg;

	//Processes Declaration...
	void always_process_129();
	void assign_process_xmitdt_150();
	void assign_process_recvdt_151();
	void always_process_155();
	void assign_process_ser_if_data_163();
	void assign_process_MW_ser_out_muxdin0l_164();
	void assign_process_MW_ser_out_muxdin1l_165();
	void assign_process_MW_ser_out_muxdin2l_166();
	void assign_process_MW_ser_out_muxdin3l_167();
	void assign_process_zeros_171();

	//Instantiated Modules Object Declaration...
	status_registers	I1;
	xmit_rcv_control<>	I0;

	//Constructor Declaration...
	SC_CTOR(serial_interface) :
		I1("I1") ,
		I0("I0")
	{
		I1.clear_flags(clear_flags);
		I1.clk(clk);
		I1.done_rcving(done_rcving);
		I1.done_xmitting(done_xmitting);
		I1.rcving(rcving);
		I1.rst(rst);
		I1.xmitting(xmitting);
		I1.int2(int2);
		I1.status(status);

		I0.clk(clk);
		I0.rst(rst);
		I0.sample(sample);
		I0.sin(sin);
		I0.start_xmit(start_xmit);
		I0.xmitdt(xmitdt);
		I0.done_rcving(done_rcving);
		I0.done_xmitting(done_xmitting);
		I0.enable_rcv_clk(enable_rcv_clk);
		I0.enable_xmit_clk(enable_xmit_clk);
		I0.rcv_bit_cnt(rcv_bit_cnt);
		I0.rcving(rcving);
		I0.read_bit(read_bit);
		I0.sout(sout);
		I0.xmitting(xmitting);

		SC_METHOD(always_process_129);
		sensitive << clk.pos() << rst.neg();

		SC_METHOD(assign_process_xmitdt_150);
		sensitive << xmitdt_reg;

		SC_METHOD(assign_process_recvdt_151);
		sensitive << recvdt_reg;

		SC_METHOD(always_process_155);
		sensitive << MW_ser_out_muxdin0l << MW_ser_out_muxdin1l << MW_ser_out_muxdin2l << MW_ser_out_muxdin3l << ser_if_select;

		SC_METHOD(assign_process_ser_if_data_163);
		sensitive << MW_ser_out_muxdtempl;

		SC_METHOD(assign_process_MW_ser_out_muxdin0l_164);
		sensitive << xmitdt;

		SC_METHOD(assign_process_MW_ser_out_muxdin1l_165);
		sensitive << recvdt;

		SC_METHOD(assign_process_MW_ser_out_muxdin2l_166);
		sensitive << status;

		SC_METHOD(assign_process_MW_ser_out_muxdin3l_167);
		sensitive << zeros;

		SC_METHOD(assign_process_zeros_171);
	}
};

#endif
