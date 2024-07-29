#pragma once
#ifndef _ADDER_WRAPPER_H
#define _ADDER_WRAPPER_H

#include "inc/adder_four_bit_adder.h"
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

#define ADDERINPUT	(0x00 + 0x00)
#define ADDEROUTPUT (0x00 + 0x04)


template<unsigned int BUSWIDTH = 32>
class wrapper_four_bit_adder : sc_module
{
private:
	std::string m_name;
	four_bit_adder m_four_bit_adder;
	RegisterInterface regs;
	bool m_message;

private:
	/* 4-bit adder interface*/
	sc_signal<sc_uint<4> >	sum;
	sc_signal<bool>	cout;
	sc_signal<sc_uint<4> >	a;
	sc_signal<sc_uint<4> >	b;
	sc_signal<bool>	cin;

private:
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
			std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << m_name << " END_REQ received" << std::endl;
			break;
		}
		default:
			break;
		}
		return tlm::TLM_ACCEPTED;
	}

	void binding_inout()
	{
		m_four_bit_adder.a(a);
		m_four_bit_adder.b(b);
		m_four_bit_adder.cin(cin);
		m_four_bit_adder.sum(sum);
		m_four_bit_adder.cout(cout);
	}

	void init_register()
	{
		regs.add_register("ADDERINPUT", ADDERINPUT, 0x00, 0x8f0f, 0x0);
		regs.add_register("ADDEROUTPUT", ADDEROUTPUT, 0x00, 0x800f, 0x0);
		/*Define callback*/
		regs.set_register_callback("ADDERINPUT", std::bind(&wrapper_four_bit_adder::cb_ADDERINPUT, this,
			std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5));
	}

	/* Call back register */
	void cb_ADDERINPUT(const std::string& name, uint32_t value, uint32_t old_value, uint32_t mask, uint32_t ch)
	{
		/* Register ADDERINPUT definition
		*	bit 0 -> 4 : input a
		*   bit 8 -> 12: input b
		*   bit 15 -> 16: input cin
		*/
		if (name == "ADDERINPUT")
		{
			sc_dt::sc_uint<4> _a = value & 0x0f;
			sc_dt::sc_uint<4> _b = (value >> 8) & 0x0f;
			bool _cin = (value >> 15) & 0x01;
			a.write(_a);
			b.write(_b);
			cin.write(_cin);
		}

	}

	/* Method */

	void output_handling()
	{
		/* Register ADDEROUTPUT definition
		*	bit 0 -> 4 : sum
		*   bit 15 -> 16: cout
		*/
		unsigned long int _cout = cout.read();
		regs[ADDEROUTPUT] = (_cout << 15) | sum.read();
	}

public:
	tlm_utils::simple_target_socket<wrapper_four_bit_adder, BUSWIDTH> target_socket;


	wrapper_four_bit_adder(sc_core::sc_module_name name, bool message = false) :
		sc_core::sc_module(name)
		, m_name(name)
		, target_socket("target_socket")
		, m_four_bit_adder("four_bit_adder")
		, m_message(message)
	{
		target_socket.register_nb_transport_fw(this, &wrapper_four_bit_adder::nb_transport_fw);
		init_register();
		binding_inout();

		SC_METHOD(output_handling);
		sensitive << sum << cout;

	};
};

#endif
