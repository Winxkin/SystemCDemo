/******************************************************************
	address_decode_address_decode.cpp
	This file is generated from address_decode.v by V2SC
	(c) Copryight 2007 by Ali Haj Abutalebi & Leila Mahmoudi Ayough
	Mazdak and Alborz Design Automation
	email: info@mazdak-alborz.com
	website: www.mazdak-alborz.com
*******************************************************************/

#include "../inc/address_decode_address_decode.h"

void address_decode::always_process_47() 
{
	clk_div_en = 0;
	xmitdt_en = 0;
	clr_int_en = 0;
	ser_if_select = 0;
	switch( addr.read() )
	{
		case 0 : 
			clk_div_en = 1;
			break;
		case 1 : 
			clk_div_en = 1;
			break;
		case 4 : 
			xmitdt_en = 1;
			ser_if_select = addr.read().range(1,0);
			break;
		case 5 : 
			ser_if_select = addr.read().range(1,0);
			break;
		case 6 : 
			ser_if_select = addr.read().range(1,0);
			break;
		case 7 : 
			clr_int_en = 1;
			break;
		default :
			clk_div_en = 0;
			xmitdt_en = 0;
			ser_if_select = "11";
			clr_int_en = 0;
			break;
	}
}
