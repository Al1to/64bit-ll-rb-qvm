#include "../headers/Memory.hpp"
#include "../headers/Registers.hpp"

void Memory::write(u32 address, u8 byte) {
    mem[address] = byte;
}

u8 Memory::read(u32 address) {
    return mem[address];
}

u8 Memory::fetch8() {
    u32 prgc = cregs.get_prgc();

    u8 value = mem[prgc];

    cregs.set_prgc(prgc + 1);
    return value;
}

u16 Memory::fetch16() {
    u32 prgc = cregs.get_prgc();

    u16 value = mem[prgc + 1] 
    | (mem[prgc] << 8);

    cregs.set_prgc(prgc + 2);
    return value;
}

u32 Memory::fetch32() {
    u32 prgc = cregs.get_prgc();

    u32 value = mem[prgc + 3] 
    | (mem[prgc + 2] << 8) 
    | (mem[prgc + 1] << 16) 
    | (mem[prgc] << 24);

    cregs.set_prgc(prgc + 4);
    return value;
}

u64 Memory::fetch64() {
    u32 prgc = cregs.get_prgc();

    u64 value = mem[prgc + 7] 
    | ((u64)mem[prgc + 6] << 8) 
    | ((u64)mem[prgc + 5] << 16) 
    | ((u64)mem[prgc + 4] << 24) 
    | ((u64)mem[prgc + 3] << 32) 
    | ((u64)mem[prgc + 2] << 40) 
    | ((u64)mem[prgc + 1] << 48) 
    | ((u64)mem[prgc] << 56);

    cregs.set_prgc(prgc + 8);
    return value;
}

int Memory::mem_len() {
    return mem.size();
}

void Memory::add_data(u16 number, u32 prgc) {
    data_addresses[number] = prgc;
}

u32 Memory::get_data_prgc(u16 number) {
    if (data_addresses.find(number) == data_addresses.end()) {
        throw std::runtime_error("ERR: data number " + std::to_string(number) + " not found");
    }

    return data_addresses[number];
}

void Memory::add_label(u16 number, u32 prgc) {
    labels_addresses[number] = prgc;
}

u32 Memory::get_label_prgc(u16 number) {
    if (labels_addresses.find(number) == labels_addresses.end()) {
        throw std::runtime_error("ERR: label number " + std::to_string(number) + " not found");
    }

    return labels_addresses[number];
}

void Memory::push_to_stack(u64 value) {
    for (int i = 56; i >= 0; i -= 8) {
        stack.push_back((value >> i) & 0xFF);
    }
    u32 sp = cregs.get_sp();
    cregs.set_sp(sp + 8);
}

u64 Memory::pop_from_stack() {
    u32 sp = cregs.get_sp();
    if (sp == 0xFFFFFFFF) {
        throw std::out_of_range("ERR: stack is empty");
    }
    u64 value = 0;
    for (int i = 0; i < 8; ++i) {
        value = (value << 8) | stack[sp - 8 + i];
    }
    for (int i = 0; i < 8; ++i) {
        stack.pop_back();
    }
    cregs.set_sp(sp - 8);
    return value;
}

void Memory::enter_frame() {
    u32 fp = cregs.get_fp();
    push_to_stack(fp);

    u32 sp = cregs.get_sp();
    cregs.set_fp(sp);
}

void Memory::exit_frame() {
    u32 fp = cregs.get_fp();

    if (fp == 0xFFFFFFFF) {
        throw std::out_of_range("ERR: no frame to exit");
    }

    cregs.set_sp(fp);
    cregs.set_fp(pop_from_stack());
}

void Memory::dbg_log_mem() {
    // std::cout << "dbg_log_mem\n";
    int count = 0;
    for (u8 i : mem) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(i) << " ";
        count++;
        if (count == 32) {
            std::cout << "\n";
            count = 0;
        }
    }

    if (count > 0) {
        std::cout << "\n";
    }

    std::cout << "\n";
}

void Memory::dbg_log_stack() {
    // std::cout << "dbg_log_stack\n";
    int count = 0;
    for (u8 i : stack) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(i);
        count++;
        if (count == 8) {
            std::cout << "\n";
            count = 0;
        }
    }
    std::cout << "\n";
}
