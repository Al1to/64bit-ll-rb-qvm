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
    auto reg1 = cregs.opcode_to_reg(mem.fetch8());
    auto reg2 = cregs.opcode_to_reg(mem.fetch8());

    u64 value = cregs.get_reg(reg2.first);
    cregs.set_reg(reg1.first, value);
}

void QPU::mov_rl_0x11() {
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
    auto reg = cregs.opcode_to_reg(mem.fetch8());
    u16 data_address = mem.fetch16();

    cregs.set_reg(reg.first, data_address);
}

void QPU::lea_rar_0x15() {
    auto reg1 = cregs.opcode_to_reg(mem.fetch8());
    auto reg2 = cregs.opcode_to_reg(mem.fetch8());

    cregs.set_reg(reg1.first, cregs.get_reg(reg2.first));
}


void QPU::add_rr_0x20() {
    auto reg1 = cregs.opcode_to_reg(mem.fetch8());
    auto reg2 = cregs.opcode_to_reg(mem.fetch8());

    cregs.set_reg(
        reg1.first, 
        cregs.get_reg(reg1.first) + cregs.get_reg(reg2.first)
    );
}

void QPU::add_rl_0x21() {
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
    auto reg1 = cregs.opcode_to_reg(mem.fetch8());
    auto reg2 = cregs.opcode_to_reg(mem.fetch8());

    cregs.set_reg(
        reg1.first, 
        cregs.get_reg(reg1.first) - cregs.get_reg(reg2.first)
    );
}

void QPU::sub_rl_0x25() {
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
    auto reg = cregs.opcode_to_reg(mem.fetch8());

    cregs.set_reg(
        reg.first, 
        cregs.get_reg(reg.first) + 1
    );
}

void QPU::dec_r_0x29() {
    auto reg = cregs.opcode_to_reg(mem.fetch8());

    cregs.set_reg(
        reg.first, 
        cregs.get_reg(reg.first) - 1
    );
}


void QPU::mul_rr_0x30() {
    auto reg1 = cregs.opcode_to_reg(mem.fetch8());
    auto reg2 = cregs.opcode_to_reg(mem.fetch8());

    cregs.set_reg(
        reg1.first, 
        cregs.get_reg(reg1.first) * cregs.get_reg(reg2.first)
    );
}

void QPU::mul_rl_0x31() {
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
    auto reg1 = cregs.opcode_to_reg(mem.fetch8());
    auto reg2 = cregs.opcode_to_reg(mem.fetch8());

    cregs.set_reg(
        reg1.first, 
        cregs.get_reg(reg1.first) / cregs.get_reg(reg2.first)
    );
}

void QPU::div_rl_0x35() {
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
    if (compare == none) {
        throw std::runtime_error("ERR: conditional jump before comparison instruction.");
    }

    if (compare == less) {
        u16 label = mem.fetch16();
        cregs.set_prgc(mem.get_label_prgc(label));
    }
}

void QPU::jnl_lbl_0x41() {
    if (compare == none) {
        throw std::runtime_error("ERR: conditional jump before comparison instruction.");
    }

    if (compare != less) {
        u16 label = mem.fetch16();
        cregs.set_prgc(mem.get_label_prgc(label));
    }
}


void QPU::je_lbl_0x42() {
    if (compare == none) {
        throw std::runtime_error("ERR: conditional jump before comparison instruction.");
    }

    if (compare == equal) {
        u16 label = mem.fetch16();
        cregs.set_prgc(mem.get_label_prgc(label));
    }
}

void QPU::jne_lbl_0x43() {
    if (compare == none) {
        throw std::runtime_error("ERR: conditional jump before comparison instruction.");
    }

    if (compare != equal) {
        u16 label = mem.fetch16();
        cregs.set_prgc(mem.get_label_prgc(label));
    }
}


void QPU::jg_lbl_0x44() {
    if (compare == none) {
        throw std::runtime_error("ERR: conditional jump before comparison instruction.");
    }

    if (compare == great) {
        u16 label = mem.fetch16();
        cregs.set_prgc(mem.get_label_prgc(label));
    }
}

void QPU::jng_lbl_0x45() {
    if (compare == none) {
        throw std::runtime_error("ERR: conditional jump before comparison instruction.");
    }

    if (compare != great) {
        u16 label = mem.fetch16();
        cregs.set_prgc(mem.get_label_prgc(label));
    }
}


void QPU::jle_lbl_0x46() {
    if (compare == none) {
        throw std::runtime_error("ERR: conditional jump before comparison instruction.");
    }

    if (compare == less || compare == equal) {
        u16 label = mem.fetch16();
        cregs.set_prgc(mem.get_label_prgc(label));
    }
}

void QPU::jge_lbl_0x47() {
    if (compare == none) {
        throw std::runtime_error("ERR: conditional jump before comparison instruction.");
    }

    if (compare == great || compare == equal) {
        u16 label = mem.fetch16();
        cregs.set_prgc(mem.get_label_prgc(label));
    }
}


void QPU::jmp_lbl_0x48() {
    u16 label_number = mem.fetch16();
    cregs.set_prgc(mem.get_label_prgc(label_number));
}

void QPU::jtoa_lbl_0x49() {
    u16 address = mem.fetch16();
    cregs.set_prgc(address);
}


void QPU::call_lbl_0x50() {
    u16 label = mem.fetch16();
    u32 addr_to = mem.get_label_prgc(label);
    u32 ret_addr = cregs.get_prgc();

    mem.enter_frame();
    mem.push_to_stack(ret_addr);

    cregs.set_prgc(addr_to);
}

void QPU::ret_0x51() {
    u32 ret_addr = mem.pop_from_stack();
    mem.exit_frame();

    cregs.set_prgc(ret_addr);
}


void QPU::push_r_0x52() {
    auto reg = cregs.opcode_to_reg(mem.fetch8());
    u64 value = cregs.get_reg(reg.first);
    mem.push_to_stack(value);
}

void QPU::pop_r_0x53() {
    auto reg = cregs.opcode_to_reg(mem.fetch8());
    u64 value = mem.pop_from_stack();
    cregs.set_reg(reg.first, value);
}


void QPU::qreg_qc_0x60() {

}

void QPU::meas_qr_0x61() {

}


void QPU::h_q_0x62() {

}

void QPU::x_q_0x63() {

}

void QPU::y_q_0x64() {

}

void QPU::z_q_0x65() {

}


void QPU::ps_qp_0x66() {

}


void QPU::cx_q_0x67() {

}

void QPU::cps_qp_0x68() {

}
