/******************************************************************
	xmit_rcv_control_xmit_rcv_control.h
	This file is generated from xmit_rcv_control.v by V2SC
	(c) Copryight 2007 by Ali Haj Abutalebi & Leila Mahmoudi Ayough
	Mazdak and Alborz Design Automation
	email: info@mazdak-alborz.com
	website: www.mazdak-alborz.com
*******************************************************************/

#ifndef _XMIT_RCV_CONTROL_XMIT_RCV_CONTROL_H
#define _XMIT_RCV_CONTROL_XMIT_RCV_CONTROL_H

#include "systemc.h"

template<unsigned waiting = 0, unsigned check_lock = 1, unsigned rcv_locked = 2, unsigned read_data = 3, unsigned incr_count2 = 4, unsigned done_read = 5, unsigned read_stop_bit = 6, unsigned finish_rcv = 7, unsigned waiting_to_xmit = 0, unsigned send_start = 1, unsigned send_data = 2, unsigned incr_count = 3, unsigned done_xmit = 4, unsigned send_stop_bit = 5, unsigned finish_xmit = 6>
SC_MODULE(xmit_rcv_control) 
{
	//Interface Ports...
	sc_in<bool>	clk;
	sc_in<bool>	rst;
	sc_in<bool>	sample;
	sc_in<bool>	sin;
	sc_in<bool>	start_xmit;
	sc_in<sc_uint<8> >	xmitdt;
	sc_out<bool>	done_rcving;
	sc_out<bool>	done_xmitting;
	sc_out<bool>	enable_rcv_clk;
	sc_out<bool>	enable_xmit_clk;
	sc_out<sc_uint<3> >	rcv_bit_cnt;
	sc_out<bool>	rcving;
	sc_out<bool>	read_bit;
	sc_out<bool>	sout;
	sc_out<bool>	xmitting;

	//Internal Signals...
	sc_signal<bool>	enable_xmit_clk_cld;
	sc_signal<sc_uint<3> >	rcv_bit_cnt_cld;
	sc_signal<bool>	sout_cld;
	sc_signal<bool>	xmitting_cld;
	sc_signal<sc_uint<3> >	xmit_bit_cnt;
	sc_signal<sc_uint<3> >	Rcv_current_state;
	sc_signal<sc_uint<3> >	Rcv_next_state;
	sc_signal<sc_uint<3> >	Xmit_current_state;
	sc_signal<sc_uint<3> >	Xmit_next_state;

	//Processes Declaration...
	void always_process_104();
	void always_process_251();
	void always_process_301();
	void always_process_373();
	void always_process_392();
	void always_process_434();
	void always_process_486();

	//Constructor Declaration...
	SC_CTOR(xmit_rcv_control)
	{
		SC_METHOD(always_process_104);
		sensitive << Rcv_current_state << rcv_bit_cnt_cld << sample << sin;

		SC_METHOD(always_process_251);
		sensitive << Xmit_current_state << sample << start_xmit << xmit_bit_cnt;

		SC_METHOD(always_process_301);
		sensitive << Rcv_current_state << rcv_bit_cnt_cld << sample << sin;

		SC_METHOD(always_process_373);
		sensitive << Xmit_current_state;

		SC_METHOD(always_process_392);
		sensitive << clk.pos() << rst.neg();

		SC_METHOD(always_process_434);
		sensitive << clk.pos() << rst.neg();

		SC_METHOD(always_process_486);
		sensitive << enable_xmit_clk_cld << rcv_bit_cnt_cld << sout_cld << xmitting_cld;
	}
};

template<unsigned waiting, unsigned check_lock, unsigned rcv_locked, unsigned read_data, unsigned incr_count2, unsigned done_read, unsigned read_stop_bit, unsigned finish_rcv, unsigned waiting_to_xmit, unsigned send_start, unsigned send_data, unsigned incr_count, unsigned done_xmit, unsigned send_stop_bit, unsigned finish_xmit>
void xmit_rcv_control<waiting, check_lock, rcv_locked, read_data, incr_count2, done_read, read_stop_bit, finish_rcv, waiting_to_xmit, send_start, send_data, incr_count, done_xmit, send_stop_bit, finish_xmit>::always_process_104() 
{
	switch( Rcv_current_state.read() )
	{
		case waiting : 
			if( !(sin.read()) ) 
			{
				Rcv_next_state = check_lock;
			}
			else
			{
				Rcv_next_state = waiting;
			}
			break;
		case check_lock : 
			if( sin.read() ) 
			{
				Rcv_next_state = waiting;
			}
			else
			{
				if( !(sin.read()) ) 
				{
					Rcv_next_state = rcv_locked;
				}
				else
				{
					Rcv_next_state = check_lock;
				}
			}
			break;
		case rcv_locked : 
			if( sample.read() ) 
			{
				Rcv_next_state = read_data;
			}
			else
			{
				Rcv_next_state = rcv_locked;
			}
			break;
		case read_data : 
			if( !(sample.read()) ) 
			{
				Rcv_next_state = incr_count2;
			}
			else
			{
				Rcv_next_state = read_data;
			}
			break;
		case incr_count2 : 
			if( sample.read() && (rcv_bit_cnt_cld.read() != 7) ) 
			{
				Rcv_next_state = read_data;
			}
			else
			{
				if( sample.read() && (rcv_bit_cnt_cld.read() == 7) ) 
				{
					Rcv_next_state = done_read;
				}
				else
				{
					Rcv_next_state = incr_count2;
				}
			}
			break;
		case done_read : 
			if( !(sample.read()) ) 
			{
				Rcv_next_state = read_stop_bit;
			}
			else
			{
				Rcv_next_state = done_read;
			}
			break;
		case read_stop_bit : 
			if( sample.read() ) 
			{
				Rcv_next_state = finish_rcv;
			}
			else
			{
				Rcv_next_state = read_stop_bit;
			}
			break;
		case finish_rcv : 
			Rcv_next_state = waiting;
			break;
		default :
			Rcv_next_state = waiting;
			break;
	}
}

template<unsigned waiting, unsigned check_lock, unsigned rcv_locked, unsigned read_data, unsigned incr_count2, unsigned done_read, unsigned read_stop_bit, unsigned finish_rcv, unsigned waiting_to_xmit, unsigned send_start, unsigned send_data, unsigned incr_count, unsigned done_xmit, unsigned send_stop_bit, unsigned finish_xmit>
void xmit_rcv_control<waiting, check_lock, rcv_locked, read_data, incr_count2, done_read, read_stop_bit, finish_rcv, waiting_to_xmit, send_start, send_data, incr_count, done_xmit, send_stop_bit, finish_xmit>::always_process_251() 
{
	switch( Xmit_current_state.read() )
	{
		case waiting_to_xmit : 
			if( start_xmit.read() ) 
			{
				Xmit_next_state = send_start;
			}
			else
			{
				Xmit_next_state = waiting_to_xmit;
			}
			break;
		case send_start : 
			if( sample.read() ) 
			{
				Xmit_next_state = send_data;
			}
			else
			{
				Xmit_next_state = send_start;
			}
			break;
		case send_data : 
			if( !(sample.read()) ) 
			{
				Xmit_next_state = incr_count;
			}
			else
			{
				Xmit_next_state = send_data;
			}
			break;
		case incr_count : 
			if( sample.read() && (xmit_bit_cnt.read() != 0) ) 
			{
				Xmit_next_state = send_data;
			}
			else
			{
				if( sample.read() && ~(xmit_bit_cnt.read()) ) 
				{
					Xmit_next_state = done_xmit;
				}
				else
				{
					Xmit_next_state = incr_count;
				}
			}
			break;
		case done_xmit : 
			if( !(sample.read()) ) 
			{
				Xmit_next_state = send_stop_bit;
			}
			else
			{
				Xmit_next_state = done_xmit;
			}
			break;
		case send_stop_bit : 
			Xmit_next_state = finish_xmit;
			break;
		case finish_xmit : 
			Xmit_next_state = waiting_to_xmit;
			break;
		default :
			Xmit_next_state = waiting_to_xmit;
			break;
	}
}

template<unsigned waiting, unsigned check_lock, unsigned rcv_locked, unsigned read_data, unsigned incr_count2, unsigned done_read, unsigned read_stop_bit, unsigned finish_rcv, unsigned waiting_to_xmit, unsigned send_start, unsigned send_data, unsigned incr_count, unsigned done_xmit, unsigned send_stop_bit, unsigned finish_xmit>
void xmit_rcv_control<waiting, check_lock, rcv_locked, read_data, incr_count2, done_read, read_stop_bit, finish_rcv, waiting_to_xmit, send_start, send_data, incr_count, done_xmit, send_stop_bit, finish_xmit>::always_process_301() 
{
	done_rcving = 0;
	enable_rcv_clk = 0;
	rcving = 0;
	read_bit = 0;
	switch( Rcv_current_state.read() )
	{
		case check_lock : 
			enable_rcv_clk = 1;
			break;
		case rcv_locked : 
			rcving = 1;
			enable_rcv_clk = 1;
			break;
		case read_data : 
			rcving = 1;
			enable_rcv_clk = 1;
			break;
		case incr_count2 : 
			rcving = 1;
			enable_rcv_clk = 1;
			break;
		case done_read : 
			rcving = 1;
			enable_rcv_clk = 1;
			break;
		case read_stop_bit : 
			rcving = 1;
			enable_rcv_clk = 1;
			break;
		case finish_rcv : 
			enable_rcv_clk = 0;
			rcving = 0;
			done_rcving = 1;
			break;
	}
	switch( Rcv_current_state.read() )
	{
		case waiting : 
			if( !(sin.read()) ) 
			{
				enable_rcv_clk = 1;
			}
			break;
		case check_lock : 
			if( sin.read() ) 
			{
				enable_rcv_clk = 0;
			}
			break;
		case incr_count2 : 
			if( sample.read() && (rcv_bit_cnt_cld.read() != 7) ) 
			{
				read_bit = 1;
			}
			else
			{
				if( sample.read() && (rcv_bit_cnt_cld.read() == 7) ) 
				{
					read_bit = 1;
				}
			}
			break;
	}
}

template<unsigned waiting, unsigned check_lock, unsigned rcv_locked, unsigned read_data, unsigned incr_count2, unsigned done_read, unsigned read_stop_bit, unsigned finish_rcv, unsigned waiting_to_xmit, unsigned send_start, unsigned send_data, unsigned incr_count, unsigned done_xmit, unsigned send_stop_bit, unsigned finish_xmit>
void xmit_rcv_control<waiting, check_lock, rcv_locked, read_data, incr_count2, done_read, read_stop_bit, finish_rcv, waiting_to_xmit, send_start, send_data, incr_count, done_xmit, send_stop_bit, finish_xmit>::always_process_373() 
{
	done_xmitting = 0;
	switch( Xmit_current_state.read() )
	{
		case send_stop_bit : 
			done_xmitting = 1;
			break;
	}
}

template<unsigned waiting, unsigned check_lock, unsigned rcv_locked, unsigned read_data, unsigned incr_count2, unsigned done_read, unsigned read_stop_bit, unsigned finish_rcv, unsigned waiting_to_xmit, unsigned send_start, unsigned send_data, unsigned incr_count, unsigned done_xmit, unsigned send_stop_bit, unsigned finish_xmit>
void xmit_rcv_control<waiting, check_lock, rcv_locked, read_data, incr_count2, done_read, read_stop_bit, finish_rcv, waiting_to_xmit, send_start, send_data, incr_count, done_xmit, send_stop_bit, finish_xmit>::always_process_392() 
{
	if( !(rst.read()) ) 
	{
		Rcv_current_state = waiting;
		rcv_bit_cnt_cld = 0;
	}
	else
	{
		Rcv_current_state = Rcv_next_state.read();
		switch( Rcv_current_state.read() )
		{
			case waiting : 
				rcv_bit_cnt_cld = 0;
				break;
		}
		switch( Rcv_current_state.read() )
		{
			case waiting : 
				if( !(sin.read()) ) 
				{
					rcv_bit_cnt_cld = 0;
				}
				break;
			case incr_count2 : 
				if( sample.read() && (rcv_bit_cnt_cld.read() != 7) ) 
				{
					rcv_bit_cnt_cld = rcv_bit_cnt_cld.read() + 1;
				}
				break;
		}
	}
}

template<unsigned waiting, unsigned check_lock, unsigned rcv_locked, unsigned read_data, unsigned incr_count2, unsigned done_read, unsigned read_stop_bit, unsigned finish_rcv, unsigned waiting_to_xmit, unsigned send_start, unsigned send_data, unsigned incr_count, unsigned done_xmit, unsigned send_stop_bit, unsigned finish_xmit>
void xmit_rcv_control<waiting, check_lock, rcv_locked, read_data, incr_count2, done_read, read_stop_bit, finish_rcv, waiting_to_xmit, send_start, send_data, incr_count, done_xmit, send_stop_bit, finish_xmit>::always_process_434() 
{
	if( !(rst.read()) ) 
	{
		Xmit_current_state = waiting_to_xmit;
		enable_xmit_clk_cld = 0;
		sout_cld = 1;
		xmitting_cld = 0;
		xmit_bit_cnt = 0;
	}
	else
	{
		Xmit_current_state = Xmit_next_state.read();
		sout_cld = 1;
		switch( Xmit_current_state.read() )
		{
			case waiting_to_xmit : 
				xmit_bit_cnt = 0;
				break;
			case send_start : 
				sout_cld = 0;
				enable_xmit_clk_cld = 1;
				xmitting_cld = 1;
				break;
			case send_data : 
				sout_cld = xmitdt.read()[xmit_bit_cnt.read()];
				break;
			case incr_count : 
				sout_cld = xmitdt.read()[xmit_bit_cnt.read()];
				break;
			case finish_xmit : 
				enable_xmit_clk_cld = 0;
				xmitting_cld = 0;
				break;
		}
		switch( Xmit_current_state.read() )
		{
			case send_data : 
				if( !(sample.read()) ) 
				{
					xmit_bit_cnt = xmit_bit_cnt.read() + 1;
				}
				break;
		}
	}
}

template<unsigned waiting, unsigned check_lock, unsigned rcv_locked, unsigned read_data, unsigned incr_count2, unsigned done_read, unsigned read_stop_bit, unsigned finish_rcv, unsigned waiting_to_xmit, unsigned send_start, unsigned send_data, unsigned incr_count, unsigned done_xmit, unsigned send_stop_bit, unsigned finish_xmit>
void xmit_rcv_control<waiting, check_lock, rcv_locked, read_data, incr_count2, done_read, read_stop_bit, finish_rcv, waiting_to_xmit, send_start, send_data, incr_count, done_xmit, send_stop_bit, finish_xmit>::always_process_486() 
{
	enable_xmit_clk = enable_xmit_clk_cld.read();
	rcv_bit_cnt = rcv_bit_cnt_cld.read();
	sout = sout_cld.read();
	xmitting = xmitting_cld.read();
}

#endif
