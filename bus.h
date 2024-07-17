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

typedef sc_core::sc_in<bool> sc_clk_in;

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
	//sc_clk_in m_clk;

private:
	tlm_utils::simple_initiator_socket<BUS, BUSWIDTH> initiator_sockets[NUM_TS];

	tlm::tlm_generic_payload local_trans;
	sc_core::sc_time local_delay;
	unsigned int socket_index;
	sc_core::sc_event efowardtran;
	sc_core::sc_event ecomplete;
	struct address
	{
		unsigned int id;
		unsigned int addr;
		unsigned int size;
	};

	std::vector<address> address_mapping;

private:

	void synchronize_cycles(unsigned int num_cycles ,sc_core::sc_time& clk_period)
	{
		wait(num_cycles * clk_period);
	}

	void foward_tran_process()
	{
		while (true)
		{
			wait(efowardtran);
			wait(10, sc_core::SC_NS);
			initiator_sockets[socket_index]->b_transport(local_trans, local_delay);

		}
	}


	void b_transport(int id, tlm::tlm_generic_payload& trans, sc_core::sc_time& delay)
	{
		
		unsigned int address = trans.get_address();
		bool decodeaddstatus = false;
		for (unsigned int i = 0; i < address_mapping.size(); i++)
		{
			if ((address >= address_mapping[i].addr) && (address < (address_mapping[i].addr + address_mapping[i].size)))
			{
				trans.set_address(address - address_mapping[i].addr);
				trans.set_response_status(tlm::TLM_OK_RESPONSE);
				socket_index = i;
				local_trans.set_address(trans.get_address());
				local_trans.set_data_ptr(trans.get_data_ptr());
				local_trans.set_data_length(trans.get_data_length());
				local_trans.set_command(trans.get_command());
				local_trans.set_response_status(trans.get_response_status());
				local_trans.set_streaming_width(trans.get_streaming_width());
				local_trans.set_byte_enable_ptr(trans.get_byte_enable_ptr());
				local_trans.set_dmi_allowed(trans.is_dmi_allowed());
				local_delay = delay;
				decodeaddstatus = true;
				break;
			}
		}
		if (decodeaddstatus)
		{
			efowardtran.notify();
		}
	}



public:
	BUS(sc_core::sc_module_name name) :
		sc_core::sc_module(name)
		, target_sockets("target_socket")
	{
		target_sockets.register_b_transport(this, &BUS::b_transport);
		SC_THREAD(foward_tran_process);
		sensitive << efowardtran;
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