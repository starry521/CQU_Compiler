#ifndef IRINSTRUCTION_H
#define IRINSTRUCTION_H

#include "ir/ir_operand.h"
#include "ir/ir_operator.h"

#include <vector>
#include <string>


namespace ir
{

struct Instruction {    // IR
    Operand op1;    // 第一个操作数
    Operand op2;    // 第二个操作数
    Operand des;    // 目标操作数
    Operator op;    // 操作符
    Instruction();
    Instruction(const Operand& op1, const Operand& op2, const Operand& des, const Operator& op);
    virtual std::string draw() const;
};

struct CallInst: public Instruction{    // 函数调用IR
    std::vector<Operand> argumentList;  // 参数列表
    CallInst(const Operand& op1, std::vector<Operand> paraList, const Operand& des);
    CallInst(const Operand& op1, const Operand& des);   //无参数情况
    std::string draw() const;
};


}
#endif
