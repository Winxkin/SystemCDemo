/*
 *	Author: Huan Nguyen-Duy
 *  Date: 26/07/2024
 */

#ifndef _TOP_H
#define _TOP_H
#include <systemc>
#include <tlm>
#include <iostream>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include "bus.h"
#include "DummyMaster.h"
#include "DummySlave.h"
#include "target.h"
#include "memory.h"
#include "DMAC.h"
#include "IO.h"
#include "Inverter.h"
#include "Adder/Adder_wrapper.h"
#include "Counter/Counter_wrapper.h"
#include "Uart/Uart_wrapper.h"


class SoCPlatform : public sc_core::sc_module
{
public:
	SoCPlatform(sc_core::sc_module_name name) :
		sc_core::sc_module(name)
		, m_dummymaster("DummyMaster", true)
		, m_dummyslave("DummySlave", true)
		, m_dmac("Dmac", true)
		, m_bus("bus_mmio", true)
		, m_sysclk("systemCLK", 10, sc_core::SC_NS)	// 100MHz 
		, m_target1("Target1")
		, m_target2("Target2")
		, m_target3("Target3")
		, m_target4("Target4")
		, m_ram1("ram1", 0x3000, true)
		, m_wrapper_four_bit_adder("wrapper_four_bit_adder", true)
		, m_wrapper_counter("wrapper_counter", true)
		, m_wrapper_uart("wrapper_uart", true)
		{
			do_signals_binding();
			do_bus_binding();
		}

private:

	void do_bus_binding()
	{
		// Connecting Initiator socket to bus MMIO
		m_dummymaster.initiator_socket.bind(m_bus.target_sockets);
		m_dmac.initiator_socket.bind(m_bus.target_sockets);

		// Binding target sockets into bus MMIO
		m_bus.mapping_target_sockets(BASE_RAM1,	SIZE_RAM1).bind(m_ram1.socket);
		m_bus.mapping_target_sockets(BASE_DUMMYSLAVE, SIZE_DUMMYSLAVE).bind(m_dummyslave.target_socket);
		m_bus.mapping_target_sockets(BASE_DMAC,	SIZE_DMAC).bind(m_dmac.target_socket);
		m_bus.mapping_target_sockets(BASE_TARGET1, SIZE_TARGET1).bind(m_target1.target_socket);
		m_bus.mapping_target_sockets(BASE_TARGET2, SIZE_TARGET2).bind(m_target2.target_socket);
		m_bus.mapping_target_sockets(BASE_TARGET3, SIZE_TARGET3).bind(m_target3.target_socket);
		m_bus.mapping_target_sockets(BASE_TARGET4, SIZE_TARGET4).bind(m_target4.target_socket);
		m_bus.mapping_target_sockets(BASE_FOUR_BIT_ADDER, SIZE_FOUR_BIT_ADDER).bind(m_wrapper_four_bit_adder.target_socket);
		m_bus.mapping_target_sockets(BASE_COUNTER, SIZE_COUNTER).bind(m_wrapper_counter.target_socket);
		m_bus.mapping_target_sockets(BASE_UART, SIZE_UART).bind(m_wrapper_uart.target_socket);
	}

	void do_signals_binding()
	{
		m_dummyslave.add_input_port("system_reset")->bind(m_sysrst);
		// binding system clock
		m_bus.m_clk(m_sysclk);
		m_dmac.clk.bind(m_sysclk);
		m_dummymaster.clk.bind(m_sysclk);
		m_dummyslave.clk.bind(m_sysclk);
		m_dummyslave.rst(m_sysrst);
		m_wrapper_counter.m_clk(m_sysclk);
		m_wrapper_uart.m_clk(m_sysclk);

		// binding reset
		m_dummymaster.rst.bind(m_sysrst);
		m_bus.m_rst.bind(m_sysrst);
		m_dmac.rst.bind(m_sysrst);
		m_wrapper_uart.m_rst(m_sysrst);
		
		for (unsigned int i = 0; i < 256; i++)
		{
			m_dmac.DMA_req[i].bind(dma_req[i]);
			m_dmac.DMA_int[i].bind(dma_int[i]);
			m_dmac.DMA_ack[i].bind(dma_ack[i]);

			// add ports to dummy slave
			m_dummyslave.add_input_port("DMA_req" + std::to_string(i))->bind(dma_req[i]);
			m_dummyslave.add_input_port("DMA_int" + std::to_string(i))->bind(dma_int[i]);
			m_dummyslave.add_input_port("DMA_ack" + std::to_string(i))->bind(dma_ack[i]);
			m_dummyslave.add_output_port("DMA_req" + std::to_string(i))->bind(dma_req[i]);
		}

		/* Counter */
		m_wrapper_counter.m_clear(counter_clear);
		m_wrapper_counter.m_load(counter_load);
		m_wrapper_counter.m_start(counter_start);
		m_dummyslave.add_output_port("counter_clear")->bind(counter_clear);
		m_dummyslave.add_output_port("counter_load")->bind(counter_load);
		m_dummyslave.add_output_port("counter_start")->bind(counter_start);

		/* UART */
		m_wrapper_uart.m_nrw(uart_nrw);
		m_wrapper_uart.m_cs(uart_cs);
		m_wrapper_uart.m_sin(uart_sin);
		m_wrapper_uart.m_int2(uart_int2);
		m_wrapper_uart.m_sout(uart_sout);
		m_dummyslave.add_output_port("uart_nrw")->bind(uart_nrw);
		m_dummyslave.add_output_port("uart_cs")->bind(uart_cs);
		m_dummyslave.add_output_port("uart_sin")->bind(uart_sin);
		m_dummyslave.add_input_port("uart_int2")->bind(uart_int2);
		m_dummyslave.add_input_port("uart_sout")->bind(uart_sout);

		
		
	}

public:

	// RAM model

	/**@brief
	 * dump_memory
	 *
	 * Show memory regions
	 * @param offset The ram offset
	 * @param len The ram size
	 */
	void dump_memory(const std::string& name, sc_dt::uint64 offset, unsigned int len)
	{
		if (name == m_ram1.get_name())
		{
			m_ram1.dump_memory(offset, len);
		}
	}

	// DummySlave model

	/**@brief
	* monitor_ports
	*
	* Using to enable or disable port monitor operation
	* @param is_enable Indicating whether enabling or disabling port monitor
	*/
	void monitor_ports(bool is_enable)
	{
		m_dummyslave.monitor_ports(is_enable);
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
		m_dummyslave.set_output_ports(name, value);
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
		m_dummyslave.trigger_output_ports(name, high_level, is_pos);
	}

	/**@brief
	* read_input_ports
	*
	* Using to read the value of specific port
	* @param name Reference to the port name
	*/
	bool read_input_ports(const std::string& name)
	{
		return m_dummyslave.read_input_ports(name);
	}

	// DummyMaster model

	/**@brief
	 * get_received_data
	 *
	 * Using to get data from the returned transaction
	 *
	 * @return the array of data
	 *
	 */
	unsigned char* get_received_data()
	{
		return m_dummymaster.get_received_data();
	}

	/**@brief
	 * get_received_data
	 *
	 * Using to get data from the returned transaction
	 *
	 * @return the 32-bit data
	 *
	 */
	unsigned int get_received_32bit_data()
	{
		return m_dummymaster.get_received_32bit_data();
	}

	/**@brief
	 * SentTransaction
	 *
	 * Implements the sent a transaction with 32 bit data to bus MMIO
	 *
	 * @param addr  Reference to the address of the slave
	 *
	 * @param data	Reference to 32-bit data
	 *
	 * @param cmd   Reference to tlm command
	 *				-TLM_READ_COMMAND  : TLM read requset
	 *				-TLM_WRITE_COMMAND : TLM write requset
	 *				-TLM_IGNORE_COMMAND: TLM ignore requset
	 */
	void SentTransaction(unsigned int addr, uint32_t data, tlm::tlm_command cmd)
	{
		m_dummymaster.SentTransaction(addr, data, cmd);
	}

	/**@brief
	 * Sentcustomtransaction
	 *
	 * Implements the sent a custom transaction to bus MMIO
	 *
	 * @param addr  Reference to the address of the slave
	 *
	 * @param data	Reference to the pointer of array data
	 *
	 * @param data_length Reference to the length of data
	 *
	 * @param cmd   Reference to tlm command
	 *				-TLM_READ_COMMAND  : TLM read requset
	 *				-TLM_WRITE_COMMAND : TLM write requset
	 *				-TLM_IGNORE_COMMAND: TLM ignore requset
	 */
	void Sentcustomtransaction(unsigned int addr, unsigned char* data, unsigned int data_length, tlm::tlm_command cmd)
	{
		m_dummymaster.Sentcustomtransaction(addr, data, data_length, cmd);
	}


private: // Private models
	DummyMaster<32> m_dummymaster;
	DummySlave<32> m_dummyslave;
	BUS<APB> m_bus;
	DMAC<32> m_dmac;
	Target<32> m_target1;
	Target<32> m_target2;
	Target<32> m_target3;
	Target<32> m_target4;
	RAM<32> m_ram1;
	wrapper_four_bit_adder<32> m_wrapper_four_bit_adder;
	wrapper_counter<32> m_wrapper_counter;
	wrapper_uart<32> m_wrapper_uart;

private: // Define signals

	sc_core::sc_clock m_sysclk;
	sc_core::sc_signal<bool> m_sysrst;

	/* DMAC */
	sc_core::sc_signal<bool> dma_req[256];
	sc_core::sc_signal<bool> dma_ack[256];
	sc_core::sc_signal<bool> dma_int[256];

	/* Counter */
	sc_core::sc_signal<bool> counter_load;
	sc_core::sc_signal<bool> counter_clear;
	sc_core::sc_signal<bool> counter_start;

	/* UART */
	sc_core::sc_signal<bool>	uart_nrw;
	sc_core::sc_signal<bool>	uart_cs;
	sc_core::sc_signal<bool>	uart_sin;

	sc_core::sc_signal<bool>	uart_int2;
	sc_core::sc_signal<bool>	uart_sout;

};
#endif