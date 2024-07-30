/******************************************************************
	status_registers_status_registers.cpp
	This file is generated from status_registers.v by V2SC
	(c) Copryight 2007 by Ali Haj Abutalebi & Leila Mahmoudi Ayough
	Mazdak and Alborz Design Automation
	email: info@mazdak-alborz.com
	website: www.mazdak-alborz.com
*******************************************************************/

#include "../inc/status_registers_status_registers.h"

void status_registers::always_process_55() 
{
	if( !(rst.read()) ) 
	{
		xmitting_reg = 0;
		done_xmitting_reg = 0;
		rcving_reg = 0;
		done_rcving_reg = 0;
	}
	else
	{
		if( clear_flags.read() ) 
		{
			xmitting_reg = 0;
			done_xmitting_reg = 0;
			rcving_reg = 0;
			done_rcving_reg = 0;
		}
		else
		{
			xmitting_reg = xmitting.read();
			rcving_reg = rcving.read();
			if( done_xmitting.read() == 1 ) 
			{
				done_xmitting_reg = done_xmitting.read();
			}
			if( done_rcving.read() == 1 ) 
			{
				done_rcving_reg = done_rcving.read();
			}
		}
	}
}

void status_registers::assign_process_int2_82() 
{
	if( done_xmitting_reg.read() || done_rcving_reg.read() )
	{
		int2 = 1;
	}
	else
	{
		int2 = 0;
	};
}

void status_registers::assign_process_complex_status_84() 
{
	sc_uint<8> status_tmp(status.read());
	status_tmp.range(7,4) = 0;
	status = status_tmp;
	status_tmp[3] = done_rcving_reg.read();
	status = status_tmp;
	status_tmp[2] = done_xmitting_reg.read();
	status = status_tmp;
	status_tmp[1] = rcving_reg.read();
	status = status_tmp;
	status_tmp[0] = xmitting_reg.read();
	status = status_tmp;
}
