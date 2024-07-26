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
#include "common/bus.h"
#include "common/DummyMaster.h"
#include "common/DummySlave.h"
#include "common/target.h"
#include "common/memory.h"
#include "common/DMAC.h"

class SoCPlatform : public sc_core::sc_module
{
public:
	SoCPlatform(sc_core::sc_module_name name) :
		sc_core::sc_module(name)
		, m_dummymaster("DummyMaster")
		, m_dmac("Dmac")
		, m_bus("bus_mmio", false)
		, sysclk("sysclk", 10, sc_core::SC_PS)
		, m_target1("Target1")
		, m_target2("Target2")
		, m_target3("Target3")
		, m_target4("Target4")
		, m_ram("ram1", 0x3000)
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
		m_bus.mapping_target_sockets(0, 0x0000, 0x3000).bind(m_ram.socket);
		m_bus.mapping_target_sockets(1, 0x3000, 0x1000).bind(m_dmac.target_socket);
		m_bus.mapping_target_sockets(2, 0x4000, 0x1000).bind(m_target1.target_socket);
		m_bus.mapping_target_sockets(3, 0x5000, 0x1000).bind(m_target2.target_socket);
		m_bus.mapping_target_sockets(4, 0x6000, 0x1000).bind(m_target3.target_socket);
		m_bus.mapping_target_sockets(5, 0x7000, 0x1000).bind(m_target4.target_socket);

	}

	void do_signals_binding()
	{
		// binding system clock
		m_bus.m_clk(sysclk);
		m_dmac.clk.bind(sysclk);
		m_dummymaster.clk.bind(sysclk);

		// binding reset
		m_dummymaster.rst.bind(rst);
		m_bus.rst.bind(rst);
		m_dmac.rst.bind(rst);
		
		for (unsigned int i = 0; i < 256; i++)
		{
			m_dmac.DMA_req[i].bind(dma_req[i]);
			m_dmac.DMA_int[i].bind(dma_int[i]);
			m_dmac.DMA_ack[i].bind(dma_ack[i]);
		}
	}

public: // public models
	DummyMaster<32> m_dummymaster;

public: // Private models
	BUS<APB,6> m_bus;
	DMAC<32> m_dmac;
	Target<32> m_target1;
	Target<32> m_target2;
	Target<32> m_target3;
	Target<32> m_target4;
	RAM<32> m_ram;

public: // Define signals

	sc_core::sc_clock sysclk;
	sc_core::sc_signal<bool> rst;
	sc_core::sc_signal<bool> dma_req[256];
	sc_core::sc_signal<bool> dma_ack[256];
	sc_core::sc_signal<bool> dma_int[256];
};
#endif