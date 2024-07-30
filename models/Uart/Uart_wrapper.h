#pragma once
#ifndef _UART_WRAPPER_H
#define _UART_WRAPPER_H

#include "inc/uart_top_uart_top.h"
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

#define UARTINPUT	(0x00 + 0x00)
#define UARTOUTPUT	(0x00 + 0x04)

template<unsigned int BUSWIDTH = 32>
class wrapper_uart : sc_module
{
private:
	std::string m_name;
	RegisterInterface regs;
	bool m_message;
	uart_top m_uart;

private:
	/* UART interface */
	// input
	sc_signal<sc_uint<3> >	addr;
	sc_signal<bool>	clk;
	sc_signal<bool>	cs;
	sc_signal<sc_uint<8> >	data_in;
	sc_signal<bool>	nrw;
	sc_signal<bool>	rst;
	sc_signal<bool>	sin;

	// output
	sc_signal<sc_uint<8> >	data_out;
	sc_signal<bool>	int2;
	sc_signal<bool>	sout;
public:
	/* Wrapper uart interface */
	sc_in<bool>	m_clk;
	sc_in<bool>	m_rst;
	sc_in<bool>	m_nrw;
	sc_in<bool>	m_cs;
	sc_in<bool>	m_sin;

	sc_out<bool>	m_int2;
	sc_out<bool>	m_sout;
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
		// in
		m_uart.addr(addr);
		m_uart.clk(clk);
		m_uart.cs(cs);
		m_uart.data_in(data_in);
		m_uart.nrw(nrw);
		m_uart.rst(rst);
		m_uart.sin(rst);
		//out
		m_uart.data_out(data_out);
		m_uart.int2(int2);
		m_uart.sout(sout);
	}

	void init_register()
	{
		regs.add_register("UARTINPUT", UARTINPUT, 0x00, 0x07ff, 0x0); // RW permission 
		regs.add_register("UARTOUTPUT", UARTOUTPUT, 0x00, 0x0000, 0x0); // R only
		regs.set_register_callback("UARTINPUT", std::bind(&wrapper_uart::cb_UARTINPUT, this,
			std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5));
	}

	/* Call back register */
	void cb_UARTINPUT(const std::string& name, uint32_t value, uint32_t old_value, uint32_t mask, uint32_t ch)
	{
		/* Register COUNTERINPUT definition
		*	bit 0 -> 7 : input data_in
		*   bit 8 -> 10 : input addr
		*/
		sc_dt::sc_uint<8> _din = value & 0xff;
		data_in.write(_din);
		sc_dt::sc_uint<3> _addr = (value >> 8) & 0x07;
		addr.write(_addr);
	}

	/* Method for input and output handling */

	void regs_output_handling()
	{
		/* Register UARTOUTPUT definition
		*	bit 0 -> 7 : input data_out
		*/
		unsigned int _output = data_out.read();
		regs[UARTOUTPUT].set_value(_output, true); // To set directly value into register that is not apply mask
	}

	void output_handling()
	{
		m_int2.write(int2.read());
		m_sout.write(sout.read());
	}

	void input_handling()
	{
		clk.write(m_clk.read()); // clock synchronization
		rst.write(!m_rst.read()); // negative reset
		nrw.write(m_nrw.read());
		cs.write(m_cs.read());
		sin.write(m_sin.read());
	}


public:
	tlm_utils::simple_target_socket<wrapper_uart, BUSWIDTH> target_socket;


	wrapper_uart(sc_core::sc_module_name name, bool message = false) :
		sc_core::sc_module(name)
		, m_name(name)
		, target_socket("target_socket")
		, m_message(message)
		, m_uart("uart")
	{
		target_socket.register_nb_transport_fw(this, &wrapper_uart::nb_transport_fw);
		init_register();
		binding_inout();
		
		SC_METHOD(regs_output_handling);
		sensitive << data_out;

		SC_METHOD(output_handling);
		sensitive << int2 << sout;
	
		SC_METHOD(input_handling);
		sensitive << m_clk << m_rst << m_nrw << m_cs << m_sin;
	};
};

#endif
