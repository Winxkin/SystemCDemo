 /*
 *	Description: Implementation the bus MMI with clock synchronization
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

typedef enum
{
	D8BIT = 0,
	D16BIT = 1,
	D32BIT = 2,
	D64BIT = 3,
	D128BIT = 4

} DATAWIDTH;

template<unsigned int BUSWIDTH = 32, unsigned int NUM_TS = 10, unsigned int DATA_WIDTH = D8BIT>
class BUS : public sc_core::sc_module
{
public:
	tlm_utils::multi_passthrough_target_socket<BUS, BUSWIDTH> target_sockets;
	sc_clk_in m_clk;

private:
	tlm_utils::simple_initiator_socket<BUS, BUSWIDTH> initiator_sockets[NUM_TS];

	std::string m_name;
	unsigned int div_index;
	unsigned int socket_index;
	bool Bus_lock;
	bool m_clkmonitor;
	sc_core::sc_event e_forward_tran;
	tlm::tlm_generic_payload current_trans;
	int current_ts_id;
	
	struct address
	{
		unsigned int id;
		unsigned int addr;
		unsigned int size;
	};

	std::vector<address> address_mapping;

private:

	/*
	* copy_tlm_generic_payload
	* 
	* Impelmentation the copy operation from source TLM generic payload to destination TLM generic payload
	* 
	* @param des Reference to destination TLM generic payload
	* 
	* @param src Reference to source TLM generic payload
	* 
	*/
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
	* synchronize_cycles
	* 
	* Implementation the method to monitor clock cycles
	* 
	*/
	void synchronize_cycles()
	{
		if (m_clkmonitor)
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ] Clock posedge is triggered.\n";
		}
	}

	
	/*
		Thread handles forward transaction
	*/

	/*
	* foward_transaction_process
	*
	* Implementation the thread to synchronize with clock cycles and forward the transaction to slave through the corresponding initiator 
	*
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
			unsigned int num_cycles = (current_trans.get_data_length() / div_index) + 1;		// Note: 1 cycle clock for address transfering
			for (unsigned int i = 0; i < num_cycles; i++)
			{
				wait(m_clk.posedge_event()); // Synchronizing with each clock cycle
				if (i == 0)
				{
					std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << ": (Bus) (Address Phase) accepting address 0x" << std::hex
						<< current_trans.get_address() << std::endl;
				}
				else
				{
					if (current_trans.is_write())
					{
						std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << ": (Initiator socket)" << "[" << socket_index << "]"
							<< "(Data phase) Writing data :	0x" << std::hex << (unsigned int)current_trans.get_data_ptr()[i-1] << std::endl;
					}
				}
			}
			tlm::tlm_phase fw_phase = tlm::BEGIN_REQ;
			tlm::tlm_sync_enum status;
			sc_core::sc_time delay = sc_core::SC_ZERO_TIME;
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << ": (Initiator socket)" << "[" << socket_index << "]"
				<< " Fowarding transaction to target socket successfully with base address 0x" << std::hex << address_mapping[socket_index].addr << std::endl;

			status = initiator_sockets[socket_index]->nb_transport_fw(current_trans, fw_phase, delay);
		}
	}

	/*
	* TS_handle_begin_req
	* 
	* Implementation for decoding address from transaction payload and selecting the suitable initiator socket with the corresponding id
	* 
	* @param trans Reference to the generic payload object containing the transaction details
	*              such as command, address, and data.
	* 
	* @param delay Reference to the annotated delay. Specifies the timing delay for the transaction
	*              and may be updated by the function. 
	*/
	void TS_handle_begin_req(int id, tlm::tlm_generic_payload& trans, sc_core::sc_time& delay)
	{
		unsigned int address = trans.get_address();
		bool decode_addr_status = false;
		current_ts_id = id;

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

	/*
	 * nb_transport_bw
	 *
	 * Implements the non-blocking backward transport interface for the nitiator.
	 *
	 * @param trans Reference to the generic payload object containing the transaction details
	 *              such as command, address, and data.
	 *
	 * @param phase Reference to the transaction phase. The current phase of the transaction,
	 *              which may be updated by the function.
	 *
	 * @param delay Reference to the annotated delay. Specifies the timing delay for the transaction
	 *              and may be updated by the function.
	 *
	 * @return tlm::tlm_sync_enum Enumeration indicating the synchronization state of the transaction:
	 *         - TLM_ACCEPTED: Transaction is accepted, and no immediate further action is required.
	 *         - TLM_UPDATED: Transaction phase has been updated. The initiator should check the new phase.
	 *         - TLM_COMPLETED: Transaction is completed immediately, and no further phases will occur.
	 */
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
			// Handle END_REQ phase
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << ": (Initiator socket)" << "[" << socket_index << "]" << "END_REQ received" << std::endl;
			tlm::tlm_phase bw_phase = tlm::END_REQ;
			target_sockets[current_ts_id]->nb_transport_bw(trans, bw_phase, delay);
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

	/*
	 * nb_transport_fw
	 *
	 * Implements the non-blocking forward transport interface for the target.
	 *
	 * @param id    Integer identifier to distinguish between multiple initiators or channels.
	 *
	 * @param trans Reference to the generic payload object containing the transaction details
	 *              such as command, address, and data.
	 *
	 * @param phase Reference to the transaction phase. The current phase of the transaction,
	 *              which may be updated by the function.
	 *
	 * @param delay Reference to the annotated delay. Specifies the timing delay for the transaction
	 *              and may be updated by the function.
	 *
	 * @return tlm::tlm_sync_enum Enumeration indicating the synchronization state of the transaction:
	 *         - TLM_ACCEPTED: Transaction is accepted, and no immediate further action is required.
	 *         - TLM_UPDATED: Transaction phase has been updated. The initiator should check the new phase.
	 *         - TLM_COMPLETED: Transaction is completed immediately, and no further phases will occur.
	 */
	tlm::tlm_sync_enum nb_transport_fw(int id, tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_core::sc_time& delay)
	{
		switch (phase)
		{
		case tlm::BEGIN_REQ:
		{
			// Handle BEGIN_REQ phase
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << ": (Target socket) BEGIN_REQ received" << std::endl;
			if (!Bus_lock)
			{
				std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << " detect transaction" << std::endl;
				this->Bus_lock = true;
				TS_handle_begin_req(id, trans, delay);
				return tlm::TLM_ACCEPTED;
			}
			else
			{
				// Handle END_REQ phase
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
	/*
	* BUS constructor
	* 
	* @param name Reference to sc_module name
	* 
	* @param enclkmonitor To enable clock monitor through print message when the posedge clock event is triggered 
	*/
	BUS(sc_core::sc_module_name name, bool enclkmonitor) :
		sc_core::sc_module(name)
		, m_name(name)
		, target_sockets("target_socket")
		, Bus_lock(false)
		, socket_index(0)
		, m_clkmonitor(enclkmonitor)
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

		// Setting data width
		switch (DATA_WIDTH)
		{
		case D8BIT:
			div_index = 1;
			break;
		case D16BIT:
			div_index = 2;
			break;
		case D32BIT:
			div_index = 4;
			break;
		case D64BIT:
			div_index = 8;
			break;
		case D128BIT:
			div_index = 16;
			break;
		default:
			div_index = 1;
			break;
		}
	};

	/*
	* BUS destructor
	*/
	~BUS()
	{
	};

	/*
	* mapping_target_sockets
	* 
	* Implement the registration socket address range for target socket
	* 
	* @param _id   The id number of target socket in bus memory mapping I/O
	* 
	* @param _addr The base address of target socket that is registered into bus memory mapping I/O
	* 
	* @param _size the range of address space
	* 
	* @return tlm_utils::simple_initiator_socket is the initiator socket with id registration used to bind with the corresponding target socket
	*/
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