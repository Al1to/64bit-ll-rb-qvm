#pragma once

#include <string>

#include "../utils.hpp"
#include "./Memory.hpp"
#include "./Registers.hpp"
#include "./QPU.hpp"

enum instr_type {
    reg_reg,
    reg_lit,
    reg_addr,
    reg_addr_in_reg,
};

class QVM {
public:
    QVM(unsigned int &mem_size, std::string &qasm_code) 
    : qasm_code(qasm_code), cregs(), mem(mem_size, cregs), qpu(mem, cregs) {};

    void run();

private:
    std::string qasm_code;
    c_Registers cregs;
    Memory mem;
    QPU qpu;

    void load_to_mem();

    instr_type check_instr_type(unsigned long long &cur);
};
