#ifndef RV_DEF_H
#define RV_DEF_H

#include<string>

namespace rv {

// rv interger registers,整型寄存器
enum class rvREG {
    
    // 常量寄存器,zero
    X0,         

    // 返回地址ra
    X1,         

    // 栈指针sp
    X2,         

    // 全局指针gp
    X3,         

    // tp
    X4,         

    // 临时寄存器t0-t2
    X5,         
    X6,         // .... FIX these comment by reading the risc-v ABI, and u should figure out the role of every register in function call, including its saver(caller/callee)
    X7,
    
    // 保存寄存器s0-s1
    X8,
    X9,

    // 函数参数a0-a7
    X10,
    X11,
    X12,
    X13,
    X14,
    X15,
    X16,
    X17,

    // 保存寄存器s2-s11
    X18,
    X19,
    X20,
    X21,
    X22,
    X23,
    X24,
    X25,
    X26,
    X27,

    // 临时寄存器t3-t6
    X28,
    X29,
    X30,
    X31,
};
std::string toString(rvREG r);  // implement this in ur own way


enum class rvFREG {
    F0,
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,
    F13,
    F14,
    F15,
    F16,
    F17,
    F18,
    F19,
    F20,
    F21,
    F22,
    F23,
    F24,
    F25,
    F26,
    F27,
    F28,
    F29,
    F30,
    F31,
};
std::string toString(rvFREG r);  // implement this in ur own way

// rv32i instructions
// add instruction u need here!
enum class rvOPCODE {
    // RV32I Base Integer Instructions
    ADD, SUB, MUL, DIV, REM, XOR, OR, AND, SLL, SRL, SRA, SLT, SLTU,       // arithmetic & logic
    ADDI, XORI, ORI, ANDI, SLLI, SRLI, SRAI, SLTI, SLTIU, NOT, SEQZ, SNEZ,                  // immediate
    LW, SW,                                                                // load & store
    BEQ, BNE, BLT, BGE, BLTU, BGEU, BNEZ,                        // conditional branch
    JAL, JALR,                                              // jump
    NOP,
    CALL,
    // RV32M Multiply Extension

    // RV32F / D Floating-Point Extensions

    // Pseudo Instructions
    LA, LI, MOV, J, JR                                        // ...
};
std::string toString(rvOPCODE r);  // implement this in ur own way


} // namespace rv



#endif