#pragma once

#include<functional>

#include "../utils.hpp"
#include "./Memory.hpp"
#include "./Registers.hpp"

enum CMP {
    equal,
    less, 
    great,
    none
};

class QPU {
public:
    QPU(Memory &mem, c_Registers &cregs, q_Registers &qregs) : mem(mem), cregs(cregs), qregs(qregs) {
        instrs_set = {
            { 0x10, [this]() {  mov_rr_0x10(); } },
            { 0x11, [this]() {  mov_rl_0x11(); } },
            { 0x12, [this]() {  mov_ra_0x12(); } },
            { 0x13, [this]() {  mov_rar_0x13(); } },

            { 0x14, [this]() {  lea_ra_0x14(); } },
            { 0x15, [this]() {  lea_rar_0x15(); } },

            { 0x16, [this]() {  out_r_0x16(); } },
            { 0x17, [this]() {  in_r_0x17(); } },

            { 0x20, [this]() {  add_rr_0x20(); } },
            { 0x21, [this]() {  add_rl_0x21(); } },
            { 0x22, [this]() {  add_ra_0x22(); } },
            { 0x23, [this]() {  add_rar_0x23(); } },

            { 0x24, [this]() {  sub_rr_0x24(); } },
            { 0x25, [this]() {  sub_rl_0x25(); } },
            { 0x26, [this]() {  sub_ra_0x26(); } },
            { 0x27, [this]() {  sub_rar_0x27(); } },

            { 0x28, [this]() {  inc_r_0x28(); } },
            { 0x29, [this]() {  dec_r_0x29(); } },

            { 0x30, [this]() {  mul_rr_0x30(); } },
            { 0x31, [this]() {  mul_rl_0x31(); } },
            { 0x32, [this]() {  mul_ra_0x32(); } },
            { 0x33, [this]() {  mul_rar_0x33(); } },

            { 0x34, [this]() {  div_rr_0x34(); } },
            { 0x35, [this]() {  div_rl_0x35(); } },
            { 0x36, [this]() {  div_ra_0x36(); } },
            { 0x37, [this]() {  div_rar_0x37(); } },

            { 0xA3, [this]() {  cmp_rr_0xA3(); } },
            { 0xB3, [this]() {  cmp_rl_0xB3(); } },
            { 0xC3, [this]() {  cmp_ra_0xC3(); } },
            { 0xD3, [this]() {  cmp_rar_0xD3(); } },

            { 0x40, [this]() {  jl_lbl_0x40(); } },
            { 0x41, [this]() {  jnl_lbl_0x41(); } },

            { 0x42, [this]() {  je_lbl_0x42(); } },
            { 0x43, [this]() {  jne_lbl_0x43(); } },

            { 0x44, [this]() {  jg_lbl_0x44(); } },
            { 0x45, [this]() {  jng_lbl_0x45(); } },

            { 0x46, [this]() {  jle_lbl_0x46(); } },
            { 0x47, [this]() {  jge_lbl_0x47(); } },

            { 0x48, [this]() {  jmp_lbl_0x48(); } },
            { 0x49, [this]() {  jtoa_ra_0x49(); } },

            { 0x50, [this]() {  call_lbl_0x50(); } },
            { 0x51, [this]() {  ret_0x51(); } },

            { 0x52, [this]() {  push_r_0x52(); } },
            { 0x53, [this]() {  pop_r_0x53(); } },

            { 0x60, [this]() {  qreg_qc_0x60(); } },
            { 0x61, [this]() {  meas_rq_0x61(); } },

            { 0x62, [this]() {  h_q_0x62(); } },
            { 0x63, [this]() {  x_q_0x63(); } },
            { 0x64, [this]() {  y_q_0x64(); } },
            { 0x65, [this]() {  z_q_0x65(); } },

            { 0x66, [this]() {  rz_qp_0x66(); } },

            { 0x67, [this]() {  cx_qq_0x67(); } },
            { 0x68, [this]() {  crz_qqp_0x68(); } },

            { 0x69, [this]() {  rst_q_0x69(); } },

            { 0x70, [this]() {  swap_qq_0x70(); } },
        };
    }

    bool execute_instr(u8 opcode);

private:
    Memory& mem;
    c_Registers& cregs;
    q_Registers& qregs;
    CMP compare = none;
    std::unordered_map<u8, std::function<void()>> instrs_set;

    void mov_rr_0x10();

    void mov_rl_0x11();

    void mov_ra_0x12();

    void mov_rar_0x13();


    void lea_ra_0x14();

    void lea_rar_0x15();


    void out_r_0x16();

    void in_r_0x17();


    void add_rr_0x20();

    void add_rl_0x21();

    void add_ra_0x22();

    void add_rar_0x23();


    void sub_rr_0x24();

    void sub_rl_0x25();

    void sub_ra_0x26();

    void sub_rar_0x27();


    void inc_r_0x28();

    void dec_r_0x29();


    void mul_rr_0x30();

    void mul_rl_0x31();

    void mul_ra_0x32();

    void mul_rar_0x33();


    void div_rr_0x34();

    void div_rl_0x35();

    void div_ra_0x36();

    void div_rar_0x37();


    void cmp_rr_0xA3();

    void cmp_rl_0xB3();

    void cmp_ra_0xC3();

    void cmp_rar_0xD3();


    void jl_lbl_0x40();

    void jnl_lbl_0x41();


    void je_lbl_0x42();

    void jne_lbl_0x43();


    void jg_lbl_0x44();

    void jng_lbl_0x45();


    void jle_lbl_0x46();

    void jge_lbl_0x47();


    void jmp_lbl_0x48();

    void jtoa_ra_0x49();


    void call_lbl_0x50();

    void ret_0x51();


    void push_r_0x52();

    void pop_r_0x53();


    void qreg_qc_0x60();

    void meas_rq_0x61();


    void h_q_0x62();

    void x_q_0x63();

    void y_q_0x64();

    void z_q_0x65();


    void rz_qp_0x66();


    void cx_qq_0x67();

    void crz_qqp_0x68();


    void rst_q_0x69();


    void swap_qq_0x70();
};
