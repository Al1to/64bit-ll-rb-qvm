#include "../headers/QVM.hpp"

void QVM::run() {
    load_to_mem();
    mem.dbg_log_mem();

    while (cregs.get_prgc() < mem.mem_len()) {
        bool is_run = qpu.execute_instr(mem.fetch8());
        
        if (!is_run) {
            break;
        }
    }

    cregs.dbg_log_regs_branch(0);
    cregs.dbg_log_regs_branch(1);
}

void QVM::load_to_mem() {
    std::unordered_map<std::string, u16> label_numbers;
    u16 labels_counter = 0;

    std::unordered_map<std::string, u16> data_numbers;
    u16 data_counter = 0;

    unsigned long long cur = 0;
    unsigned int cur_byte = 0;

    while (cur < qasm_code.length()) {
        while (isspace(qasm_code[cur]) || qasm_code[cur] == ',' || qasm_code[cur] == '[' || qasm_code[cur] == ']') {
            ++cur;
        }

        if (isalpha(qasm_code[cur])) {
            std::string lexeme = std::string() + qasm_code[cur];
            ++cur;

            while (isalpha(qasm_code[cur]) || qasm_code[cur] == '_' || isdigit(qasm_code[cur])) {
                lexeme += std::string() + qasm_code[cur];
                ++cur;
            }

            if (lexeme == "rax") {
                mem.write(cur_byte, 0x5A);
                ++cur_byte;
            } else if (lexeme == "eax") {
                mem.write(cur_byte, 0x4A);
                ++cur_byte;
            } else if (lexeme == "ax") {
                mem.write(cur_byte, 0x3A);
                ++cur_byte;
            } else if (lexeme == "ah") {
                mem.write(cur_byte, 0x2A);
                ++cur_byte;
            } else if (lexeme == "al") {
                mem.write(cur_byte, 0x1A);
                ++cur_byte;
            }

            else if (lexeme == "rbx") {
                mem.write(cur_byte, 0x5B);
                ++cur_byte;
            } else if (lexeme == "ebx") {
                mem.write(cur_byte, 0x4B);
                ++cur_byte;
            } else if (lexeme == "bx") {
                mem.write(cur_byte, 0x3B);
                ++cur_byte;
            } else if (lexeme == "bh") {
                mem.write(cur_byte, 0x2B);
                ++cur_byte;
            } else if (lexeme == "bl") {
                mem.write(cur_byte, 0x1B);
                ++cur_byte;
            }

            else if (lexeme == "rcx") {
                mem.write(cur_byte, 0x5C);
                ++cur_byte;
            } else if (lexeme == "ecx") {
                mem.write(cur_byte, 0x4C);
                ++cur_byte;
            } else if (lexeme == "cx") {
                mem.write(cur_byte, 0x3C);
                ++cur_byte;
            } else if (lexeme == "ch") {
                mem.write(cur_byte, 0x2C);
                ++cur_byte;
            } else if (lexeme == "cl") {
                mem.write(cur_byte, 0x1C);
                ++cur_byte;
            }

            else if (lexeme == "rdx") {
                mem.write(cur_byte, 0x5D);
                ++cur_byte;
            } else if (lexeme == "edx") {
                mem.write(cur_byte, 0x4D);
                ++cur_byte;
            } else if (lexeme == "dx") {
                mem.write(cur_byte, 0x3D);
                ++cur_byte;
            } else if (lexeme == "dh") {
                mem.write(cur_byte, 0x2D);
                ++cur_byte;
            } else if (lexeme == "dl") {
                mem.write(cur_byte, 0x1D);
                ++cur_byte;
            }

            else if (lexeme == "section") {
                mem.write(cur_byte, 0xAA);
                ++cur_byte;
            }

            else if (lexeme == "byte") {
                mem.write(cur_byte, 0x01);
                ++cur_byte;
            }

            else if (lexeme == "word") {
                mem.write(cur_byte, 0x02);
                ++cur_byte;
            }

            else if (lexeme == "dword") {
                mem.write(cur_byte, 0x04);
                ++cur_byte;
            }

            else if (lexeme == "qword") {
                mem.write(cur_byte, 0x08);
                ++cur_byte;
            }

            else if (lexeme == "entry") {
                mem.write(cur_byte, 0x48);
                ++cur_byte;
            }

            else if (lexeme == "hlt") {
                mem.write(cur_byte, 0xFF);
                ++cur_byte;
            }

            else if (lexeme == "mov") {
                instr_type type = check_instr_type(cur);

                if (type == reg_reg) {
                    mem.write(cur_byte, 0x10);
                    ++cur_byte;
                } else if (type == reg_lit) {
                    mem.write(cur_byte, 0x11);
                    ++cur_byte;
                } else if (type == reg_addr) {
                    mem.write(cur_byte, 0x12);
                    ++cur_byte;
                } else if (type == reg_addr_in_reg) {
                    mem.write(cur_byte, 0x13);
                    ++cur_byte;
                }
            }

            else if (lexeme == "lea") {
                instr_type type = check_instr_type(cur);

                if (type == reg_reg || type == reg_lit) {
                    throw std::runtime_error("ERR: invalid syntax `lea reg, ?`, expected `[reg]` or `&data_name`");
                } else if (type == reg_addr) {
                    mem.write(cur_byte, 0x14);
                    ++cur_byte;
                } else if (type == reg_addr_in_reg) {
                    mem.write(cur_byte, 0x15);
                    ++cur_byte;
                }
            }

            else if (lexeme == "add") {
                instr_type type = check_instr_type(cur);

                if (type == reg_reg) {
                    mem.write(cur_byte, 0x20);
                    ++cur_byte;
                } else if (type == reg_lit) {
                    mem.write(cur_byte, 0x21);
                    ++cur_byte;
                } else if (type == reg_addr) {
                    mem.write(cur_byte, 0x22);
                    ++cur_byte;
                } else if (type == reg_addr_in_reg) {
                    mem.write(cur_byte, 0x23);
                    ++cur_byte;
                }
            }

            else if (lexeme == "sub") {
                instr_type type = check_instr_type(cur);

                if (type == reg_reg) {
                    mem.write(cur_byte, 0x24);
                    ++cur_byte;
                } else if (type == reg_lit) {
                    mem.write(cur_byte, 0x25);
                    ++cur_byte;
                } else if (type == reg_addr) {
                    mem.write(cur_byte, 0x26);
                    ++cur_byte;
                } else if (type == reg_addr_in_reg) {
                    mem.write(cur_byte, 0x27);
                    ++cur_byte;
                }
            }

            else if (lexeme == "inc") {
                mem.write(cur_byte, 0x28);
                ++cur_byte;
            }

            else if (lexeme == "dec") {
                mem.write(cur_byte, 0x29);
                ++cur_byte;
            }

            else if (lexeme == "mul") {
                instr_type type = check_instr_type(cur);

                if (type == reg_reg) {
                    mem.write(cur_byte, 0x30);
                    ++cur_byte;
                } else if (type == reg_lit) {
                    mem.write(cur_byte, 0x31);
                    ++cur_byte;
                } else if (type == reg_addr) {
                    mem.write(cur_byte, 0x32);
                    ++cur_byte;
                } else if (type == reg_addr_in_reg) {
                    mem.write(cur_byte, 0x33);
                    ++cur_byte;
                }
            }

            else if (lexeme == "div") {
                instr_type type = check_instr_type(cur);

                if (type == reg_reg) {
                    mem.write(cur_byte, 0x34);
                    ++cur_byte;
                } else if (type == reg_lit) {
                    mem.write(cur_byte, 0x35);
                    ++cur_byte;
                } else if (type == reg_addr) {
                    mem.write(cur_byte, 0x36);
                    ++cur_byte;
                } else if (type == reg_addr_in_reg) {
                    mem.write(cur_byte, 0x37);
                    ++cur_byte;
                }
            }

            else if (lexeme == "cmp") {
                instr_type type = check_instr_type(cur);

                if (type == reg_reg) {
                    mem.write(cur_byte, 0xA3);
                    ++cur_byte;
                } else if (type == reg_lit) {
                    mem.write(cur_byte, 0xB3);
                    ++cur_byte;
                } else if (type == reg_addr) {
                    mem.write(cur_byte, 0xC3);
                    ++cur_byte;
                } else if (type == reg_addr_in_reg) {
                    mem.write(cur_byte, 0xD3);
                    ++cur_byte;
                }
            }

            else if (lexeme == "jl") {
                mem.write(cur_byte, 0x40);
                ++cur_byte;
            }

            else if (lexeme == "jnl") {
                mem.write(cur_byte, 0x41);
                ++cur_byte;
            }

            else if (lexeme == "je") {
                mem.write(cur_byte, 0x42);
                ++cur_byte;
            }

            else if (lexeme == "jne") {
                mem.write(cur_byte, 0x43);
                ++cur_byte;
            }

            else if (lexeme == "jg") {
                mem.write(cur_byte, 0x44);
                ++cur_byte;
            }

            else if (lexeme == "jng") {
                mem.write(cur_byte, 0x45);
                ++cur_byte;
            }

            else if (lexeme == "jle") {
                mem.write(cur_byte, 0x46);
                ++cur_byte;
            }

            else if (lexeme == "jge") {
                mem.write(cur_byte, 0x47);
                ++cur_byte;
            }

            else if (lexeme == "jmp") {
                mem.write(cur_byte, 0x48);
                ++cur_byte;
            }

            else if (lexeme == "jtoa") {
                mem.write(cur_byte, 0x49);
                ++cur_byte;
            }

            else if (lexeme == "call") {
                mem.write(cur_byte, 0x50);
                ++cur_byte;
            }

            else if (lexeme == "ret") {
                mem.write(cur_byte, 0x51);
                ++cur_byte;
            }

            else if (lexeme == "push") {
                mem.write(cur_byte, 0x52);
                ++cur_byte;
            }

            else if (lexeme == "pop") {
                mem.write(cur_byte, 0x53);
                ++cur_byte;
            }

            else if (lexeme == "qreg") {
                mem.write(cur_byte, 0x60);
                ++cur_byte;
            }

            else if (lexeme == "meas") {
                mem.write(cur_byte, 0x61);
                ++cur_byte;
            }

            else if (lexeme == "h") {
                mem.write(cur_byte, 0x62);
                ++cur_byte;
            }

            else if (lexeme == "x") {
                mem.write(cur_byte, 0x63);
                ++cur_byte;
            }

            else if (lexeme == "y") {
                mem.write(cur_byte, 0x64);
                ++cur_byte;
            }

            else if (lexeme == "z") {
                mem.write(cur_byte, 0x65);
                ++cur_byte;
            }

            else if (lexeme == "ps") {
                mem.write(cur_byte, 0x66);
                ++cur_byte;
            }

            else if (lexeme == "cx") {
                mem.write(cur_byte, 0x67);
                ++cur_byte;
            }

            else if (lexeme == "cps") {
                mem.write(cur_byte, 0x68);
                ++cur_byte;
            }

            // quantum instrs -> 0x79

            else {
                auto it = data_numbers.find(lexeme);

                if (it == data_numbers.end()) {
                    data_numbers.insert({lexeme, data_counter});
                    mem.add_data(data_counter, cur_byte);
                    ++data_counter;

                } else {
                    mem.write(cur_byte, it->second & 0xFF);
                    ++cur_byte;
                    mem.write(cur_byte, it->second << 8);
                    ++cur_byte;
                }
            }

        } else if (isdigit(qasm_code[cur]) || qasm_code[cur] == '-') {
            std::string lexeme = std::string() + qasm_code[cur];
            ++cur;

            while (isdigit(qasm_code[cur])) {
                lexeme += std::string() + qasm_code[cur];
                ++cur;
            }

            i64 num = std::stoll(lexeme);

            if (num > 0xFFFFFFFFFFFFFFFF) {
                throw std::runtime_error("ERR: the number exceeds the limits of 64 bits");
            }

            u8 last_opcode = mem.read(cur_byte - 1);
            std::vector<u8> bytes;

            if (last_opcode >= 0x5A) {
                bytes = to_bytes(sign_to_unsign<u64>(num));
            } else if (last_opcode >= 0x4A) {
                bytes = to_bytes(sign_to_unsign<u32>(num));
            } else if (last_opcode >= 0x3A) {
                bytes = to_bytes(sign_to_unsign<u16>(num));
            } else if (last_opcode >= 0x1A) {
                bytes.push_back(sign_to_unsign<u8>(num));
            } 
            
            else if (last_opcode == 0x08) {
                bytes = to_bytes(sign_to_unsign<u64>(num));
            } else if (last_opcode == 0x04) {
                bytes = to_bytes(sign_to_unsign<u32>(num));
            } else if (last_opcode == 0x02) {
                bytes = to_bytes(sign_to_unsign<u16>(num));
            } else if (last_opcode == 0x01) {
                bytes.push_back(sign_to_unsign<u8>(num));
            } else {
                throw std::runtime_error("ERR: invalid syntax");
            }

            for (u8 byte : bytes) {
                mem.write(cur_byte, byte);
                ++cur_byte;
            }

        } else if (qasm_code[cur] == '_') {
            std::string lexeme = std::string() + qasm_code[cur];
            ++cur;

            while (isalpha(qasm_code[cur]) || qasm_code[cur] == '_') {
                lexeme += std::string() + qasm_code[cur];
                ++cur;
            }

            auto it = label_numbers.find(lexeme);

            if (it == label_numbers.end()) {
                label_numbers.insert({lexeme, labels_counter});

                if (qasm_code[cur] == ':') {
                    mem.add_label(labels_counter, cur_byte);
                    ++cur;
                } else {
                    mem.write(cur_byte, labels_counter << 8);
                    ++cur_byte;
                    mem.write(cur_byte, labels_counter & 0xFF);
                    ++labels_counter;
                    ++cur_byte;
                }

            } else {
                if (qasm_code[cur] == ':') {
                    mem.add_label(it->second, cur_byte);
                    ++labels_counter;
                    ++cur;
                } else {
                    mem.write(cur_byte, it->second << 8);
                    ++cur_byte;
                    mem.write(cur_byte, it->second & 0xFF);
                    ++cur_byte;
                }

            }
            
        } else if (qasm_code[cur] == '&') {
            std::string lexeme;
            ++cur;

            while (isalpha(qasm_code[cur]) || qasm_code[cur] == '_' || isdigit(qasm_code[cur])) {
                lexeme += std::string() + qasm_code[cur];
                ++cur;
            }

            auto it = data_numbers.find(lexeme);

            if (it == data_numbers.end()) {
                data_numbers.insert({lexeme, data_counter});
            } else {
                mem.write(cur_byte, it->second << 8);
                ++cur_byte;
                mem.write(cur_byte, it->second & 0xFF);
                ++cur_byte;
            }

        } else if (qasm_code[cur] == '.') {
            std::string lexeme = std::string() + qasm_code[cur];
            ++cur;

            while (isalpha(qasm_code[cur])) {
                lexeme += std::string() + qasm_code[cur];
                ++cur;
            }

            if (lexeme == ".data") {
                mem.write(cur_byte, 0x01);
                ++cur_byte;
            } else if (lexeme == ".text") {
                mem.write(cur_byte, 0x02);
                ++cur_byte;
            }
        }
    }
}

instr_type QVM::check_instr_type(unsigned long long &cur) {
    unsigned long long save_cur = cur;

    while (isspace(qasm_code[cur]) || qasm_code[cur] == ',') {
        ++cur;
    }

    while (isalpha(qasm_code[cur])) {
        ++cur;
    }

    while (isspace(qasm_code[cur]) || qasm_code[cur] == ',') {
        ++cur;
    }

    if (isalpha(qasm_code[cur])) {
        cur = save_cur;
        return instr_type::reg_reg;
    } else if (isdigit(qasm_code[cur]) || (qasm_code[cur] == '-' && isdigit(qasm_code[cur + 1]))) {
        cur = save_cur;
        return instr_type::reg_lit;
    } else if (qasm_code[cur] == '&') {
        cur = save_cur;
        return instr_type::reg_addr;
    } else if (qasm_code[cur] == '[') {
        cur = save_cur;
        return instr_type::reg_addr_in_reg;
    }
    
    throw std::runtime_error("ERR: syntax error, expected `instr reg, reg`, or `instr reg, lit`, or `instr reg, &name`, or `instr reg, [reg]`");
}
