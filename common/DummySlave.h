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
    std::string m_name;
	bool m_message;
	bool is_monitor_ports;
    RegisterInterface regs;

	std::map<std::string, sc_core::sc_in<bool>*> input_ports;
	std::map<std::string, sc_core::sc_out<bool>*> output_ports;

private:
	std::map<std::string, bool> input_val_ports;
	std::map<std::string, bool> output_val_ports;

private:

	void end_of_elaboration() override {
		// Set up sensitivity list for monitoring inputs
		SC_METHOD(monitor_inputs);
		for (auto& pair : input_ports) {
			sensitive << *pair.second;
		}
		dont_initialize();
	}

	void monitor_inputs()
	{
		for (const auto& pair : input_ports) {
			if (pair.second->read() != input_val_ports[pair.first])
			{
				input_val_ports[pair.first] = pair.second->read();
				if (is_monitor_ports)
				{
					std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << ": Port " << pair.first << " has changed, value: " << pair.second->read() << std::endl;

				}
			}
		}
	}


    void init_registers()
    {
		regs.add_register("DUMMYRESULT", DUMMYRESULT, 0x00, 0x01, 0x0);
		regs.set_register_callback("DUMMYRESULT", std::bind(&DummySlave::cb_DUMMYRESULT, this,
			std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5));
    }

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
			exit(0);
		}
	}

public:
    tlm_utils::simple_target_socket<DummySlave, BUSWIDTH> target_socket;
	sc_core::sc_in<bool> clk;
	sc_core::sc_in<bool> rst;


    DummySlave(sc_core::sc_module_name name, bool message = false) :
        sc_core::sc_module(name)
		,m_message(message)
		,is_monitor_ports(false)
        ,m_name(name)
        ,target_socket("target_socket")
    {
        target_socket.register_nb_transport_fw(this, &DummySlave::nb_transport_fw);
		init_registers();

    }

	// Method to add ports to the maps
	sc_core::sc_in<bool>* add_input_port(const std::string& name) {
		input_ports[name] = new sc_core::sc_in<bool>();
		input_val_ports[name] = false;
		return input_ports[name];
	}

	// Method to add ports to the maps
	sc_core::sc_out<bool>* add_output_port(const std::string& name) {
		output_ports[name] = new sc_core::sc_out<bool>();
		output_val_ports[name] = false;
		return output_ports[name];
	}

	void set_output_ports(const std::string& name, bool value)
	{
		output_ports[name]->write(value);
	}

	bool read_input_ports(const std::string& name)
	{
		return input_ports[name]->read();
	}

	void monitor_ports(bool is_enable)
	{
		is_monitor_ports = is_enable;
	}


};

#endif