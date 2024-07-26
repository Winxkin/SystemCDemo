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
	sc_core::sc_in clk;
	sc_core::sc_in rst;


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
	void bind_input_port(const string& name, sc_in<bool>& in_port) {
		input_ports[name] = &in_port;
	}

	void bind_output_port(const string& name, sc_out<bool>& out_port) {
		output_ports[name] = &out_port;
	}



};

#endif