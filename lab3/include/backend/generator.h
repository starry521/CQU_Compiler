#ifndef GENERARATOR_H
#define GENERARATOR_H

#include "ir/ir.h"
#include "backend/rv_def.h"
#include "backend/rv_inst_impl.h"

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
    int add_operand(ir::Operand, uint32_t size = 4);
};


struct Generator {
    const ir::Program& program;         // the program to gen,IR程序
    std::ofstream& fout;                 // output file
    stackVarMap stackvarmap;    // 栈中变量偏移表

    Generator(ir::Program&, std::ofstream&);

    // reg allocate api
    rv::rvREG getRd(ir::Operand);
    // rv::rvFREG fgetRd(ir::Operand);

    rv::rvREG getRs1(ir::Operand);
    rv::rvREG getRs2(ir::Operand);
    
    // rv::rvFREG fgetRs1(ir::Operand);
    // rv::rvFREG fgetRs2(ir::Operand);

    // generate wrapper function
    void gen();
    void gen_func(const ir::Function&);
    void gen_instr(ir::Instruction&, std::string& );

    void sw(Operand, std::string&);      // 将寄存器t0的值存入内存对应地址
    void lw(Operand, rvREG, std::string&);      // 从内存对应地址取出值存入指定寄存器
};



} // namespace backend


#endif