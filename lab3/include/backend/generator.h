#ifndef GENERARATOR_H
#define GENERARATOR_H

#include "ir/ir.h"
#include "backend/rv_def.h"

#include<map>
#include<string>
#include<vector>
#include<fstream>

namespace backend {

// it is a map bewteen variable and its mem addr, the mem addr of a local variable can be identified by ($sp + off)
struct stackVarMap {

    int curr_offset;    // 当前函数中已偏移量
    std::map<ir::Operand, int> _table;

    /**
     * @brief find the addr of a ir::Operand
     * @return the offset
    */
    int find_operand(ir::Operand);
    bool find_global_operand(const ir::Program&, ir::Operand);
    /**
     * @brief add a ir::Operand into current map, alloc space for this variable in memory 
     * @param[in] size: the space needed(in byte)
     * @return the offset
    */
    void add_operand(ir::Operand, uint32_t);
};


struct Generator {
    const ir::Program& program;         // the program to gen,IR程序
    std::ofstream& fout;                 // output file
    stackVarMap stackvarmap;    // 栈中变量偏移表
    int label_cnt=0;  // 标签计数器
    std::map<int, std::string> label_map;   // 标签映射表

    int curr_ir_iddr;   // 当前函数中当前是第几条ir

    Generator(ir::Program&, std::ofstream&);

    // reg allocate api
    rv::rvREG getRd(ir::Operand);
    rv::rvREG getRs1(ir::Operand);
    rv::rvREG getRs2(ir::Operand);

    void set_data();                // 进入初始化的数据段
    void set_globl(std::string);    // 全局变量
    void set_label(std::string);    // 增加标签

    // generate wrapper function
    void gen();
    void gen_global();
    void gen_func(const ir::Function&);
    void gen_instr(ir::Instruction&, std::string&, const ir::Function& func);

    void sw(ir::Operand, std::string&);      // 将寄存器t0的值存入内存对应地址
    void lw(ir::Operand, rv::rvREG, std::string&);      // 从内存对应地址取出值存入指定寄存器

    // 第一次分析函数,处理goto
    void func_init(const ir::Function&);
};



} // namespace backend


#endif