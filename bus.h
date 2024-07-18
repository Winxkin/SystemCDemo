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
	tlm_utils::peq_with_cb_and_phase<BUS> m_is_peq;
	tlm_utils::peq_with_cb_and_phase<BUS> m_ts_peq;


	// variable
	std::string m_name;
	sc_core::sc_event e_forward_tran;
	

	unsigned int socket_index;
	tlm::tlm_generic_payload* current_trans;
	sc_core::sc_time* current_delay;
	bool Bus_lock;
	
	struct address
	{
		unsigned int id;
		unsigned int addr;
		unsigned int size;
	};

	std::vector<address> address_mapping;

private:

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
			for (unsigned int i = 0; i < current_trans->get_data_length(); i++)
			{
				wait(m_clk.posedge_event()); // Synchronizing with each clock cycle 1 byte data transfer = 1 cycle
			}
			
			/*
				Preparing transaction to forward to corresponding slave
			*/

			tlm::tlm_phase fw_phase = tlm::BEGIN_REQ;
			tlm::tlm_sync_enum status;
			status = initiator_sockets[socket_index]->nb_transport_fw(*current_trans, fw_phase, *current_delay);

			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << ": (Initiator socket)" << "[" << socket_index << "]" 
					<< " Fowarding transaction to target socket with address " << address_mapping[socket_index].addr << std::endl;

			if (status == tlm::TLM_UPDATED)
			{
				m_is_peq.notify(*current_trans, fw_phase, *current_delay);
			}
			else if (status == tlm::TLM_COMPLETED) {
				if (current_trans->is_response_error()) {
					SC_REPORT_ERROR("TLM-2", "Response error from nb_transport_fw");
				}
			}
		}
	}

	/*
		Define the functions to handle for Bus Initiator sockets
	*/

	// IS socket
	tlm::tlm_sync_enum nb_transport_bw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_core::sc_time& delay) {
		m_is_peq.notify(trans, phase, delay);
		return tlm::TLM_ACCEPTED;
	}

	// IS socket
	void Is_peq_callback(tlm::tlm_generic_payload& trans, const tlm::tlm_phase& phase)
	{
		sc_core::sc_time delay;
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
			break;
		}
		case tlm::BEGIN_RESP:
		{
			// Handle BEGIN_RESP phase
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << ": (Initiator socket)" << "[" << socket_index << "]" << "BEGIN_RESP received" << std::endl;

			/*
				Return transaction to Master
			*/
			tlm::tlm_phase bw_phase = tlm::BEGIN_REQ;
			target_sockets->nb_transport_bw(trans, bw_phase, *current_delay);
			break;
		}
		case tlm::END_RESP:
		{
			// Handle END_RESP phase
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << ": (Initiator socket)" << "[" << socket_index << "]" << "END_RESP received" << std::endl;
			break;
		}
		default:
			SC_REPORT_FATAL("TLM-2", "Illegal transaction phase received");
		}
	}

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
			if (!Bus_lock)
			{
				this->Bus_lock = true;
				// Notify Ts_peq_callback event
				m_ts_peq.notify(trans, phase, delay);
				return tlm::TLM_ACCEPTED;
			}
			else
			{
				std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << " is locked, ignore transaction !" << std::endl;
				return tlm::TLM_COMPLETED;
			}
			break;
		}
		case tlm::END_RESP:
		{
			if (!Bus_lock)
			{
				/*
					End request status is sentd from Master
				*/
				m_ts_peq.notify(trans, phase, delay);
			}
			else
			{
				std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << " is locked, ignore transaction !" << std::endl;
				return tlm::TLM_COMPLETED;
			}
			break;
		}
		default:
			break;
		}
		return tlm::TLM_COMPLETED;
		
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
				socket_index = i;
				decode_addr_status = true;

				// update trans payload and delay parameter to internal current_trans and current_delay parameter
				current_trans = &trans;
				current_delay = &delay;
				break;
			}
		}
		if (decode_addr_status)
		{
			e_forward_tran.notify();
		}
	}


	// TS sockets
	void Ts_peq_callback(tlm::tlm_generic_payload& trans, const tlm::tlm_phase& phase)
	{
		sc_core::sc_time delay;
		switch (phase)
		{
		case tlm::BEGIN_REQ:
		{
			// Handle BEGIN_REQ phase
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << ": (Target socket) BEGIN_REQ received" << std::endl;
			tlm::tlm_phase next_phase = tlm::END_REQ;
			target_sockets->nb_transport_bw(trans, next_phase, *current_delay);
			TS_handle_begin_req(trans, *current_delay);
			break;
		}
		case tlm::END_REQ:
		{
			// Handle END_REQ phase (shouldn't happen here)
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << ": (Target socket) END_REQ received" << std::endl;
			break;
		}
		case tlm::BEGIN_RESP:
		{
			// Handle BEGIN_RESP phase
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << ": (Target socket) BEGIN_RESP received" << std::endl;
			break;
		}
		case tlm::END_RESP:
		{
			// Handle END_RESP phase
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << ": (Target socket) END_RESP received" << std::endl;

			/*
			* Forwarding END_RESP status to Slave through Initiator socket
			*/
			tlm::tlm_phase fw_phase = tlm::BEGIN_REQ;
			tlm::tlm_sync_enum status;
			status = initiator_sockets[socket_index]->nb_transport_fw(*current_trans, fw_phase, *current_delay);

			current_trans = nullptr;
			current_delay = nullptr;
			socket_index = 0;
			Bus_lock = false;
			break;
		}
		default:
			SC_REPORT_FATAL("TLM-2", "Illegal transaction phase received");
		}
	}

	



public:
	BUS(sc_core::sc_module_name name) :
		sc_core::sc_module(name)
		, m_name(name)
		, target_sockets("target_socket")
		, current_trans(nullptr)
		, current_delay(nullptr)
		, m_ts_peq(this, &BUS::Ts_peq_callback)
		, m_is_peq(this, &BUS::Is_peq_callback)
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
		delete current_delay;
		delete current_trans;
	}

	// Using to get corresponding Initiator socket with address mapping
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