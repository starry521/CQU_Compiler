#ifndef PROGRAM_H
#define PROGRAM_H

#include "ir/ir_function.h"
#include "ir/ir_operand.h"

#include <vector>
#include <string>

namespace ir
{
    struct GlobalVal
    {
        ir::Operand val;
        int maxlen = 0;     //为数组长度设计
        GlobalVal(ir::Operand va);
        GlobalVal(ir::Operand va, int len);
    };
    struct Program {
        std::vector<Function> functions;    // 函数
        std::vector<GlobalVal> globalVal;   // 全局变量
        Program();
        void addFunction(const Function& proc);
        std::string draw();
    };

}
#endif
