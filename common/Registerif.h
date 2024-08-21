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
#include "commondef.h"

typedef enum
{
  READWRITE = 0,
  READONLY = 1
} REGPERMISSION;

class Register {
public:

    using Callback = std::function<void(const std::string&, uint32_t, uint32_t, uint32_t, uint32_t)>;

    /**@brief
    * Register
    *
    * Default Constructure (this function does not refer to use)
    */
    Register() : name(""), address(0), value(0), mask(0), ch(0), init_val(0), permission(READWRITE) {};

    /**@brief
    * Register
    *
    * Constructure
    * @param name  The register name
    * @param address  The register base address
    * @param init The register initialization value
    * @param mask The register mask
    * @param ch The register channel
    * @param permission Indicating this register is read /write or read only permission
    */
    Register(std::string name, uint64_t address, uint32_t init, uint32_t mask, uint32_t ch, REGPERMISSION permission)
        : name(name), address(address), value(init), mask(mask), ch(ch), init_val(init), permission(permission)
    {
    };

    /**@brief
    * ~Register
    * Destructure
    */
    ~Register() {};

    /**@brief
    * get_value
    * Return the register value
    */
    uint32_t get_value() const { return value; };

    /**@brief
    * set_value
    * Using to set register value
    * @param new_value The new value that is wrote into this register
    */
    void set_value(uint32_t new_value)
    {
        uint32_t old_value = value;
        if (permission == READWRITE)
        {
            value = new_value & mask;

            /* Info message */
            LOG("Register %s changed to value -> [0x%X]\n", name.c_str(), value);

            /* Calling callback register */
            if (callback) {
                callback(name, value, old_value, mask, ch);
            }
        }
        else
        {
            LOG("Register %s Warning: only read permission\n", name.c_str());
        }
    };

    /**@brief
   * set_value
   * Using for developer to set read only register value
   * @param new_value The new value that is wrote into this register
   */
    void set_readonly_value(uint32_t new_value)
    {
        if (permission == READONLY)
        {
            value = new_value & mask;
        }
    }

    /**@brief
    * reset
    * To reset register to initialization value
    */
    void reset()
    {
        value = init_val;
    }

    /**@brief
    * get_address
    * Return the register base address
    */
    uint64_t get_address() { return address; };

    /**@brief
    * get_name
    * Return the register name
    */
    std::string get_name() { return name; };

    /**@brief
    * operator=
    * To set new value for the rigster
    * @param new_value The new value that is wrote into this register
    */
    Register& operator=(uint32_t new_value) {
        uint32_t old_value = value;
        if (permission == READWRITE)
        {
            value = new_value & mask;
            /* Info message */
            LOG("Register %s changed to value -> [0x%X]\n", name.c_str(), value);

            /* Calling callback register */
            if (callback) {
                callback(name, value, old_value, mask, ch);
            }
        }
        else
        {
            LOG("Register %s Warning: only read permission\n", name.c_str());
        }
        return *this;
    }

    /**@brief
    * operator=
    * Using to register call back function for the register
    * @param cb The address of call back function
    */
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

    /**@brief
    * add_register
    *
    * Constructure
    * @param name  The register name
    * @param address  The register base address
    * @param init The register initialization value
    * @param mask The register mask
    * @param ch The register channel
    * @param permission Indicating this register is read /write or read only permission
    */
    void add_register(std::string name, uint64_t address, uint32_t init, uint32_t mask, uint32_t ch, REGPERMISSION permission) {
        registers.emplace(name, Register(name, address, init, mask, ch, permission));
        /*
        std::cout << "[Adding new register]   Register name: [" << registers[name].get_name()
            << "], address [" << std::hex << registers[name].get_address()
            << "], initialize value [" << std::hex << registers[name].get_value() << "]" << std::endl;
        */
    };

    /**@brief
    * add_register
    * The operator to get register by name for example this->reg[name]
    * @param name  The register name
    * @return Register object with corresponding name
    */
    Register& operator[](std::string name) {
        if (registers.find(name) == registers.end()) {
            throw std::runtime_error("Register not found");
        }
        return registers[name];
    };

    
    /**@brief
    * operator[]
    * The operator to get register by address for example this->reg[addr]
    * @param address  The register address
    * @return Register object with corresponding address
    */
    Register& operator[](uint64_t address)
    {
        for (auto& reg : registers) {
            if (reg.second.get_address() == address) {
                return registers[reg.first];
            }
        }
        throw std::runtime_error("Register with the given address not found");
    }


    /**@brief
   * update_register
   * Using to update the value of register with specific address
   * @param address  The register address
   * @return value The new value that is wrote into this register
   */
    void update_register(uint64_t address, uint32_t value) {
        for (auto& reg : registers) {
            if (reg.second.get_address() == address) {
                reg.second = value;
                return;
            }
        }
        throw std::runtime_error("Register with the given address not found");
    };

    /**@brief
   * update_register
   * Using to reset registers
   */
    void reset_regs()
    {
        for (auto& reg : registers)
        {
            reg.second.reset();
        }
    }

    /**@brief
    * update_register
    * Using to show all register informations
    */
    void dump_registers()
    {
        for (auto& reg : registers)
        {
            LOG("[dump]  Register %s, address: [0x%lX], value: [0x%lX]\n", reg.second.get_name().c_str(), reg.second.get_address(), reg.second.get_value());
        }

    };

    /**@brief
    * update_register
    * Using to register register callback function
    * @param name The register name
    * @param cb The address of call back function
    */
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