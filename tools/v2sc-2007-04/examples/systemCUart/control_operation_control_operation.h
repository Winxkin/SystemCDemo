/******************************************************************
	control_operation_control_operation.h
	This file is generated from control_operation.v by V2SC
	(c) Copryight 2007 by Ali Haj Abutalebi & Leila Mahmoudi Ayough
	Mazdak and Alborz Design Automation
	email: info@mazdak-alborz.com
	website: www.mazdak-alborz.com
*******************************************************************/

#ifndef _CONTROL_OPERATION_CONTROL_OPERATION_H
#define _CONTROL_OPERATION_CONTROL_OPERATION_H

#include "systemc.h"

template<unsigned waiting = 0, unsigned reading_from_reg = 1, unsigned clearing_flags = 2, unsigned writing_to_reg = 3, unsigned xmitting = 4>
SC_MODULE(control_operation) 
{
	//Interface Ports...
	sc_in<bool>	clk;
	sc_in<bool>	clr_int_en;
	sc_in<bool>	cs;
	sc_in<bool>	nrw;
	sc_in<bool>	rst;
	sc_in<bool>	xmitdt_en;
	sc_out<bool>	clear_flags;
	sc_out<bool>	enable_write;
	sc_out<bool>	start_xmit;

	//Internal Signals...
	sc_signal<sc_uint<3> >	current_state;
	sc_uint<3> 	next_state;

	//Processes Declaration...
	void always_process_63();
	void always_process_131();

	//Constructor Declaration...
	SC_CTOR(control_operation)
	{
		SC_METHOD(always_process_63);
		sensitive << clr_int_en << cs << current_state << nrw << xmitdt_en;

		SC_METHOD(always_process_131);
		sensitive << clk.pos() << rst.neg();
	}
};

template<unsigned waiting, unsigned reading_from_reg, unsigned clearing_flags, unsigned writing_to_reg, unsigned xmitting>
void control_operation<waiting, reading_from_reg, clearing_flags, writing_to_reg, xmitting>::always_process_63() 
{
	clear_flags = 0;
	enable_write = 0;
	start_xmit = 0;
	switch( current_state.read() )
	{
		case clearing_flags : 
			clear_flags = 1;
			break;
		case writing_to_reg : 
			enable_write = 1;
			break;
		case xmitting : 
			start_xmit = 1;
			break;
	}
	switch( current_state.read() )
	{
		case waiting : 
			if( nrw.read() && !(cs.read()) ) 
			{
				next_state = writing_to_reg;
			}
			else
			{
				if( !(nrw.read()) && !(cs.read()) ) 
				{
					next_state = reading_from_reg;
				}
				else
				{
					next_state = waiting;
				}
			}
			break;
		case reading_from_reg : 
			if( cs.read() ) 
			{
				next_state = waiting;
			}
			else
			{
				if( !(nrw.read()) && clr_int_en.read() ) 
				{
					next_state = clearing_flags;
				}
				else
				{
					next_state = reading_from_reg;
				}
			}
			break;
		case clearing_flags : 
			if( cs.read() ) 
			{
				next_state = waiting;
			}
			else
			{
				next_state = clearing_flags;
			}
			break;
		case writing_to_reg : 
			if( cs.read() ) 
			{
				next_state = waiting;
			}
			else
			{
				if( nrw.read() && xmitdt_en.read() ) 
				{
					next_state = xmitting;
				}
				else
				{
					next_state = writing_to_reg;
				}
			}
			break;
		case xmitting : 
			if( cs.read() ) 
			{
				next_state = waiting;
			}
			else
			{
				next_state = xmitting;
			}
			break;
		default :
			next_state = waiting;
			break;
	}
}

template<unsigned waiting, unsigned reading_from_reg, unsigned clearing_flags, unsigned writing_to_reg, unsigned xmitting>
void control_operation<waiting, reading_from_reg, clearing_flags, writing_to_reg, xmitting>::always_process_131() 
{
	if( !(rst.read()) ) 
	{
		current_state = waiting;
	}
	else
	{
		current_state = next_state;
	}
}

#endif
