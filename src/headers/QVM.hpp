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
    addr_reg,
    addr_lit,
    addr_in_reg_reg,
    addr_in_reg_lit,
};

class QVM {
public:
    QVM(unsigned int &mem_size, std::string &qasm_code) 
    : qasm_code(qasm_code), cregs(), qregs(), mem(mem_size, cregs), qpu(mem, cregs, qregs) {};

    void run();

private:
    std::string qasm_code;
    c_Registers cregs;
    q_Registers qregs;
    Memory mem;
    QPU qpu;

    void load_to_mem();

    instr_type check_instr_type(unsigned long long &cur);
};
