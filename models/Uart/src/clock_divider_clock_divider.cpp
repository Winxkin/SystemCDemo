/******************************************************************
	clock_divider_clock_divider.cpp
	This file is generated from clock_divider.v by V2SC
	(c) Copryight 2007 by Ali Haj Abutalebi & Leila Mahmoudi Ayough
	Mazdak and Alborz Design Automation
	email: info@mazdak-alborz.com
	website: www.mazdak-alborz.com
*******************************************************************/

#include "../inc/clock_divider_clock_divider.h"

void clock_divider::always_process_57() 
{
	sc_uint<16> clk_cnt_tmp(clk_cnt.read());
	sc_uint<16> div_msb_lsb_tmp(div_msb_lsb.read());
	{ // begin of block: clk_div
		if( !(rst.read()) ) 
		{
			div_msb_lsb_tmp = 0;
			div_msb_lsb = div_msb_lsb_tmp;
			clk_cnt_tmp.range(15,1) = 0;
			clk_cnt = clk_cnt_tmp;
			clk_cnt_tmp[0] = 1;
			clk_cnt = clk_cnt_tmp;
		}
		else
		{
			if( (clk_div_en.read() == 1) && (enable_write.read() == 1) ) 
			{
				if( !(addr.read()) ) 
				{
					div_msb_lsb_tmp.range(7,0) = data_in.read();
					div_msb_lsb = div_msb_lsb_tmp;
				}
				else
				{
					div_msb_lsb_tmp.range(15,8) = data_in.read();
					div_msb_lsb = div_msb_lsb_tmp;
				}
			}
			if( (enable_xmit_clk.read() == 1) || (enable_rcv_clk.read() == 1) ) 
			{
				if( clk_cnt.read() >= (div_msb_lsb.read() - 1) ) 
				{
					clk_cnt_tmp = 0;
					clk_cnt = clk_cnt_tmp;
				}
				else
				{
					clk_cnt_tmp = clk_cnt.read() + 1;
					clk_cnt = clk_cnt_tmp;
				}
			}
			else
			{
				clk_cnt_tmp.range(15,1) = 0;
				clk_cnt = clk_cnt_tmp;
				clk_cnt_tmp[0] = 1;
				clk_cnt = clk_cnt_tmp;
			}
		}
	} // end of block: clk_div
	
}

void clock_divider::always_process_95() 
{
	if( addr.read() == 0 )
	{
		div_data = div_msb_lsb.read().range(7,0);
	}
	else
	{
		div_data = div_msb_lsb.read().range(15,8);
	};
	if( clk_cnt.read() >= (sc_uint<1>(0), div_msb_lsb.read().range(15,1)) )
	{
		sample = 1;
	}
	else
	{
		sample = 0;
	};
}
