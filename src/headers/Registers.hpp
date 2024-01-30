#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <unordered_map>

#include "../utils.hpp"
#include "./Qubit.hpp"

class c_Registers {
public:
    c_Registers();

    std::pair<std::string, u8> opcode_to_reg(u8 opcode);

    void set_reg(const std::string &reg_name, u64 value);

    u64 get_reg(const std::string &reg_name);

    void set_prgc(u32 address);

    u32 get_prgc();

    void set_sp(u32 value);

    u32 get_sp();

    void set_fp(u32 value);

    u32 get_fp();

    void dbg_log_regs_branch(unsigned int line_index);

private:
    std::unordered_map<std::string, std::pair<std::vector<u8>*, std::pair<int, int>>> regs;
    std::vector<std::vector<u8>> regs_mem;
    u32 prgc = 0x00000000;
    u32 sp = 0xFFFFFFFF;
    u32 fp = 0xFFFFFFFF;
    const std::vector<std::vector<std::string>> reg_names = {
        {"rax", "eax", "ax", "ah", "al"},
        {"rbx", "ebx", "bx", "bh", "bl"},
        {"rcx", "ecx", "cx", "ch", "cl"},
        {"rdx", "edx", "dx", "dh", "dl"},
    };
    std::unordered_map<u8, std::pair<std::string, int>> regs_opcode = {
        {0x5A, {"rax", 64}}, {0x4A, {"eax", 32}}, {0x3A, {"ax", 16}}, {0x2A, {"ah", 8}}, {0x1A, {"al", 8}},
        {0x5B, {"rbx", 64}}, {0x4B, {"ebx", 32}}, {0x3B, {"bx", 16}}, {0x2B, {"bh", 8}}, {0x1B, {"bl", 8}},
        {0x5C, {"rcx", 64}}, {0x4C, {"ecx", 32}}, {0x3C, {"cx", 16}}, {0x2C, {"ch", 8}}, {0x1C, {"cl", 8}},
        {0x5D, {"rdx", 64}}, {0x4D, {"edx", 32}}, {0x3D, {"dx", 16}}, {0x2D, {"dh", 8}}, {0x1D, {"dl", 8}},
    };
};

struct q_Register {
    std::vector<Qubit> qubits;

    void add_qubit(Qubit it);

    Qubit& get_qubit(unsigned int index); 

    int measure(unsigned int index);

    void apply_X(unsigned int index);

    void apply_H(unsigned int index);

    void apply_Y(unsigned int index);

    void apply_Z(unsigned int index);

    void apply_rz(unsigned int index, double phase);

    void apply_CNOT(unsigned int control_index, unsigned int target_index);

    void apply_crz(
        unsigned int control_index, 
        unsigned int target_index, 
        double phase
    );

    void reset(unsigned int index);

    void dbg_log_qubits();
};

class q_Registers {
public:
    std::unordered_map<u16, q_Register> qregs;

    void add_qreg(u16 number, u8 qubit_count);

    q_Register& get_qreg(u16 number);

    void dbg_log_qregs();
};
