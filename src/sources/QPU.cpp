#include "../headers/QPU.hpp"

bool QPU::execute_instr(u8 opcode) {
    if (opcode == 0xFF) {
        return false;
    }

    if (opcode == 0x00) {
        std::cout << "ERR: opcode 0x00 detected\n";
        return false;
    }

    auto it = instrs_set.find(opcode);
    if (it != instrs_set.end()) {
        it->second();
        return true;
    } else {
        std::cout 
        << "ERR: unknown opcode 0x" 
        << std::hex << std::setw(2) << std::setfill('0') 
        << static_cast<int>(opcode) << "\n";
        return false;
    }
}

void QPU::mov_rr_0x10() {
    DBG("mov rr 10");
    auto reg1 = cregs.opcode_to_reg(mem.fetch8());
    auto reg2 = cregs.opcode_to_reg(mem.fetch8());

    u64 value = cregs.get_reg(reg2.first);
    cregs.set_reg(reg1.first, value);
}

void QPU::mov_rl_0x11() {
    DBG("mov rl 11");
    auto reg = cregs.opcode_to_reg(mem.fetch8());
    int reg_size = reg.second;
    u64 value;

    if (reg_size == 64) {
        value = mem.fetch64();
    } else if (reg_size == 32) {
        value = mem.fetch32();
    } else if (reg_size == 16) {
        value = mem.fetch16();
    } else {
        value = mem.fetch8();
    }

    cregs.set_reg(reg.first, value);
}

void QPU::mov_ra_0x12() {
    DBG("mov ra 12");
    auto reg = cregs.opcode_to_reg(mem.fetch8());
    u16 data_address = mem.fetch16();

    u32 save_prgc = cregs.get_prgc();

    cregs.set_prgc(mem.get_data_prgc(data_address));

    u8 data_size = mem.fetch8();
    u64 value;

    if (data_size == 0x01) {
        value = mem.fetch8();
    } else if (data_size == 0x02) {
        value = mem.fetch16();
    } else if (data_size == 0x04) {
        value = mem.fetch32();
    } else if (data_size == 0x08) {
        value = mem.fetch64();
    } else {
        throw std::runtime_error("ERR: invalid data size: " + data_size);
    }

    cregs.set_reg(reg.first, value);

    cregs.set_prgc(save_prgc);
}

void QPU::mov_rar_0x13() {
    DBG("mov rar 13");
    auto reg1 = cregs.opcode_to_reg(mem.fetch8());
    auto reg2 = cregs.opcode_to_reg(mem.fetch8());
    u16 data_address = cregs.get_reg(reg2.first);

    u32 save_prgc = cregs.get_prgc();

    cregs.set_prgc(mem.get_data_prgc(data_address));

    u8 data_size = mem.fetch8();
    u64 value;

    if (data_size == 0x01) {
        value = mem.fetch8();
    } else if (data_size == 0x02) {
        value = mem.fetch16();
    } else if (data_size == 0x04) {
        value = mem.fetch32();
    } else if (data_size == 0x08) {
        value = mem.fetch64();
    } else {
        throw std::runtime_error("ERR: invalid data size: " + data_size);
    }

    cregs.set_reg(reg1.first, value);

    cregs.set_prgc(save_prgc);
}


void QPU::lea_ra_0x14() {
    DBG("lea ra 14");
    auto reg = cregs.opcode_to_reg(mem.fetch8());
    u16 data_address = mem.fetch16();

    cregs.set_reg(reg.first, data_address);
}

void QPU::lea_rar_0x15() {
    DBG("lea rar 15");
    auto reg1 = cregs.opcode_to_reg(mem.fetch8());
    auto reg2 = cregs.opcode_to_reg(mem.fetch8());

    cregs.set_reg(reg1.first, cregs.get_reg(reg2.first));
}


void QPU::add_rr_0x20() {
    DBG("add rr 20");
    auto reg1 = cregs.opcode_to_reg(mem.fetch8());
    auto reg2 = cregs.opcode_to_reg(mem.fetch8());

    cregs.set_reg(
        reg1.first, 
        cregs.get_reg(reg1.first) + cregs.get_reg(reg2.first)
    );
}

void QPU::add_rl_0x21() {
    DBG("add rl 21");
    auto reg = cregs.opcode_to_reg(mem.fetch8());
    int reg_size = reg.second;
    u64 value;

    if (reg_size == 64) {
        value = mem.fetch64();
    } else if (reg_size == 32) {
        value = mem.fetch32();
    } else if (reg_size == 16) {
        value = mem.fetch16();
    } else {
        value = mem.fetch8();
    }

    cregs.set_reg(
        reg.first, 
        cregs.get_reg(reg.first) + value
    );
}

void QPU::add_ra_0x22() {
    DBG("add ra 22");
    auto reg = cregs.opcode_to_reg(mem.fetch8());
    u16 data_address = mem.fetch16();

    u32 save_prgc = cregs.get_prgc();

    cregs.set_prgc(mem.get_data_prgc(data_address));

    u8 data_size = mem.fetch8();
    u64 value;

    if (data_size == 0x01) {
        value = mem.fetch8();
    } else if (data_size == 0x02) {
        value = mem.fetch16();
    } else if (data_size == 0x04) {
        value = mem.fetch32();
    } else if (data_size == 0x08) {
        value = mem.fetch64();
    } else {
        throw std::runtime_error("ERR: invalid data size: " + data_size);
    }

    cregs.set_reg(
        reg.first, 
        cregs.get_reg(reg.first) + value
    );

    cregs.set_prgc(save_prgc);
}

void QPU::add_rar_0x23() {
    DBG("add rar 23");
    auto reg1 = cregs.opcode_to_reg(mem.fetch8());
    auto reg2 = cregs.opcode_to_reg(mem.fetch8());
    u16 data_address = cregs.get_reg(reg2.first);

    u32 save_prgc = cregs.get_prgc();

    cregs.set_prgc(mem.get_data_prgc(data_address));

    u8 data_size = mem.fetch8();
    u64 value;

    if (data_size == 0x01) {
        value = mem.fetch8();
    } else if (data_size == 0x02) {
        value = mem.fetch16();
    } else if (data_size == 0x04) {
        value = mem.fetch32();
    } else if (data_size == 0x08) {
        value = mem.fetch64();
    } else {
        throw std::runtime_error("ERR: invalid data size: " + data_size);
    }

    cregs.set_reg(
        reg1.first, 
        cregs.get_reg(reg1.first) + value
    );

    cregs.set_prgc(save_prgc);
}


void QPU::sub_rr_0x24() {
    DBG("sub rr 24");
    auto reg1 = cregs.opcode_to_reg(mem.fetch8());
    auto reg2 = cregs.opcode_to_reg(mem.fetch8());

    cregs.set_reg(
        reg1.first, 
        cregs.get_reg(reg1.first) - cregs.get_reg(reg2.first)
    );
}

void QPU::sub_rl_0x25() {
    DBG("sub rl 25");
    auto reg = cregs.opcode_to_reg(mem.fetch8());
    int reg_size = reg.second;
    u64 value;

    if (reg_size == 64) {
        value = mem.fetch64();
    } else if (reg_size == 32) {
        value = mem.fetch32();
    } else if (reg_size == 16) {
        value = mem.fetch16();
    } else {
        value = mem.fetch8();
    }

    cregs.set_reg(
        reg.first, 
        cregs.get_reg(reg.first) - value
    );
}

void QPU::sub_ra_0x26() {
    DBG("sub ra 26");
    auto reg = cregs.opcode_to_reg(mem.fetch8());
    u16 data_address = mem.fetch16();

    u32 save_prgc = cregs.get_prgc();

    cregs.set_prgc(mem.get_data_prgc(data_address));

    u8 data_size = mem.fetch8();
    u64 value;

    if (data_size == 0x01) {
        value = mem.fetch8();
    } else if (data_size == 0x02) {
        value = mem.fetch16();
    } else if (data_size == 0x04) {
        value = mem.fetch32();
    } else if (data_size == 0x08) {
        value = mem.fetch64();
    } else {
        throw std::runtime_error("ERR: invalid data size: " + data_size);
    }

    cregs.set_reg(
        reg.first, 
        cregs.get_reg(reg.first) - value
    );

    cregs.set_prgc(save_prgc);
}

void QPU::sub_rar_0x27() {
    DBG("sub rar 27");
    auto reg1 = cregs.opcode_to_reg(mem.fetch8());
    auto reg2 = cregs.opcode_to_reg(mem.fetch8());
    u16 data_address = cregs.get_reg(reg2.first);

    u32 save_prgc = cregs.get_prgc();

    cregs.set_prgc(mem.get_data_prgc(data_address));

    u8 data_size = mem.fetch8();
    u64 value;

    if (data_size == 0x01) {
        value = mem.fetch8();
    } else if (data_size == 0x02) {
        value = mem.fetch16();
    } else if (data_size == 0x04) {
        value = mem.fetch32();
    } else if (data_size == 0x08) {
        value = mem.fetch64();
    } else {
        throw std::runtime_error("ERR: invalid data size: " + data_size);
    }

    cregs.set_reg(
        reg1.first, 
        cregs.get_reg(reg1.first) - value
    );

    cregs.set_prgc(save_prgc);
}


void QPU::inc_r_0x28() {
    DBG("inc r 28");
    auto reg = cregs.opcode_to_reg(mem.fetch8());

    cregs.set_reg(
        reg.first, 
        cregs.get_reg(reg.first) + 1
    );
}

void QPU::dec_r_0x29() {
    DBG("dec r 29");
    auto reg = cregs.opcode_to_reg(mem.fetch8());

    cregs.set_reg(
        reg.first, 
        cregs.get_reg(reg.first) - 1
    );
}


void QPU::mul_rr_0x30() {
    DBG("mul rr 30");
    auto reg1 = cregs.opcode_to_reg(mem.fetch8());
    auto reg2 = cregs.opcode_to_reg(mem.fetch8());

    cregs.set_reg(
        reg1.first, 
        cregs.get_reg(reg1.first) * cregs.get_reg(reg2.first)
    );
}

void QPU::mul_rl_0x31() {
    DBG("mul rl 31");
    auto reg = cregs.opcode_to_reg(mem.fetch8());
    int reg_size = reg.second;
    u64 value;

    if (reg_size == 64) {
        value = mem.fetch64();
    } else if (reg_size == 32) {
        value = mem.fetch32();
    } else if (reg_size == 16) {
        value = mem.fetch16();
    } else {
        value = mem.fetch8();
    }

    cregs.set_reg(
        reg.first, 
        cregs.get_reg(reg.first) * value
    );
}

void QPU::mul_ra_0x32() {
    DBG("mul ra 32");
    auto reg = cregs.opcode_to_reg(mem.fetch8());
    u16 data_address = mem.fetch16();

    u32 save_prgc = cregs.get_prgc();

    cregs.set_prgc(mem.get_data_prgc(data_address));

    u8 data_size = mem.fetch8();
    u64 value;

    if (data_size == 0x01) {
        value = mem.fetch8();
    } else if (data_size == 0x02) {
        value = mem.fetch16();
    } else if (data_size == 0x04) {
        value = mem.fetch32();
    } else if (data_size == 0x08) {
        value = mem.fetch64();
    } else {
        throw std::runtime_error("ERR: invalid data size: " + data_size);
    }

    cregs.set_reg(
        reg.first, 
        cregs.get_reg(reg.first) * value
    );

    cregs.set_prgc(save_prgc);
}

void QPU::mul_rar_0x33() {
    DBG("mul rar 33");
    auto reg1 = cregs.opcode_to_reg(mem.fetch8());
    auto reg2 = cregs.opcode_to_reg(mem.fetch8());
    u16 data_address = cregs.get_reg(reg2.first);

    u32 save_prgc = cregs.get_prgc();

    cregs.set_prgc(mem.get_data_prgc(data_address));

    u8 data_size = mem.fetch8();
    u64 value;

    if (data_size == 0x01) {
        value = mem.fetch8();
    } else if (data_size == 0x02) {
        value = mem.fetch16();
    } else if (data_size == 0x04) {
        value = mem.fetch32();
    } else if (data_size == 0x08) {
        value = mem.fetch64();
    } else {
        throw std::runtime_error("ERR: invalid data size: " + data_size);
    }

    cregs.set_reg(
        reg1.first, 
        cregs.get_reg(reg1.first) * value
    );

    cregs.set_prgc(save_prgc);
}


void QPU::div_rr_0x34() {
    DBG("div rr 34");
    auto reg1 = cregs.opcode_to_reg(mem.fetch8());
    auto reg2 = cregs.opcode_to_reg(mem.fetch8());

    cregs.set_reg(
        reg1.first, 
        cregs.get_reg(reg1.first) / cregs.get_reg(reg2.first)
    );
}

void QPU::div_rl_0x35() {
    DBG("div rl 35");
    auto reg = cregs.opcode_to_reg(mem.fetch8());
    int reg_size = reg.second;
    u64 value;

    if (reg_size == 64) {
        value = mem.fetch64();
    } else if (reg_size == 32) {
        value = mem.fetch32();
    } else if (reg_size == 16) {
        value = mem.fetch16();
    } else {
        value = mem.fetch8();
    }

    cregs.set_reg(
        reg.first, 
        cregs.get_reg(reg.first) / value
    );
}

void QPU::div_ra_0x36() {
    DBG("div ra 36");
    auto reg = cregs.opcode_to_reg(mem.fetch8());
    u16 data_address = mem.fetch16();

    u32 save_prgc = cregs.get_prgc();

    cregs.set_prgc(mem.get_data_prgc(data_address));

    u8 data_size = mem.fetch8();
    u64 value;

    if (data_size == 0x01) {
        value = mem.fetch8();
    } else if (data_size == 0x02) {
        value = mem.fetch16();
    } else if (data_size == 0x04) {
        value = mem.fetch32();
    } else if (data_size == 0x08) {
        value = mem.fetch64();
    } else {
        throw std::runtime_error("ERR: invalid data size: " + data_size);
    }

    cregs.set_reg(
        reg.first, 
        cregs.get_reg(reg.first) / value
    );

    cregs.set_prgc(save_prgc);
}

void QPU::div_rar_0x37() {
    DBG("div rar 37");
    auto reg1 = cregs.opcode_to_reg(mem.fetch8());
    auto reg2 = cregs.opcode_to_reg(mem.fetch8());
    u16 data_address = cregs.get_reg(reg2.first);

    u32 save_prgc = cregs.get_prgc();

    cregs.set_prgc(mem.get_data_prgc(data_address));

    u8 data_size = mem.fetch8();
    u64 value;

    if (data_size == 0x01) {
        value = mem.fetch8();
    } else if (data_size == 0x02) {
        value = mem.fetch16();
    } else if (data_size == 0x04) {
        value = mem.fetch32();
    } else if (data_size == 0x08) {
        value = mem.fetch64();
    } else {
        throw std::runtime_error("ERR: invalid data size: " + data_size);
    }

    cregs.set_reg(
        reg1.first, 
        cregs.get_reg(reg1.first) / value
    );

    cregs.set_prgc(save_prgc);
}


void QPU::cmp_rr_0xA3() {
    DBG("cmp rr a3");
    auto reg1 = cregs.opcode_to_reg(mem.fetch8());
    auto reg2 = cregs.opcode_to_reg(mem.fetch8());

    u64 value1 = cregs.get_reg(reg1.first);
    u64 value2 = cregs.get_reg(reg2.first);

    if (value1 == value2) {
        compare = equal;
        return;
    } else if (value1 < value2) {
        compare = less;
        return;
    } else if (value1 > value2) {
        compare = great;
        return;
    } else {
        throw std::runtime_error("ERR: invalid compare");
    }
}

void QPU::cmp_rl_0xB3() {
    DBG("cmp rl b3");
    auto reg = cregs.opcode_to_reg(mem.fetch8());
    int reg_size = reg.second;
    u64 value1 = cregs.get_reg(reg.first);
    u64 value2;

    if (reg_size == 64) {
        value2 = mem.fetch64();
    } else if (reg_size == 32) {
        value2 = mem.fetch32();
    } else if (reg_size == 16) {
        value2 = mem.fetch16();
    } else {
        value2 = mem.fetch8();
    }

    if (value1 == value2) {
        compare = equal;
        return;
    } else if (value1 < value2) {
        compare = less;
        return;
    } else if (value1 > value2) {
        compare = great;
        return;
    } else {
        throw std::runtime_error("ERR: invalid compare");
    }
}

void QPU::cmp_ra_0xC3() {
    DBG("cmp ra c3");
    auto reg = cregs.opcode_to_reg(mem.fetch8());
    u64 value1 = cregs.get_reg(reg.first);
    u16 data_address = mem.fetch16();

    u32 save_prgc = cregs.get_prgc();

    cregs.set_prgc(mem.get_data_prgc(data_address));

    u8 data_size = mem.fetch8();
    u64 value2;

    if (data_size == 0x01) {
        value2 = mem.fetch8();
    } else if (data_size == 0x02) {
        value2 = mem.fetch16();
    } else if (data_size == 0x04) {
        value2 = mem.fetch32();
    } else if (data_size == 0x08) {
        value2 = mem.fetch64();
    } else {
        throw std::runtime_error("ERR: invalid data size: " + data_size);
    }

    cregs.set_prgc(save_prgc);

    if (value1 == value2) {
        compare = equal;
        return;
    } else if (value1 < value2) {
        compare = less;
        return;
    } else if (value1 > value2) {
        compare = great;
        return;
    } else {
        throw std::runtime_error("ERR: invalid compare");
    }
}

void QPU::cmp_rar_0xD3() {
    DBG("cmp rar d3");
    auto reg1 = cregs.opcode_to_reg(mem.fetch8());
    auto reg2 = cregs.opcode_to_reg(mem.fetch8());
    u64 value1 = cregs.get_reg(reg1.first);
    u16 data_address = cregs.get_reg(reg2.first);

    u32 save_prgc = cregs.get_prgc();

    cregs.set_prgc(mem.get_data_prgc(data_address));

    u8 data_size = mem.fetch8();
    u64 value2;

    if (data_size == 0x01) {
        value2 = mem.fetch8();
    } else if (data_size == 0x02) {
        value2 = mem.fetch16();
    } else if (data_size == 0x04) {
        value2 = mem.fetch32();
    } else if (data_size == 0x08) {
        value2 = mem.fetch64();
    } else {
        throw std::runtime_error("ERR: invalid data size: " + data_size);
    }

    cregs.set_prgc(save_prgc);

    if (value1 == value2) {
        compare = equal;
        return;
    } else if (value1 < value2) {
        compare = less;
        return;
    } else if (value1 > value2) {
        compare = great;
        return;
    } else {
        throw std::runtime_error("ERR: invalid compare");
    }
}


void QPU::jl_lbl_0x40() {
    DBG("jl lbl 40");
    if (compare == none) {
        throw std::runtime_error("ERR: conditional jump before comparison instruction.");
    }

    if (compare == less) {
        u16 label = mem.fetch16();
        cregs.set_prgc(mem.get_label_prgc(label));
    }
}

void QPU::jnl_lbl_0x41() {
    DBG("jnl lbl 41");
    if (compare == none) {
        throw std::runtime_error("ERR: conditional jump before comparison instruction.");
    }

    if (compare != less) {
        u16 label = mem.fetch16();
        cregs.set_prgc(mem.get_label_prgc(label));
    }
}


void QPU::je_lbl_0x42() {
    DBG("je lbl 42");
    if (compare == none) {
        throw std::runtime_error("ERR: conditional jump before comparison instruction.");
    }

    if (compare == equal) {
        u16 label = mem.fetch16();
        cregs.set_prgc(mem.get_label_prgc(label));
    }
}

void QPU::jne_lbl_0x43() {
    DBG("jne lbl 43");
    if (compare == none) {
        throw std::runtime_error("ERR: conditional jump before comparison instruction.");
    }

    if (compare != equal) {
        u16 label = mem.fetch16();
        cregs.set_prgc(mem.get_label_prgc(label));
    }
}


void QPU::jg_lbl_0x44() {
    DBG("jg lbl 44");
    if (compare == none) {
        throw std::runtime_error("ERR: conditional jump before comparison instruction.");
    }

    if (compare == great) {
        u16 label = mem.fetch16();
        cregs.set_prgc(mem.get_label_prgc(label));
    }
}

void QPU::jng_lbl_0x45() {
    DBG("jng lbl 45");
    if (compare == none) {
        throw std::runtime_error("ERR: conditional jump before comparison instruction.");
    }

    if (compare != great) {
        u16 label = mem.fetch16();
        cregs.set_prgc(mem.get_label_prgc(label));
    }
}


void QPU::jle_lbl_0x46() {
    DBG("jle lbl 46");
    if (compare == none) {
        throw std::runtime_error("ERR: conditional jump before comparison instruction.");
    }

    if (compare == less || compare == equal) {
        u16 label = mem.fetch16();
        cregs.set_prgc(mem.get_label_prgc(label));
    }
}

void QPU::jge_lbl_0x47() {
    DBG("jge lbl 47");
    if (compare == none) {
        throw std::runtime_error("ERR: conditional jump before comparison instruction.");
    }

    if (compare == great || compare == equal) {
        u16 label = mem.fetch16();
        cregs.set_prgc(mem.get_label_prgc(label));
    }
}


void QPU::jmp_lbl_0x48() {
    DBG("jmp lbl 48");
    u16 label_number = mem.fetch16();
    cregs.set_prgc(mem.get_label_prgc(label_number));
}

void QPU::jtoa_lbl_0x49() {
    DBG("jtoa lbl 49");
    u16 address = mem.fetch16();
    cregs.set_prgc(address);
}


void QPU::call_lbl_0x50() {
    DBG("call lbl 50");
    u16 label = mem.fetch16();
    u32 addr_to = mem.get_label_prgc(label);
    u32 ret_addr = cregs.get_prgc();

    mem.enter_frame();
    mem.push_to_stack(ret_addr);

    cregs.set_prgc(addr_to);
}

void QPU::ret_0x51() {
    DBG("ret 51");
    u32 ret_addr = mem.pop_from_stack();
    mem.exit_frame();

    cregs.set_prgc(ret_addr);
}


void QPU::push_r_0x52() {
    DBG("push r 52");
    auto reg = cregs.opcode_to_reg(mem.fetch8());
    u64 value = cregs.get_reg(reg.first);
    mem.push_to_stack(value);
}

void QPU::pop_r_0x53() {
    DBG("pop r 53");
    auto reg = cregs.opcode_to_reg(mem.fetch8());
    u64 value = mem.pop_from_stack();
    cregs.set_reg(reg.first, value);
}


void QPU::qreg_qc_0x60() {
    DBG("qreg qc 60");
    u16 number = mem.fetch16();
    u8 count = mem.fetch8();

    qregs.add_qreg(number, count);
}

void QPU::meas_rq_0x61() {
    DBG("meas rq 60");
    auto reg = cregs.opcode_to_reg(mem.fetch8());
    u16 qreg_number = mem.fetch16();
    u8 qubit_index = mem.fetch8();

    auto &qreg = qregs.get_qreg(qreg_number);
    cregs.set_reg(reg.first, qreg.measure(qubit_index));
}


void QPU::h_q_0x62() {
    DBG("H q 62");
    u16 qreg_number = mem.fetch16();
    u8 qubit_index = mem.fetch8();

    auto &qreg = qregs.get_qreg(qreg_number);
    qreg.apply_H(qubit_index);
}

void QPU::x_q_0x63() {
    DBG("X q 63");
    u16 qreg_number = mem.fetch16();
    u8 qubit_index = mem.fetch8();

    auto &qreg = qregs.get_qreg(qreg_number);
    qreg.apply_X(qubit_index);
}

void QPU::y_q_0x64() {
    DBG("Y q 64");
    u16 qreg_number = mem.fetch16();
    u8 qubit_index = mem.fetch8();

    auto &qreg = qregs.get_qreg(qreg_number);
    qreg.apply_Y(qubit_index);
}

void QPU::z_q_0x65() {
    DBG("Z q 65");
    u16 qreg_number = mem.fetch16();
    u8 qubit_index = mem.fetch8();

    auto &qreg = qregs.get_qreg(qreg_number);
    qreg.apply_Z(qubit_index);
}


void QPU::rz_qp_0x66() {

}


void QPU::cx_qq_0x67() {

}

void QPU::crz_qqp_0x68() {

}


void QPU::rst_q_0x69() {
    DBG("rst q 69");
    u16 qreg_number = mem.fetch16();
    u8 qubit_index = mem.fetch8();

    auto &qreg = qregs.get_qreg(qreg_number);
    qreg.reset(qubit_index);
}
