#pragma once

#include <unordered_map>

#include "../utils.hpp"
#include "./Registers.hpp"

class Memory {
public:
    Memory(unsigned int mem_size_in_bytes, c_Registers &cregs) : cregs(cregs) {
        mem.resize(mem_size_in_bytes, 0);
    };

    void write(u32 address, u8 byte);

    u8 read(u32 address);

    u8 fetch8();

    u16 fetch16();

    u32 fetch32();
    
    u64 fetch64();

    int mem_len();

    void add_data(u16 number, u32 prgc);

    u32 get_data_prgc(u16 number);

    void add_label(u16 number, u32 prgc);

    u32 get_label_prgc(u16 number);

    void push_to_stack(u64 value);

    u64 pop_from_stack();
    
    void enter_frame();

    void exit_frame();

    void dbg_log_mem();

    void dbg_log_stack();

private:
    std::vector<u8> mem;
    std::vector<u8> stack;
    c_Registers& cregs;
    std::unordered_map<u16, u32> data_addresses;
    std::unordered_map<u16, u32> labels_addresses;
};
