/*
 *	Author: Huan Nguyen-Duy
 *  Date: 2/08/2024
 */

#ifndef _DATA_MEM_H
#define _DATA_MEM_H

#include <systemc>
#include <map>
#include <vector>
#include <iostream>
#include <list>
#include <cstdint>

class DataMem : public sc_core::sc_module
{

public:
	DataMem(sc_core::sc_module_name name) : sc_module(name)
	{

	}

};

#endif
