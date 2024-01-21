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
    auto result = qubits[index].measure();
    std::cout << "    meas: " << result << "\n";
    return result;
}

void q_Register::apply_X(unsigned int index) {
    Qubit& it = qubits[index];
    std::cout << "    a: " << it.alpha << " b: " << it.beta << "\n";
    std::swap(it.alpha, it.beta);
    std::cout << "    a: " << it.alpha << " b: " << it.beta << "\n";
}

void q_Register::apply_H(unsigned int index) {
    Qubit& it = qubits[index];
    std::cout << "    a: " << it.alpha << " b: " << it.beta << "\n";
    auto alpha_temp = (it.alpha + it.beta) / std::sqrt(2);
    auto beta_temp = (it.alpha - it.beta) / std::sqrt(2);
    it.alpha = alpha_temp;
    it.beta = beta_temp;
    std::cout << "    a: " << it.alpha << " b: " << it.beta << "\n";
}

void q_Register::apply_Y(unsigned int index) {
    Qubit& it = qubits[index];
    std::cout << "    a: " << it.alpha << " b: " << it.beta << "\n";
    auto temp_alpha = std::complex<double>(0, -1) * it.beta;
    auto temp_beta = std::complex<double>(0, 1) * it.alpha;
    it.alpha = temp_alpha;
    it.beta = temp_beta;
    std::cout << "    a: " << it.alpha << " b: " << it.beta << "\n";
}

void q_Register::apply_Z(unsigned int index) {
    Qubit& it = qubits[index];
    std::cout << "    a: " << it.alpha << " b: " << it.beta << "\n";
    it.beta *= std::complex<double>(-1, 0);
    std::cout << "    a: " << it.alpha << " b: " << it.beta << "\n";
}

void q_Register::apply_rz(unsigned int index, double phase) {
    Qubit& it = qubits[index];
    it.beta *= std::exp(std::complex<double>(0, phase));
}

void q_Register::apply_CNOT(unsigned int control_index, unsigned int target_index) {
    if (std::norm(qubits[control_index].beta) > 0.5) {
        apply_X(target_index);
    }
}

void q_Register::apply_crz(
    unsigned int control_index, 
    unsigned int target_index, 
    double phase
) {
    if (std::norm(qubits[control_index].beta) > 0.5) {
        qubits[target_index].beta *= std::exp(std::complex<double>(0, phase));
    }
}

void q_Register::reset(unsigned int index) {
    qubits[index].reset();
}

void q_Register::dbg_log_qubits() {
    for (auto qubit : qubits) {
        qubit.dbg_log_qubit();
    }
}

void q_Registers::add_qreg(u16 number, u8 qubit_count) {
    q_Register new_qreg;
    for(int i=0; i<qubit_count; i++){
        Qubit new_qubit;
        new_qreg.qubits.push_back(new_qubit);
    }
    qregs.insert({number, new_qreg});
}

q_Register& q_Registers::get_qreg(u16 number) {
    if(qregs.find(number) != qregs.end()){
        return qregs[number];
    }
    else{
        throw std::runtime_error("ERR: qreg " + std::to_string(number) + " does not exist");
    }
}

void q_Registers::dbg_log_qregs() {
    int count = 0;
    std::cout << "\n";
    for (auto& qreg : qregs) {
        std::cout << "qreg " << count << ":\n";
        qreg.second.dbg_log_qubits();
        count++;
    }
    std::cout << "\n";
}
