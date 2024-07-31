/*
 * Author: Huan Nguyen-Duy
 * date: 01/07/2024
 */
#ifndef _REGISTERIF_H
#define _REGISTERIF_H
#include <unordered_map>
#include <map>
#include <string>
#include <cstdint>
#include <stdexcept>
#include <memory>
#include <stdio.h>
#include <iostream>

typedef enum
{
  READWRITE = 0,
  READONLY = 1
} REGPERMISSION;

class Register {
public:

    using Callback = std::function<void(const std::string&, uint32_t, uint32_t, uint32_t, uint32_t)>;

    Register() : name(""), address(0), value(0), mask(0), ch(0), init_val(0), permission(READWRITE) {};

    Register(std::string name, uint64_t address, uint32_t init, uint32_t mask, uint32_t ch, REGPERMISSION permission)
        : name(name), address(address), value(init), mask(mask), ch(ch), init_val(init), permission(permission)
    {
    };

    ~Register() {};

    uint32_t get_value() const { return value; };

    void set_value(uint32_t new_value)
    {
        uint32_t old_value = value;
        if (permission == READWRITE)
        {
            value = new_value & mask;

            /* Info message */
            std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << "    Register " << name
                << " changed to value -> [0x" << std::hex << value << "]" << std::endl;

            /* Calling callback register */
            if (callback) {
                callback(name, value, old_value, mask, ch);
            }
        }
        else
        {
            std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << "    Register " << name << " Warning: only read permission" << std::endl;
        }
    };

    void set_readonly_value(uint32_t new_value)
    {
        if (permission == READONLY)
        {
            value = new_value & mask;
        }
    }


    void reset()
    {
        value = init_val;
    }

    uint64_t get_address() { return address; };

    std::string get_name() { return name; };

    Register& operator=(uint32_t new_value) {
        uint32_t old_value = value;
        if (permission == READWRITE)
        {
            value = new_value & mask;
            /* Info message */
            std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << "    Register " << name
                << " changed to value -> [0x" << std::hex << value << "]" << std::endl;

            /* Calling callback register */
            if (callback) {
                callback(name, value, old_value, mask, ch);
            }
        }
        else
        {
            std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" << "    Register " << name << " Warning: only read permission" << std::endl;
        }
        return *this;
    }

    void set_callback(Callback cb) {
        callback = cb;
    }

private:
    std::string name;
    uint64_t address;
    uint32_t value;
    uint32_t mask;  // Using to perform read or write permission 
    const uint32_t ch;
    const uint32_t init_val;
    const REGPERMISSION permission;
    Callback callback;
};

// Class to manage a collection of registers
class RegisterInterface {
public:
    void add_register(std::string name, uint64_t address, uint32_t init, uint32_t mask, uint32_t ch, REGPERMISSION permission) {
        registers.emplace(name, Register(name, address, init, mask, ch, permission));
        /*
        std::cout << "[Adding new register]   Register name: [" << registers[name].get_name()
            << "], address [" << std::hex << registers[name].get_address()
            << "], initialize value [" << std::hex << registers[name].get_value() << "]" << std::endl;
        */
    };

    // The operator to get register by name for example this->reg[name]
    Register& operator[](std::string name) {
        if (registers.find(name) == registers.end()) {
            throw std::runtime_error("Register not found");
        }
        return registers[name];
    };

    // The operator to get register by address for example this->reg[addr]
    Register& operator[](uint64_t address)
    {
        for (auto& reg : registers) {
            if (reg.second.get_address() == address) {
                return registers[reg.first];
            }
        }
        throw std::runtime_error("Register with the given address not found");
    }


    void update_register(uint64_t address, uint32_t value) {
        for (auto& reg : registers) {
            if (reg.second.get_address() == address) {
                reg.second = value;
                return;
            }
        }
        throw std::runtime_error("Register with the given address not found");
    };

    void reset_regs()
    {
        for (auto& reg : registers)
        {
            reg.second.reset();
        }
    }

    void dump_registers()
    {
        for (auto& reg : registers)
        {
            std::cout << "[" << sc_core::sc_time_stamp().to_double() << " NS ]" "[dump]  Register " << reg.second.get_name()
                << ", address: [" << std::hex << reg.second.get_address()
                << "] value: [" << std::hex << reg.second.get_value() << "]" << std::endl;
        }

    };

    void set_register_callback(const std::string& name, Register::Callback cb) {
        if (registers.find(name) == registers.end()) {
            throw std::runtime_error("Register not found");
        }
        registers[name].set_callback(cb);
    }

private:
    std::map<std::string, Register> registers;
};



#endif