#pragma once
#ifndef _COUNTER_WRAPPER_H
#define _COUNTER_WRAPPER_H

#include "inc/counter_counter.h"
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
#include "commondef.h"

#define COUNTERINPUT	(0x00 + 0x00)
#define COUNTEROUTPUT	(0x00 + 0x04)


template<unsigned int BUSWIDTH = 32>
class wrapper_counter : sc_module
{
private:
	std::string m_name;
	counter m_counter;
	RegisterInterface regs;
	bool m_message;

private:
	/* Counter */
	sc_signal<bool>	clock;
	sc_signal<bool>	load;
	sc_signal<bool>	clear;
	sc_signal<bool>	start;
	sc_signal<sc_uint<8> >	din;
	sc_signal<sc_uint<8> >	dout;

public:
	sc_in<bool> m_clk;
	sc_in<bool> m_load;
	sc_in<bool> m_clear;
	sc_in<bool> m_start;
private:
	tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_core::sc_time& delay) {
		switch (phase)
		{
		case tlm::BEGIN_REQ:
		{
			// Handle BEGIN_REQ phase
			if (m_message)
			{
				LOG("(%s) BEGIN_REQ received\n", m_name.c_str());
			}
			switch (trans.get_command()) {
			case tlm::TLM_WRITE_COMMAND:
			{
				unsigned int wdata = 0;
				std::memcpy(&wdata, trans.get_data_ptr(), sizeof(wdata));
				regs.update_register(trans.get_address(), wdata);
				if (m_message)
				{
					LOG("(%s) Received transaction with address 0x%X, data: 0x%X\n", m_name.c_str(), trans.get_address(), wdata);
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
			LOG("(%s) END_REQ received\n", m_name.c_str());
			break;
		}
		default:
			break;
		}
		return tlm::TLM_ACCEPTED;
	}

	void binding_inout()
	{
		// binding signal to sub-model
		m_counter.clock(clock);
		m_counter.load(load);
		m_counter.clear(clear);
		m_counter.din(din);
		m_counter.dout(dout);
		m_counter.start(start);
	}

	void init_register()
	{
		regs.add_register("COUNTERINPUT", COUNTERINPUT, 0x00, 0x00ff, 0x0, READWRITE); // RW permission 
		regs.add_register("COUNTEROUTPUT", COUNTEROUTPUT, 0x00, 0x00ff, 0x0, READONLY); // R only
		/*Define callback*/
		regs.set_register_callback("COUNTERINPUT", std::bind(&wrapper_counter::cb_COUNTERINPUT, this,
			std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5));
	}

	/* Call back register */
	void cb_COUNTERINPUT(const std::string& name, uint32_t value, uint32_t old_value, uint32_t mask, uint32_t ch)
	{
		/* Register COUNTERINPUT definition
		*	bit 0 -> 7 : input din
		*/
		if (name == "COUNTERINPUT")
		{
			sc_dt::sc_uint<8> _din = value & 0xff;
			din.write(din);
		}
	}

	/* Method */

	void output_handling()
	{
		/* Register COUNTEROUTPUT definition
		*	bit 0 -> 7 : dout
		*/
		unsigned int output = dout.read();
		regs[COUNTEROUTPUT].set_readonly_value(output);
		if (m_message)
		{
			LOG("(%s) counter value: %d\n", m_name.c_str(), output);
		}
	}

	void input_handling()
	{
		load.write(m_load.read());
		clear.write(m_clear.read());
		clock.write(m_clk.read());
		start.write(m_start.read());
	}


public:
	tlm_utils::simple_target_socket<wrapper_counter, BUSWIDTH> target_socket;


	wrapper_counter(sc_core::sc_module_name name, bool message = false) :
		sc_core::sc_module(name)
		, m_name(name)
		, target_socket("target_socket")
		, m_message(message)
		, m_counter("Counter")
	{
		target_socket.register_nb_transport_fw(this, &wrapper_counter::nb_transport_fw);
		init_register();
		binding_inout();

		SC_METHOD(output_handling);
		sensitive << dout;

		SC_METHOD(input_handling);
		sensitive << m_load << m_clear << m_start << m_clk;

		load.write(true);
		clear.write(false);
		start.write(false);
	};
};

#endif
