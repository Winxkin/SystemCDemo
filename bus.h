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
#include <tlm_utils/peq_with_cb_and_phase.h>
#include <map>
#include <vector>
#include <mutex>

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
	sc_clk_in m_clk;

private:
	tlm_utils::simple_initiator_socket<BUS, BUSWIDTH> initiator_sockets[NUM_TS];



	// variable
	std::string m_name;
	sc_core::sc_event e_forward_tran;
	

	unsigned int socket_index;
	tlm::tlm_generic_payload current_trans;
	bool Bus_lock;
	
	struct address
	{
		unsigned int id;
		unsigned int addr;
		unsigned int size;
	};

	std::vector<address> address_mapping;

private:

	void copy_tlm_generic_payload(tlm::tlm_generic_payload& des, tlm::tlm_generic_payload& src)
	{
		des.set_command(src.get_command());
		des.set_address(src.get_address());
		des.set_data_length(src.get_data_length());

		unsigned char* data = new unsigned char[src.get_data_length()];
		std::memcpy(data, src.get_data_ptr(), src.get_data_length());
		des.set_data_ptr(data);

		des.set_response_status(src.get_response_status());
		des.set_streaming_width(src.get_streaming_width());
		des.set_byte_enable_ptr(src.get_byte_enable_ptr());
		des.set_dmi_allowed(src.is_dmi_allowed());
	}

	/*
	* The method use to monitor clock cycles
	*/
	void synchronize_cycles()
	{
		std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ] Clock posedge is triggered.\n";
	}

	
	/*
		Thread handles forward transaction
	*/

	void foward_transaction_process()
	{
		while (true)
		{
			/*
				Waiting for forwarding transaction event that is triggered
			*/
			wait(e_forward_tran);

			/*
				Synchronization with clock cycles, 1 byte data = 1 clock cycle
			*/
			for (unsigned int i = 0; i <= current_trans.get_data_length(); i++)
			{
				wait(m_clk.posedge_event()); // Synchronizing with each clock cycle 1 byte data transfer = 1 cycle
			}
			tlm::tlm_phase fw_phase = tlm::BEGIN_REQ;
			tlm::tlm_sync_enum status;
			sc_core::sc_time delay = sc_core::SC_ZERO_TIME;
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << ": (Initiator socket)" << "[" << socket_index << "]"
				<< " Fowarding transaction to target socket with address " << address_mapping[socket_index].addr << std::endl;

			status = initiator_sockets[socket_index]->nb_transport_fw(current_trans, fw_phase, delay);
		}
	}

	// TS sockets
	void TS_handle_begin_req(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay)
	{
		unsigned int address = trans.get_address();
		bool decode_addr_status = false;

		for (unsigned int i = 0; i < address_mapping.size(); i++)
		{
			if ((address >= address_mapping[i].addr) && (address < (address_mapping[i].addr + address_mapping[i].size)))
			{
				trans.set_address(address - address_mapping[i].addr);
				trans.set_response_status(tlm::TLM_OK_RESPONSE);

				copy_tlm_generic_payload(current_trans, trans);

				socket_index = i;
				decode_addr_status = true;
				break;
			}
		}
		if (decode_addr_status)
		{
			e_forward_tran.notify();
		}
	}

	/*
		Define the functions to handle for Bus Initiator sockets
	*/

	// IS socket
	tlm::tlm_sync_enum nb_transport_bw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_core::sc_time& delay) {
		switch (phase)
		{
		case tlm::BEGIN_REQ:
		{
			// Handle BEGIN_REQ phase
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << ": (Initiator socket)" << "[" << socket_index << "]" << "BEGIN_REQ received" << std::endl;
			break;
		}
		case tlm::END_REQ:
		{
			// Handle END_REQ phase (shouldn't happen here)
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << ": (Initiator socket)" << "[" << socket_index << "]" << "END_REQ received" << std::endl;
			tlm::tlm_phase bw_phase = tlm::END_REQ;
			target_sockets->nb_transport_bw(trans, bw_phase, delay);
			Bus_lock = false;
			break;
		}
		default:
			break;
		};
		return tlm::TLM_ACCEPTED;
	};


	/*
		Define the functions to handle for Bus Target socket
	*/

	//TS socket
	tlm::tlm_sync_enum nb_transport_fw(int id, tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_core::sc_time& delay)
	{
		switch (phase)
		{
		case tlm::BEGIN_REQ:
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << ": (Target socket) BEGIN_REQ received" << std::endl;
			if (!Bus_lock)
			{
				std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << " detect transaction" << std::endl;
				this->Bus_lock = true;
				TS_handle_begin_req(trans, delay);
				return tlm::TLM_ACCEPTED;
			}
			else
			{
				std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << " is locked, ignore transaction !" << std::endl;
				return tlm::TLM_COMPLETED;
			}
			break;
		}
		case tlm::END_REQ:
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << ": (Target socket) END_REQ received" << std::endl;
			tlm::tlm_phase fw_phase = tlm::END_REQ;
			tlm::tlm_sync_enum status;
			Bus_lock = false;
			status = initiator_sockets[socket_index]->nb_transport_fw(trans, fw_phase, delay);
			break;
		}
		default:
			break;
		}
		return tlm::TLM_COMPLETED;
		
	}


public:
	BUS(sc_core::sc_module_name name) :
		sc_core::sc_module(name)
		, m_name(name)
		, target_sockets("target_socket")
		, Bus_lock(false)
		, socket_index(0)
	{
		// Registration nb_transport_fw for target socket
		target_sockets.register_nb_transport_fw(this, &BUS::nb_transport_fw);

		// Registratiob nb_transport_bw for Initiator sockets
		for (unsigned int i = 0; i < NUM_TS; i++)
		{
			initiator_sockets[i].register_nb_transport_bw(this, &BUS::nb_transport_bw);
		}

		// define thread and method
		SC_THREAD(foward_transaction_process);
		sensitive << e_forward_tran;

		SC_METHOD(synchronize_cycles);
		sensitive << m_clk.pos();	// Synchronizing with posdge clock
		dont_initialize();
	};

	~BUS()
	{
	};

	// Using to get corresponding Initiator socket with address mapping
	tlm_utils::simple_initiator_socket<BUS, BUSWIDTH>& mapping_target_sockets(unsigned int _id, unsigned int _addr, unsigned int _size)
	{

		for (unsigned int i = 0; i < address_mapping.size(); i++)
		{
			if ((_addr >= address_mapping[i].addr) && (_addr < (address_mapping[i].addr + address_mapping[i].size)))
			{
				SC_REPORT_ERROR("BUS", "address has existed");
			}
		}

		address newaddress = { _id, _addr, _size };
		address_mapping.push_back(newaddress);

		return initiator_sockets[_id];
	};

};

#endif