#ifndef IRFUNCTION_H
#define IRFUNCTION_H
#include <vector>
#include <string>
#include "ir/ir_operand.h"
#include "ir/ir_instruction.h"
namespace ir
{

struct Function {
    std::string name;   //函数名
    ir::Type returnType;    //返回值类型
    std::vector<Operand> ParameterList; //参数列表
    std::vector<Instruction*> InstVec;  //IR向量
    Function();
    Function(const std::string&, const ir::Type&);
    Function(const std::string&, const std::vector<Operand>&, const ir::Type&);
    void addInst(Instruction* inst);    //增加IR
    std::string draw();
};

}
#endif
