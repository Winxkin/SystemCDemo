/*
 *	Author: Huan Nguyen-Duy
 *  Date: 2/08/2024
 */

#ifndef _DATA_MEM_H
#define _DATA_MEM_H

#include <systemc>
#include <map>
#include <vector>
#include <iostream>
#include <list>
#include <cstdint>

class Memory : public sc_core::sc_module
{
private:
	unsigned char* data;
	sc_dt::uint64 size;
	std::uint32_t address_count;
public:
	Memory(sc_core::sc_module_name name, sc_dt::uint64 size) : sc_module(name), size(size), address_count(0)
	{
		data = new unsigned char[size];
		std::memset(data, 0, size);
	}

	void load_instruction(std::uint32_t _inst)
	{
		set_mem(address_count, _inst);
		address_count = address_count + 4;
	}

	// Method to set memory (write)
	void set_mem(std::uint32_t _offset, std::uint32_t value) {
		if (_offset + sizeof(std::uint32_t) <= size) {
			std::memcpy(&data[_offset], &value, sizeof(std::uint32_t));
		}
		else {
			SC_REPORT_ERROR("Memory", "Offset out of bounds in set_mem()");
		}
	}

	// Method to get memory (read)
	std::uint32_t get_mem(std::uint32_t _offset) const {
		std::uint32_t value = 0;
		if (_offset + sizeof(std::uint32_t) <= size) {
			std::memcpy(&value, &data[_offset], sizeof(std::uint32_t));
		}
		else {
			SC_REPORT_ERROR("Memory", "Offset out of bounds in get_mem()");
		}
		return value;
	}

	// Method to set a 32-bit word (4 bytes) in memory
	void set_word(std::uint32_t _offset, std::uint32_t value) {
		if (_offset + sizeof(std::uint32_t) <= size) {
			std::memcpy(&data[_offset], &value, sizeof(std::uint32_t));
		}
		else {
			SC_REPORT_ERROR("Memory", "Offset out of bounds in set_word()");
		}
	}

	// Method to get a 32-bit word (4 bytes) from memory
	std::uint32_t get_word(std::uint32_t _offset) const {
		std::uint32_t value = 0;
		if (_offset + sizeof(std::uint32_t) <= size) {
			std::memcpy(&value, &data[_offset], sizeof(std::uint32_t));
		}
		else {
			SC_REPORT_ERROR("Memory", "Offset out of bounds in get_word()");
		}
		return value;
	}


	// Method to set a 16-bit half-word (2 bytes) in memory
	void set_half(std::uint32_t _offset, uint16_t value) {
		if (_offset + sizeof(uint16_t) <= size) {
			std::memcpy(&data[_offset], &value, sizeof(uint16_t));
		}
		else {
			SC_REPORT_ERROR("Memory", "Offset out of bounds in set_half()");
		}
	}

	// Method to get a 16-bit half-word (2 bytes) from memory
	uint16_t get_half(std::uint32_t _offset) const {
		uint16_t value = 0;
		if (_offset + sizeof(uint16_t) <= size) {
			std::memcpy(&value, &data[_offset], sizeof(uint16_t));
		}
		else {
			SC_REPORT_ERROR("Memory", "Offset out of bounds in get_half()");
		}
		return value;
	}

	// Method to set an 8-bit byte in memory
	void set_byte(std::uint32_t _offset, uint8_t value) {
		if (_offset + sizeof(uint8_t) <= size) {
			data[_offset] = value;
		}
		else {
			SC_REPORT_ERROR("Memory", "Offset out of bounds in set_byte()");
		}
	}

	// Method to get an 8-bit byte from memory
	uint8_t get_byte(std::uint32_t _offset) const {
		if (_offset + sizeof(uint8_t) <= size) {
			return data[_offset];
		}
		else {
			SC_REPORT_ERROR("Memory", "Offset out of bounds in get_byte()");
			return 0;
		}
	}


	~Memory() {
		delete[] data;
	}

};

#endif
