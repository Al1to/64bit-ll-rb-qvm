#include "../headers/Registers.hpp"

c_Registers::c_Registers() {
    regs_mem = std::vector<std::vector<u8>>(reg_names.size(), std::vector<u8>(8, 0));

    for (unsigned int i = 0; i < reg_names.size(); ++i) {
        regs[reg_names[i][0]] = {&regs_mem[i], {0, 8}};
        regs[reg_names[i][1]] = {&regs_mem[i], {4, 4}};
        regs[reg_names[i][2]] = {&regs_mem[i], {6, 2}};
        regs[reg_names[i][3]] = {&regs_mem[i], {6, 1}};
        regs[reg_names[i][4]] = {&regs_mem[i], {7, 1}};
    }
}

std::pair<std::string, u8> c_Registers::opcode_to_reg(u8 opcode) {
    return regs_opcode[opcode];
}

void c_Registers::set_reg(const std::string &reg_name, u64 value) {
    std::vector<u8> value_bytes = to_bytes(value);
    for (int i = 0; i < regs[reg_name].second.second; ++i) {
        (*regs[reg_name].first)[regs[reg_name].second.first + i] = value_bytes[8 - regs[reg_name].second.second + i];
    }
}

u64 c_Registers::get_reg(const std::string &reg_name) {
    u64 value = 0;
    for (int i = 0; i < regs[reg_name].second.second; ++i) {
        value <<= 8;
        value |= (*regs[reg_name].first)[regs[reg_name].second.first + i];
    }
    return value;
}

void c_Registers::set_prgc(u32 address) {
    prgc = address;
}

u32 c_Registers::get_prgc() {
    return prgc;
}

void c_Registers::set_sp(u32 value) {
    sp = value;
}

u32 c_Registers::get_sp() {
    return sp;
}

void c_Registers::set_fp(u32 value) {
    fp = value;
}

u32 c_Registers::get_fp() {
    return fp;
}

void c_Registers::dbg_log_regs_branch(unsigned int line_index) {
    std::cout 
    << reg_names[line_index][0] << ": "
    << std::hex << std::setw(16) << std::setfill('0')
    << get_reg(reg_names[line_index][0]) << "\n"
    << reg_names[line_index][1] << ":         "
    << std::hex << std::setw(8) << std::setfill('0')
    << get_reg(reg_names[line_index][1]) << "\n"
    << reg_names[line_index][2] << ":              "
    << std::hex << std::setw(4) << std::setfill('0')
    << get_reg(reg_names[line_index][2]) << "\n"
    << reg_names[line_index][3] << ":              "
    << std::hex << std::setw(2) << std::setfill('0')
    << get_reg(reg_names[line_index][3]) << "\n"
    << reg_names[line_index][4] << ":                "
    << std::hex << std::setw(2) << std::setfill('0')
    << get_reg(reg_names[line_index][4]) << "\n\n";
}

void q_Register::add_qubit(Qubit it) {
    qubits.push_back(it);
}

int q_Register::measure(unsigned int index) {
    std::cout << "measure\n";
    return qubits[index].measure();
}

void q_Register::apply_X(unsigned int index) {
    Qubit& it = qubits[index];
    std::swap(it.alpha, it.beta);
}

void q_Register::apply_H(unsigned int index) {
    Qubit& it = qubits[index];
    auto alpha_temp = (it.alpha + it.beta) / std::sqrt(2);
    auto beta_temp = (it.alpha - it.beta) / std::sqrt(2);
    it.alpha = alpha_temp;
    it.beta = beta_temp;
}

void q_Register::apply_Y(unsigned int index) {
    Qubit& it = qubits[index];
    auto temp_alpha = std::complex<double>(0, -1) * it.beta;
    auto temp_beta = std::complex<double>(0, 1) * it.alpha;
    it.alpha = temp_alpha;
    it.beta = temp_beta;
}

void q_Register::apply_Z(unsigned int index) {
    Qubit& it = qubits[index];
    it.beta *= std::complex<double>(-1, 0);
}

void q_Register::apply_phase_shift(unsigned int index, double phase) {
    Qubit& it = qubits[index];
    it.beta *= std::exp(std::complex<double>(0, phase));
}

void q_Register::apply_CNOT(unsigned int control_index, unsigned int target_index) {
    if (std::norm(qubits[control_index].beta) > 0.5) {
        apply_X(target_index);
    }
}

void q_Register::apply_controlled_phase_shift(
    unsigned int control_index, 
    unsigned int target_index, 
    double phase
) {
    if (std::norm(qubits[control_index].beta) > 0.5) {
        qubits[target_index].beta *= std::exp(std::complex<double>(0, phase));
    }
}
