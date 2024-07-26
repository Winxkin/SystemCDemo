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

template<unsigned int BUSWIDTH = 32>
class DummySlave : public sc_core::sc_module
{
private:
    std::string m_name;
	bool m_message;
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
				std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << ": Port " << pair.first << " has changed, value: " << pair.second->read() << std::endl;
			}
		}
	}


    void InitializeRegister()
    {
       
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


public:
    tlm_utils::simple_target_socket<DummySlave, BUSWIDTH> target_socket;
	sc_core::sc_in<bool> clk;
	sc_core::sc_in<bool> rst;


    DummySlave(sc_core::sc_module_name name, bool message = false) :
        sc_core::sc_module(name)
		,m_message(message)
        ,m_name(name)
        ,target_socket("target_socket")
    {
        target_socket.register_nb_transport_fw(this, &DummySlave::nb_transport_fw);
        InitializeRegister();

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


};

#endif