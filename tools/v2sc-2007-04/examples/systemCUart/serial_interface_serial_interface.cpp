/******************************************************************
	serial_interface_serial_interface.cpp
	This file is generated from serial_interface.v by V2SC
	(c) Copryight 2007 by Ali Haj Abutalebi & Leila Mahmoudi Ayough
	Mazdak and Alborz Design Automation
	email: info@mazdak-alborz.com
	website: www.mazdak-alborz.com
*******************************************************************/

#include "serial_interface_serial_interface.h"

void serial_interface::always_process_129() 
{
	sc_uint<8> recvdt_reg_tmp(recvdt_reg.read());
	{ // begin of block: conv
		if( !(rst.read()) ) 
		{
			xmitdt_reg = 0;
			recvdt_reg_tmp = 0;
			recvdt_reg = recvdt_reg_tmp;
		}
		else
		{
			if( xmitdt_en.read() && enable_write.read() ) 
			{
				xmitdt_reg = data_in.read();
			}
			else
			{
				if( read_bit.read() ) 
				{
					recvdt_reg_tmp[rcv_bit_cnt.read()] = sin.read();
					recvdt_reg = recvdt_reg_tmp;
				}
			}
		}
	} // end of block: conv
	conv:;
}

void serial_interface::assign_process_xmitdt_150() 
{
	xmitdt = xmitdt_reg.read();
}

void serial_interface::assign_process_recvdt_151() 
{
	recvdt = recvdt_reg.read();
}

void serial_interface::always_process_155() 
{
	switch( ser_if_select.read() )
	{
		case 0 : 
			MW_ser_out_muxdtempl = MW_ser_out_muxdin0l.read();
			break;
		case 1 : 
			MW_ser_out_muxdtempl = MW_ser_out_muxdin1l.read();
			break;
		case 2 : 
			MW_ser_out_muxdtempl = MW_ser_out_muxdin2l.read();
			break;
		default :
			MW_ser_out_muxdtempl = MW_ser_out_muxdin3l.read();
			break;
	}
}

void serial_interface::assign_process_ser_if_data_163() 
{
	ser_if_data = MW_ser_out_muxdtempl.read();
}

void serial_interface::assign_process_MW_ser_out_muxdin0l_164() 
{
	MW_ser_out_muxdin0l = xmitdt.read();
}

void serial_interface::assign_process_MW_ser_out_muxdin1l_165() 
{
	MW_ser_out_muxdin1l = recvdt.read();
}

void serial_interface::assign_process_MW_ser_out_muxdin2l_166() 
{
	MW_ser_out_muxdin2l = status.read();
}

void serial_interface::assign_process_MW_ser_out_muxdin3l_167() 
{
	MW_ser_out_muxdin3l = zeros.read();
}

void serial_interface::assign_process_zeros_171() 
{
	zeros = 0;
}
