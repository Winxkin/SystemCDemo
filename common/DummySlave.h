/*
 *	Author: Huan Nguyen-Duy
 *  Date: 12/07/2024
 */

#ifndef _DUMMYSLAVE_H
#define _DUMMYSLAVE_H
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/multi_passthrough_initiator_socket.h>
#include <tlm_utils/multi_passthrough_target_socket.h>
#include <map>
#include <vector>
#include <iostream>
#include <cstdint>
#include "Registerif.h"

#define DUMMYRESULT 0x00

template<unsigned int BUSWIDTH = 32>
class DummySlave : public sc_core::sc_module
{
private:
	RegisterInterface regs;
    std::string m_name;
	bool m_message;
	
	/* The internal variables for triggered ports operation*/
	bool m_cur_is_pos;
	bool m_cur_triggered_val;
	std::string m_cur_port_name;
	sc_core::sc_event e_triggerd_port;

	/* The internal flags to enable or disable port monitor*/
	bool m_portmonitor;
	bool m_clkmonitor;

	std::map<std::string, sc_core::sc_in<bool>*> input_ports;
	std::map<std::string, sc_core::sc_out<bool>*> output_ports;

private:
	std::map<std::string, bool> input_val_ports;
	std::map<std::string, bool> output_val_ports;

	
private:

	/**@brief
	 * end_of_elaboration
	 *
	 * The function end_of_elaboration is called before starting simulation
	 *
	 */
	void end_of_elaboration() override {
		// Set up sensitivity list for monitoring inputs
		SC_METHOD(monitor_inputs);
		for (auto& pair : input_ports) {
			sensitive << *pair.second;
		}
		dont_initialize();
	}

	/**@brief
	 * monitor_inputs
	 *
	 * The method uses to monitor input ports
	 *
	 */
	void monitor_inputs()
	{
		for (const auto& pair : input_ports) {
			if (pair.second->read() != input_val_ports[pair.first])
			{
				input_val_ports[pair.first] = pair.second->read();
				if (m_portmonitor)
				{
					std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << ": Port " << pair.first << " has changed, value: " << pair.second->read() << std::endl;

				}
			}
		}
	}

	/**@brief
	 * init_registers
	 *
	 *	Initialization registers
	 *
	 */
    void init_registers()
    {
		regs.add_register("DUMMYRESULT", DUMMYRESULT, 0x00, 0x01, 0x0, READWRITE);
		regs.set_register_callback("DUMMYRESULT", std::bind(&DummySlave::cb_DUMMYRESULT, this,
			std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5));
    }

	/**@brief
	 * nb_transport_fw
	 *
	 * Implements the non-blocking backward transport interface for the initiator.
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
	tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_core::sc_time& delay) {
		switch (phase)
		{
		case tlm::BEGIN_REQ:
		{
			// Handle BEGIN_REQ phase
			if (m_message)
			{
				std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << " BEGIN_REQ received" << std::endl;
			}
			switch (trans.get_command()) {
			case tlm::TLM_WRITE_COMMAND:
			{
				unsigned int wdata = 0;
				std::memcpy(&wdata, trans.get_data_ptr(), sizeof(wdata));
				regs.update_register(trans.get_address(), wdata);
				if (m_message)
				{
					std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << ": Received transaction with address 0x" << std::hex << trans.get_address() << " data: 0x" << std::hex << wdata << std::dec << std::endl;
				}
				trans.set_response_status(tlm::TLM_OK_RESPONSE);
				break;
			}
			case tlm::TLM_READ_COMMAND:
			{
				unsigned int value = regs[trans.get_address()].get_value();
				unsigned char* cvalue = reinterpret_cast<unsigned char*>(&value);
				memcpy(trans.get_data_ptr(), cvalue, trans.get_data_length());
				trans.set_response_status(tlm::TLM_OK_RESPONSE);
				break;
			}
			default:
				break;
			}

			/*
				Return data and END_REQ to bus
			*/
			tlm::tlm_phase next_phase = tlm::END_REQ;
			target_socket->nb_transport_bw(trans, next_phase, delay);
			break;
		}
		case tlm::END_REQ:
		{
			// Handle END_REQ phase (shouldn't happen here)
			if (m_message)
			{
				std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << " END_REQ received" << std::endl;
			}
			break;
		}
		default:
			break;
		}
		return tlm::TLM_ACCEPTED;
	}

	/* Callback register */

	/**@brief
	 * cb_DUMMYRESULT
	 *
	 *	The callback function for DUMMYRESULT register
	 *	@param name			The name of register
	 *  @param value		The value of register
	 *  @param old_value	the previous value
	 *  @param mask			the mask of register
	 *  @param ch			the register channel
	 */
	void cb_DUMMYRESULT(const std::string& name, uint32_t value, uint32_t old_value, uint32_t mask, uint32_t ch)
	{
		if (name == "DUMMYRESULT")
		{
			if (value)
			{
				std::cout << "-----------------------\n";
				std::cout << "      TM is Pass       \n";
				std::cout << "-----------------------\n";
			}
			else
			{
				std::cout << "-----------------------\n";
				std::cout << "      TM is Fail       \n";
				std::cout << "-----------------------\n";
			}
			std::cout << "Total simulation time: " << sc_core::sc_time_stamp().to_double() << "NS" << std::endl;
			sc_core::sc_stop(); // To stop systemC kernel simulation
		}
	}

	/* SC_METHOD function */

	/**@brief
	* mth_synchronize_cycles
	*
	* Implementation the method to monitor clock cycles
	*
	*/
	void mth_synchronize_cycles()
	{
		if (m_clkmonitor)
		{
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ] Clock posedge is triggered.\n";
		}
	}

	/* SC_THREAD function */

	/**@brief
	* thr_triggered_port_process
	*
	* Implementation the process to trigger specific ports
	*
	*/
	void thr_triggered_port_process()
	{
		while (true)
		{
			wait(e_triggerd_port);
			if (m_cur_is_pos)
			{
				wait(clk.posedge_event());
			}
			else
			{
				wait(clk.negedge_event());
			}
			output_ports[m_cur_port_name]->write(!m_cur_triggered_val);
		}
	}

	/**@brief
	* mth_reset
	*
	* Implementation the method to handle reset operation
	*
	*/
	void mth_reset()
	{
		regs.reset_regs();
	}

public:
    tlm_utils::simple_target_socket<DummySlave, BUSWIDTH> target_socket;
	sc_core::sc_in<bool> clk;
	sc_core::sc_in<bool> rst;

	/**@brief
	* DummySlave
	*
	* DummySlave Constructure
	* @param name Reference to sc_module name
	* @param message To enable message log
	*/
    DummySlave(sc_core::sc_module_name name, bool message = false) :
        sc_core::sc_module(name)
		,m_message(message)
		,m_portmonitor(false)
        ,m_name(name)
        ,target_socket("target_socket")
		,m_cur_is_pos(true)
		,m_cur_triggered_val(false)
		,m_clkmonitor(false)
    {
        target_socket.register_nb_transport_fw(this, &DummySlave::nb_transport_fw);
		init_registers();

		SC_THREAD(thr_triggered_port_process);
		sensitive << e_triggerd_port;

		SC_METHOD(mth_synchronize_cycles);
		sensitive << clk.pos();	// Synchronizing with posdge clock
		dont_initialize();

		SC_METHOD(mth_reset);
		sensitive << rst;
    }


	/**@brief
	* add_input_port
	*
	* To add input specific input ports to DummySlave
	* @param name Reference to the port name
	*/
	sc_core::sc_in<bool>* add_input_port(const std::string& name) {
		input_ports[name] = new sc_core::sc_in<bool>();
		input_val_ports[name] = false;
		return input_ports[name];
	}

	/**@brief
	* add_output_port
	*
	* To add ouput specific output ports to DummySlave
	* @param name Reference to the port name
	*/
	sc_core::sc_out<bool>* add_output_port(const std::string& name) {
		output_ports[name] = new sc_core::sc_out<bool>();
		output_val_ports[name] = false;
		return output_ports[name];
	}

	/**@brief
	* set_output_ports
	*
	* Using to set specific output port
	* @param name Reference to the port name
	* @param value the value of the output port
	*/
	void set_output_ports(const std::string& name, bool value)
	{
		output_ports[name]->write(value);
	}

	/**@brief
	* set_output_ports
	*
	* Using to trigger specific output port
	* @param name Reference to the port name
	* @param high_level Indicating the triggered level
	* @param is_pos Indicating the clock edge synchronization is positive or negative
	*/
	void trigger_output_ports(const std::string& name, bool high_level, bool is_pos)
	{
		output_ports[name]->write(high_level);
		m_cur_triggered_val = high_level;
		m_cur_port_name = name;
		m_cur_is_pos = is_pos;
		e_triggerd_port.notify();
	}

	/**@brief
	* read_input_ports
	*
	* Using to read the value of specific port
	* @param name Reference to the port name
	*/
	bool read_input_ports(const std::string& name)
	{
		return input_ports[name]->read();
	}

	/**@brief
	* monitor_ports
	*
	* Using to enable or disable port monitor operation
	* @param is_enable Indicating whether enabling or disabling port monitor
	*/
	void monitor_ports(bool is_enable)
	{
		m_portmonitor = is_enable;
	}

	/**@brief
	* enable_monitor_clock
	*
	* Using to enable or disable clock monitor operation
	* @param is_enable Indicating whether enabling or disabling clock monitor
	*/
	void enable_monitor_clock(bool is_enable)
	{
		m_clkmonitor = is_enable;
	}


};

#endif