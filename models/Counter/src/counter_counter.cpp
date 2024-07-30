/******************************************************************
	counter_counter.cpp
	This file is generated from counter.v by V2SC
	(c) Copryight 2007 by Ali Haj Abutalebi & Leila Mahmoudi Ayough
	Mazdak and Alborz Design Automation
	email: info@mazdak-alborz.com
	website: www.mazdak-alborz.com
*******************************************************************/

#include "Counter/inc/counter_counter.h"

void counter::assign_process_dout_10() 
{
	dout = countval.read();
}

void counter::always_process_12() 
{
	if( clear.read() ) 
	{
		countval = 0;
	}
	else
	{
		if( load.read() ) 
		{
			countval = din.read();
		}
		else
		{
			if (start.read())
			{
				countval = countval.read() + 1;
			}
		}
	}
}
