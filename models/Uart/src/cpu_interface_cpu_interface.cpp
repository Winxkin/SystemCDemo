/******************************************************************
	cpu_interface_cpu_interface.cpp
	This file is generated from cpu_interface.v by V2SC
	(c) Copryight 2007 by Ali Haj Abutalebi & Leila Mahmoudi Ayough
	Mazdak and Alborz Design Automation
	email: info@mazdak-alborz.com
	website: www.mazdak-alborz.com
*******************************************************************/

#include "../inc/cpu_interface_cpu_interface.h"

void cpu_interface::assign_process_data_out_82() 
{
	if( clk_div_en.read() == 1 )
	{
		data_out = div_data.read();
	}
	else
	{
		data_out = ser_if_data.read();
	};
}
