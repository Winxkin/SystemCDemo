/*
 *	Author: Huan Nguyen-Duy
 *  Date: 11/07/2024
 */

#ifndef _BUS_H
#define _BUS_H
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/multi_passthrough_initiator_socket.h>
#include <tlm_utils/multi_passthrough_target_socket.h>
#include <map>
#include <vector>


typedef enum
{
	AXI32 = 32,
	AXI64 = 64,
	APB = 32
} BUS_TYPE;

template<unsigned int BUSWIDTH = 32, unsigned int NUM_TS = 10>
class BUS : public sc_core::sc_module
{
public:
	tlm_utils::multi_passthrough_target_socket<BUS, BUSWIDTH> target_sockets;

private:
	tlm_utils::simple_initiator_socket<BUS, BUSWIDTH> initiator_sockets[NUM_TS];


	struct address
	{
		unsigned int id;
		unsigned int addr;
		unsigned int size;
	};

	std::vector<address> address_mapping;

	void b_transport(int id, tlm::tlm_generic_payload& trans, sc_core::sc_time& delay)
	{
		unsigned int address = trans.get_address();
		for (unsigned int i = 0; i < address_mapping.size(); i++)
		{
			if ((address >= address_mapping[i].addr) && (address < (address_mapping[i].addr + address_mapping[i].size)))
			{
				trans.set_address(address - address_mapping[i].addr);
				initiator_sockets[i]->b_transport(trans, delay);
			}
		}
	}

public:
	BUS(sc_core::sc_module_name name) :
		sc_core::sc_module(name)
		, target_sockets("target_socket")
	{
		target_sockets.register_b_transport(this, &BUS::b_transport);
	};

	~BUS()
	{

	}


	tlm_utils::simple_initiator_socket<BUS, BUSWIDTH>& mapping_target_sockets(unsigned int _id, unsigned int _addr, unsigned int _size)
	{
		
		for (unsigned int i = 0; i < address_mapping.size(); i++)
		{
			if ((_addr >= address_mapping[i].addr) && (_addr < (address_mapping[i].addr + address_mapping[i].size)))
			{
				SC_REPORT_ERROR("BUS","address has existed");
			}
		}

		address newaddress= { _id, _addr, _size };
		address_mapping.push_back(newaddress);

		return initiator_sockets[_id];
	}

};

#endif