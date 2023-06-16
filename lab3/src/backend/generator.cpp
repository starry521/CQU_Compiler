#include"backend/generator.h"
#include<iostream>
#include<assert.h>

using ir::Operand;
using ir::Operator;
using rv::rvREG;
using rv::rvOPCODE;

#define TODO assert(0 && "todo")

// 整数寄存器toString
std::string rv::toString(rvREG r) {
    switch (r) {
        case rvREG::X0: return "zero";
        case rvREG::X1: return "ra";
        case rvREG::X2: return "sp";
        case rvREG::X3: return "gp";
        case rvREG::X4: return "tp";

        case rvREG::X5: return "t0";
        case rvREG::X6: return "t1";
        case rvREG::X7: return "t2";

        case rvREG::X8: return "s0";
        case rvREG::X9: return "s1";

        case rvREG::X10: return "a0";
        case rvREG::X11: return "a1";
        case rvREG::X12: return "a2";
        case rvREG::X13: return "a3";
        case rvREG::X14: return "a4";
        case rvREG::X15: return "a5";
        case rvREG::X16: return "a6";
        case rvREG::X17: return "a7";

        case rvREG::X18: return "s2";
        case rvREG::X19: return "s3";
        case rvREG::X20: return "s4";
        case rvREG::X21: return "s5";
        case rvREG::X22: return "s6";
        case rvREG::X23: return "s7";
        case rvREG::X24: return "s8";
        case rvREG::X25: return "s9";
        case rvREG::X26: return "s10";
        case rvREG::X27: return "s11";

        case rvREG::X28: return "t3";
        case rvREG::X29: return "t4";
        case rvREG::X30: return "t5";
        case rvREG::X31: return "t6";

        default:
            assert(0 && "invalid rvREG");
        }

    return "";
}

// opcode toString
std::string rv::toString(rvOPCODE r){
    switch (r) {
        case rvOPCODE::ADD: return "add";
        case rvOPCODE::SUB: return "sub";
        case rvOPCODE::MUL: return "mul";
        case rvOPCODE::DIV: return "div";
        case rvOPCODE::REM: return "rem";
        case rvOPCODE::XOR: return "xor";
        case rvOPCODE::OR:  return "or";
        case rvOPCODE::AND: return "and";
        case rvOPCODE::SLL: return "sll";
        case rvOPCODE::SRL: return "srl";
        case rvOPCODE::SRA: return "sra";
        case rvOPCODE::SLT: return "slt";
        case rvOPCODE::SLTU: return "sltu";
        
        case rvOPCODE::ADDI: return "addi";
        case rvOPCODE::XORI: return "xori";
        case rvOPCODE::ORI:  return "ori";
        case rvOPCODE::ANDI: return "andi";
        case rvOPCODE::SLLI: return "slli";
        case rvOPCODE::SRLI: return "srli";
        case rvOPCODE::SLTI: return "slti";
        case rvOPCODE::SLTIU: return "sltiu";
        case rvOPCODE::NOT: return "not";
        case rvOPCODE::SEQZ: return "seqz";
        case rvOPCODE::SNEZ: return "snez";

        case rvOPCODE::LW: return "lw";
        case rvOPCODE::SW: return "sw";

        case rvOPCODE::BEQ: return "beq";
        case rvOPCODE::BNE: return "bne";
        case rvOPCODE::BLT: return "blt";
        case rvOPCODE::BGE: return "bge";
        case rvOPCODE::BLTU: return "bltu";
        case rvOPCODE::BGEU: return "bgeu";
        case rvOPCODE::BNEZ: return "bnez";
        
        case rvOPCODE::JAL: return "jal";
        case rvOPCODE::JALR: return "jalr";

        case rvOPCODE::NOP: return "nop";

        case rvOPCODE::CALL: return "call";

        case rvOPCODE::LA: return "la";
        case rvOPCODE::LI: return "li"; 
        case rvOPCODE::MOV: return "mv"; 
        case rvOPCODE::J: return "j";
        case rvOPCODE::JR: return "jr";

        default:
            assert(0 && "invalid rvOPCODE");
    }

    return "";
}


// 返回局部变量对于栈指针的偏移量
int backend::stackVarMap::find_operand(ir::Operand op){
    if (_table.find(op) != _table.end())    // 栈中有该变量
        return _table[op];
    else{
        return -1;
    }
}


// 返回全局变量对于的偏移量
bool backend::stackVarMap::find_global_operand(const ir::Program& p, ir::Operand op){
    for (auto v: p.globalVal){    // 遍历program的全局变量
        if (v.val.name == op.name){
            return true;
        }
    }
    return false;     // 全局变量找不到此变量
}


// 计算当前变量对于栈指针的偏移量,并且存入表中,返回更新偏移量
void backend::stackVarMap::add_operand(ir::Operand op, uint32_t size = 4){
    _table.insert({op, curr_offset});
    curr_offset += size;    // 更新偏移量
}


// 初始化Generator
backend::Generator::Generator(ir::Program& p, std::ofstream& f): program(p), fout(f) {}


// rd分配寄存器
rvREG backend::Generator::getRd(Operand op){
    return rvREG::X7;   // 固定为t2
}


// rs1分配寄存器
rvREG backend::Generator::getRs1(Operand op){
    return rvREG::X5;   // 固定为t0
}


// rs2分配的寄存器
rvREG backend::Generator::getRs2(Operand op){
    return rvREG::X6;   // 固定为t1
}


// lw指令,从内存中加载两个变量到寄存器t0和t1
void backend::Generator::lw(Operand op, rvREG reg, std::string& str){
    if (stackvarmap.find_operand(op) != -1){     // 栈中有此变量
        str += ("\t" + toString(rvOPCODE::LW) + "\t" + toString(reg) + "," + std::to_string(stackvarmap.find_operand(op)) + "(" + toString(rvREG::X2) + ")\n");
    }else{      // 全局变量
        str += ("\t" + toString(rvOPCODE::LA) + "\t" + toString(reg) + "," + op.name + "\n");
        str += ("\t" + toString(rvOPCODE::LW) + "\t" + toString(reg) + ",0(" + toString(reg) + ")\n");
    }
}


// sw指令，将结果存入内存
void backend::Generator::sw(Operand op, std::string& str){
    if (stackvarmap.find_operand(op) != -1){     // 该变量在栈中已存在
        str += ("\t" + toString(rvOPCODE::SW) + "\t" + toString(rvREG::X7) + "," + std::to_string(stackvarmap.find_operand(op)) + "(" + toString(rvREG::X2) + ")\n");
        
        std::cout<<("\t" + toString(rvOPCODE::SW) + "\t" + toString(rvREG::X7) + "," + std::to_string(stackvarmap.find_operand(op)) + "(" + toString(rvREG::X2) + ")\n");
    }else if (stackvarmap.find_global_operand(Generator::program, op)){    // 该变量为全局变量
        str += ("\t" + toString(rvOPCODE::LA) + "\t" + toString(rvREG::X28) + "," + op.name + "\n");
        str += ("\t" + toString(rvOPCODE::SW) + "\t" + toString(rvREG::X7) + ",0(" + toString(rvREG::X28) + ")\n");
        
        std::cout<<("\t" + toString(rvOPCODE::LA) + "\t" + toString(rvREG::X28) + "," + op.name + "\n");
        std::cout<<("\t" + toString(rvOPCODE::SW) + "\t" + toString(rvREG::X7) + ",0(" + toString(rvREG::X28) + ")\n");
    }else{      // 该变量不存在，在栈中申请空间
        stackvarmap.add_operand(op);
        str += ("\t" + toString(rvOPCODE::SW) + "\t" + toString(rvREG::X7) + "," + std::to_string(stackvarmap.find_operand(op)) + "(" + toString(rvREG::X2) + ")\n");
        
        std::cout<<("\t" + toString(rvOPCODE::SW) + "\t" + toString(rvREG::X7) + "," + std::to_string(stackvarmap.find_operand(op)) + "(" + toString(rvREG::X2) + ")\n");
    }
}


void backend::Generator::set_data(){
    fout << "\t.data\n";
    
    std::cout << "\t.data\n";
}


// 增加全局变量
void backend::Generator::set_globl(std::string globl_name) {
    fout << "\t.globl\t" << globl_name << "\n";
    
    std::cout << "\t.globl\t" << globl_name << "\n";
}


// 增加标签
void backend::Generator::set_label(std::string label_name) {
    fout<<label_name<<":\n";
    
    std::cout<<label_name<<":\n";
}


// 函数第一次扫描，处理goto
void backend::Generator::func_init(const ir::Function& func) {

    label_map.clear();  // 清空标签表
    // 遍历IR,对于goto IR，设置标签表
    for(int i=0; i<(int)func.InstVec.size(); i++){

        auto inst = func.InstVec[i];    // 当前IR

        if (inst->op == ir::Operator::_goto){   // 遇到goto IR

            int goto_iddr = i + stoi(inst->des.name);  // 跳转到的IR所处位置

            if(label_map.find(goto_iddr) == label_map.end()){   // 未出现过
                label_map[goto_iddr] = "label_" + std::to_string(label_cnt++);     // 跳转地址-标签
            }
        }
    }
}


// 生成Program汇编
void backend::Generator::gen() {
    
    // 输出基本头部
    fout<<"\t.option nopic\n";
    std::cout<<"\t.option nopic\n";

    gen_global();   // 处理global函数
    
    fout<<"\t.text\n";
    for (int i=1; i<(int)program.functions.size(); i++){     // 按顺序处理函数,跳过global函数(第一个)
        auto func = program.functions[i];
        gen_func(func);
    }
}


// 分析global函数，生成risc-v指令
void backend::Generator::gen_global(){

    // 根据实验二的实现方式，未初始化的变量和数组都会被全部赋值为0

    auto global_func = program.functions[0];    // global函数

    // 处理变量和数组
    for (auto inst: global_func.InstVec)
    {
        set_data();

        // 只处理Int
        if (inst->op == Operator::def)
        {
            fout << "\t.globl\t" << inst->des.name << "\n";
            fout << "\t.type\t" << inst->des.name << ", @object\n";
            fout << "\t.size\t" << inst->des.name << ", 4\n";
            fout << "\t.align\t" << "4\n";

            std::cout << "\t.globl\t" << inst->des.name << "\n";
            std::cout << "\t.type\t" << inst->des.name << ", @object\n";
            std::cout << "\t.size\t" << inst->des.name << ", 4\n";
            std::cout << "\t.align\t" << "4\n";
            set_label(inst->des.name);

            fout << "\t.word\t" << inst->op1.name << "\n";
            
            std::cout << "\t.word\t" << inst->op1.name << "\n";
        }

        // 遇到alloc，增加数组的定义
        if (inst->op == Operator::alloc){
            fout << "\t.globl\t" << inst->des.name << "\n";
            fout << "\t.type\t" << inst->des.name << ", @object\n";
            fout << "\t.size\t" << inst->des.name << ", " << stoi(inst->op1.name) * 4 << "\n";
            fout << "\t.align\t" << "4\n";

            std::cout << "\t.globl\t" << inst->des.name << "\n";
            std::cout << "\t.type\t" << inst->des.name << ", @object\n";
            std::cout << "\t.size\t" << inst->des.name << ", " << stoi(inst->op1.name) * 4 << "\n";
            std::cout << "\t.align\t" << "4\n";
            set_label(inst->des.name);
        }

        // 遇到store，增加数组的赋值
        if (inst->op == Operator::store){
            fout << "\t.word\t" << inst->des.name << "\n";
            
            std::cout << "\t.word\t" << inst->des.name << "\n";
        }
    }
}


// 生成Function汇编
void backend::Generator::gen_func(const ir::Function& func){

    // 增加函数基本头部
    set_globl(func.name);
    fout << "\t.type\t" << func.name << ", @function\n";

    std::cout << "\t.type\t" << func.name << ", @function\n";
    set_label(func.name);   // 增加函数的标签

    func_init(func);    // 扫描函数所有确定跳转标签

    std::string str="";    // 空字符串,记录函数内的risc-v指令
    stackvarmap.curr_offset = 0;    // 对于栈指针的当前偏移,初始化为0

    // 形参全部存起来
    std::map<int, rvREG> tmp = {{0, rvREG::X10}, {1, rvREG::X11}, {2, rvREG::X12}, {3, rvREG::X13}, {4, rvREG::X14}, {5, rvREG::X15}, {6, rvREG::X16}, {7, rvREG::X17}};
    int i = 0;
    for (auto op: func.ParameterList){  // 遍历函数形参
        stackvarmap.add_operand(op);    // 栈中偏移表增加形参,当前偏移量改变
        str += ("\t" + toString(rvOPCODE::SW) + "\t" + toString(tmp[i]) + "," + std::to_string(stackvarmap.find_operand(op)) + "(" + toString(rvREG::X2) + ")\n");     // 保存在栈中

        std::cout<<("\t" + toString(rvOPCODE::SW) + "\t" + toString(tmp[i]) + "," + std::to_string(stackvarmap.find_operand(op)) + "(" + toString(rvREG::X2) + ")\n");
    }

    curr_ir_iddr = 0;
    for (int i=0; i<(int)func.InstVec.size(); i++) {     //遍历生成函数中指令

        if(label_map.find(i) != label_map.end()){   // 当前指令需要增加标签
            str += (label_map[i] + ":\n");
        }

        auto inst = func.InstVec[i];    // IR指令

        std::cout<<"["<<curr_ir_iddr<<"] "<<inst->draw()<<"\n";     // 打印IR指令

        gen_instr(*inst, str);      // IR转换为risc-v指令

        curr_ir_iddr += 1;
    }

    stackvarmap.curr_offset += 4;   // 栈中返回地址(ra)存放需要4字节

    // 函数入口
    str = ("\t" + toString(rvOPCODE::SW) + "\t" + toString(rvREG::X1) + "," + std::to_string(stackvarmap.curr_offset - 4) + "(" + toString(rvREG::X2) + ")\n") + str;  // 保存返回地址(将ra保存在栈中)
    str = ("\t" + toString(rvOPCODE::ADDI) + "\t" + toString(rvREG::X2) + "," + toString(rvREG::X2) + ",-" + std::to_string(stackvarmap.curr_offset) + "\n") + str;  //调整栈指针，分配栈帧

    // 函数出口
    str += ("\t" + toString(rvOPCODE::LW) + "\t" + toString(rvREG::X1) + "," + std::to_string(stackvarmap.curr_offset - 4) + "(" + toString(rvREG::X2) + ")\n");   // 读取返回地址(ra存返回地址)
    str += ("\t" + toString(rvOPCODE::ADDI) + "\t" + toString(rvREG::X2) + "," + toString(rvREG::X2) + "," + std::to_string(stackvarmap.curr_offset) + "\n");      // 释放栈帧
    str += ("\t" + toString(rvOPCODE::JR) + "\t" + toString(rvREG::X1) + "\n");   // 返回调用点

    str += ("\t.size\t" + func.name + ", .-" + func.name + "\n");
    if (func.name == "main"){
        str += ("\t.ident\t\"GCC: (GNU) 9.2.0\"\n");
        str += ("\t.section\t.note.GNU-stack,\"\",@progbits\n");
    }

    // 向fout中写入函数的指令
    fout<<str;

}


// 生成Instruction汇编
void backend::Generator::gen_instr(ir::Instruction& inst, std::string& str){

    switch(inst.op){
        // and和or IR在lab2处理成短路，因此不用处理and和or
        case Operator::mul:
        case Operator::div:
        case Operator::mod:
        case Operator::lss:
        case Operator::gtr:
        case Operator::sub:
        case Operator::add: {

            rvOPCODE op;    // 指令类型
            switch(inst.op){
                case Operator::add: op = rvOPCODE::ADD; break;
                case Operator::sub: op = rvOPCODE::SUB; break;
                case Operator::mul: op = rvOPCODE::MUL; break;
                case Operator::div: op = rvOPCODE::DIV; break;
                case Operator::mod: op = rvOPCODE::REM; break;
                case Operator::lss: op = rvOPCODE::SLT; break;
                case Operator::gtr: op = rvOPCODE::SLT; break;
                default:
                    assert(0);
                    break;
            }
            
            if (inst.op == Operator::gtr){      //gtr IR转换时使用slt,因此交换rs1和rs2 
                lw(inst.op2, rvREG::X5, str);
                lw(inst.op1, rvREG::X6, str);
            }else{
                lw(inst.op1, rvREG::X5, str);
                lw(inst.op2, rvREG::X6, str);
            }
            
            str += ("\t" + toString(op) + "\t" + toString(rvREG::X7) + "," + toString(rvREG::X5) + "," + toString(rvREG::X6) + "\n");
            sw(inst.des, str);

        } break;

        case Operator::addi: {     // 整数与立即数相加
            
            lw(inst.op1, rvREG::X5, str);   // t0 load op1
            str += ("\t" + toString(rvOPCODE::ADDI) + "\t" + toString(rvREG::X7) + "," + toString(rvREG::X5) + "," + inst.op2.name + "\n");
            sw(inst.des, str);

        } break;

        case Operator::subi: {     // 整数与立即数相减

            lw(inst.op1, rvREG::X5, str);   // t0 load op1
            str += ("\t" + toString(rvOPCODE::ADDI) + "\t" + toString(rvREG::X7) + "," + toString(rvREG::X5) + ",-" + inst.op2.name + "\n");   // t1 load op2(立即数)
            sw(inst.des, str);

        } break;

        case Operator::leq: {   // <=, 例如a<=b, 转换为a>b取反, a>b转换为b<a

            lw(inst.op2, rvREG::X5, str);   // t0 load op2
            lw(inst.op1, rvREG::X6, str);   // t1 load op1
            str += ("\t" + toString(rvOPCODE::SLT) + "\t" + toString(rvREG::X5) + "," + toString(rvREG::X5) + "," + toString(rvREG::X6) + "\n");
            str += ("\t" + toString(rvOPCODE::SEQZ) + "\t" + toString(rvREG::X7) + "," + toString(rvREG::X5) + "\n");
            sw(inst.des, str);  // des save t2

        } break;

        case Operator::geq: {   // >=, 例如a>=b, 转换为a<b取反

            lw(inst.op1, rvREG::X5, str);   // t0 load op1
            lw(inst.op2, rvREG::X6, str);   // t1 load op2
            str += ("\t" + toString(rvOPCODE::SLT) + "\t" + toString(rvREG::X5) + "," + toString(rvREG::X5) + "," + toString(rvREG::X6) + "\n");
            str += ("\t" + toString(rvOPCODE::SEQZ) + "\t" + toString(rvREG::X7) + "," + toString(rvREG::X5) + "\n");
            sw(inst.des, str);  // des save t2

        } break;

        case Operator::eq: {    // ==, 例如a==b, 转换为a xor b, 相等时异或结果为0, 再使用seqz置为1

            lw(inst.op1, rvREG::X5, str);   // t0 load op1
            lw(inst.op2, rvREG::X6, str);   // t1 load op2
            str += ("\t" + toString(rvOPCODE::XOR) + "\t" + toString(rvREG::X5) + "," + toString(rvREG::X5) + "," + toString(rvREG::X6) + "\n");
            str += ("\t" + toString(rvOPCODE::SEQZ) + "\t" + toString(rvREG::X7) + "," + toString(rvREG::X5) + "\n");
            sw(inst.des, str);  // des save t2

        } break;

        case Operator::neq: {   // !=, 例如a!=b

            lw(inst.op1, rvREG::X5, str);   // t0 load op1
            lw(inst.op2, rvREG::X6, str);   // t1 load op2
            str += ("\t" + toString(rvOPCODE::XOR) + "\t" + toString(rvREG::X6) + "," + toString(rvREG::X5) + "," + toString(rvREG::X6) + "\n");
            str += ("\t" + toString(rvOPCODE::SNEZ) + "\t" + toString(rvREG::X7) + "," + toString(rvREG::X6) + "\n");
            sw(inst.des, str);  // des save t2
            
        } break;

        case Operator::_not: {      // !, 例如!a, 结果为逻辑值

            lw(inst.op1, rvREG::X5, str);   // t0 load op1
            str += ("\t" + toString(rvOPCODE::SEQZ) + "\t" + toString(rvREG::X7) + "," + toString(rvREG::X5) + "\n");
            sw(inst.des, str);  // des save t2

        } break;

        case Operator::__unuse__: {     // 无用语句，防止跳出无语句的功能

            str += ("\t" + toString(rvOPCODE::NOP) + "\n");

        } break;

        case Operator::mov: 
        case Operator::def: {
            
            if (inst.op1.type == ir::Type::IntLiteral){     // 操作数为立即数
                stackvarmap.add_operand(inst.des);  // 偏移表中增加
                str += ("\t" + toString(rvOPCODE::LI) + "\t" + toString(rvREG::X7) + "," + inst.op1.name + "\n");
                sw(inst.des, str);
            }else{      // 操作数为变量
                lw(inst.op1, rvREG::X5, str);   // t0 load op1
                str += ("\t" + toString(rvOPCODE::MOV) + "\t" + toString(rvREG::X7) + "," + toString(rvREG::X5) + "\n");
                sw(inst.des, str);  // des save t2
            }

        } break;

        case Operator::load: {      // load IR

            if (inst.op2.type == ir::Type::IntLiteral){     // 组内偏移量为立即数

                if (stackvarmap.find_operand(inst.op1) != -1){   // 数组为局部变量
                    std::string offset = std::to_string(stackvarmap.find_operand(inst.op1) + stoi(inst.op2.name) * 4);     // 栈内偏移=数组偏移+组内偏移
                    str += ("\t" + toString(rvOPCODE::LW) + "\t" + toString(rvREG::X7) + "," + offset + "(" + toString(rvREG::X2) + ")\n");   // 地址=栈指针+栈内偏移=sp+offset
                    sw(inst.des, str);
                    
                }else {      // 数组为全局变量
                    std::string offset = std::to_string(stoi(inst.op2.name) * 4);   //偏移量
                    str += ("\t" + toString(rvOPCODE::LA) + "\t" + toString(rvREG::X6) + "," + inst.op1.name + "\n");     // t1 load symbol 
                    str += ("\t" + toString(rvOPCODE::LW) + "\t" + toString(rvREG::X7) + "," + offset + "(" + toString(rvREG::X6) + ")\n");   // 地址=基址+偏移量
                    sw(inst.des, str);
                }
            }else{      // 组内偏移量为变量

                if (stackvarmap.find_operand(inst.op1) != -1){   // 数组为局部变量
                    lw(inst.op2, rvREG::X5, str);    // t0 load 偏移量
                    str += ("\t" + toString(rvOPCODE::SLLI) + "\t" + toString(rvREG::X5) + "," + toString(rvREG::X5) + ",2\n");  // 偏移量*4，置于t0
                    str += ("\t" + toString(rvOPCODE::LI) + "\t" + toString(rvREG::X6) + "," + std::to_string(stackvarmap.find_operand(inst.op1)) + "\n");  // 数组栈内偏移，置于t1
                    str += ("\t" + toString(rvOPCODE::ADD) + "\t" + toString(rvREG::X5) + "," + toString(rvREG::X5) + "," + toString(rvREG::X6) + "\n");   // 栈内偏移=数组偏移+组内偏移
                    str += ("\t" + toString(rvOPCODE::ADD) + "\t" + toString(rvREG::X5) + "," + toString(rvREG::X5) + "," + toString(rvREG::X2) + "\n");   // 地址=栈指针+栈内偏移=sp+offset
                    str += ("\t" + toString(rvOPCODE::LW) + "\t" + toString(rvREG::X7) + ",0(" + toString(rvREG::X5) + ")\n");
                    sw(inst.des, str);

                }else{  // 数组为全局变量
                    lw(inst.op2, rvREG::X5, str);    // t0 load 偏移量
                    str += ("\t" + toString(rvOPCODE::SLLI) + "\t" + toString(rvREG::X5) + "," + toString(rvREG::X5) + ",2\n");  // 偏移量*4，置于t0
                    str += ("\t" + toString(rvOPCODE::LA) + "\t" + toString(rvREG::X6) + "," + inst.op1.name + "\n");     // t1 load symbol
                    str += ("\t" + toString(rvOPCODE::ADD) + "\t" + toString(rvREG::X5) + "," + toString(rvREG::X5) + "," + toString(rvREG::X6) + "\n");   // 地址=数组基址+偏移*4
                    str += ("\t" + toString(rvOPCODE::LW) + "\t" + toString(rvREG::X7) + ",0(" + toString(rvREG::X5) + ")\n");
                    sw(inst.des, str);
                }
            }

        } break;

        case Operator::store: {     // store IR

            if (inst.op2.type == ir::Type::IntLiteral){     // 组内偏移量为立即数

                if (stackvarmap.find_operand(inst.op1) != -1){   // 数组为局部变量
                    std::string offset = std::to_string(stackvarmap.find_operand(inst.op1) + stoi(inst.op2.name) * 4);     // 栈内偏移=数组偏移+组内偏移
                    str += ("\t" + toString(rvOPCODE::LI) + "\t" + toString(rvREG::X7) + "," + inst.des.name + "\n");
                    str += ("\t" + toString(rvOPCODE::SW) + "\t" + toString(rvREG::X7) + "," + offset + "(" + toString(rvREG::X2) + ")\n");   // 地址=栈指针+栈内偏移=sp+offset
                }else {      // 数组为全局变量
                    std::string offset = std::to_string(stoi(inst.op2.name) * 4);   //偏移量
                    str += ("\t" + toString(rvOPCODE::LI) + "\t" + toString(rvREG::X7) + "," + inst.des.name + "\n");
                    str += ("\t" + toString(rvOPCODE::LA) + "\t" + toString(rvREG::X6) + "," + inst.op1.name + "\n");     // t1 load symbol 
                    str += ("\t" + toString(rvOPCODE::SW) + "\t" + toString(rvREG::X7) + "," + offset + "(" + toString(rvREG::X6) + ")\n");   // 地址=基址+偏移量
                }
            }else{      // 组内偏移量为变量

                if (stackvarmap.find_operand(inst.op1) != -1){   // 数组为局部变量
                    lw(inst.op2, rvREG::X5, str);    // t0 load 偏移量
                    str += ("\t" + toString(rvOPCODE::SLLI) + "\t" + toString(rvREG::X5) + "," + toString(rvREG::X5) + ",2\n");  // 偏移量*4，置于t0
                    str += ("\t" + toString(rvOPCODE::LI) + "\t" + toString(rvREG::X6) + "," + std::to_string(stackvarmap.find_operand(inst.op1)) + "\n");  // 数组栈内偏移，置于t1
                    str += ("\t" + toString(rvOPCODE::ADD) + "\t" + toString(rvREG::X5) + "," + toString(rvREG::X5) + "," + toString(rvREG::X6) + "\n");   // 栈内偏移=数组偏移+组内偏移
                    str += ("\t" + toString(rvOPCODE::ADD) + "\t" + toString(rvREG::X5) + "," + toString(rvREG::X5) + "," + toString(rvREG::X2) + "\n");   // 地址=栈指针+栈内偏移=sp+offset
                    str += ("\t" + toString(rvOPCODE::LI) + "\t" + toString(rvREG::X7) + "," + inst.des.name + "\n");
                    str += ("\t" + toString(rvOPCODE::SW) + "\t" + toString(rvREG::X7) + ",0(" + toString(rvREG::X5) + ")\n");
                }else{  // 数组为全局变量
                    lw(inst.op2, rvREG::X5, str);    // t0 load 偏移量
                    str += ("\t" + toString(rvOPCODE::SLLI) + "\t" + toString(rvREG::X5) + "," + toString(rvREG::X5) + ",2\n");  // 偏移量*4，置于t0
                    str += ("\t" + toString(rvOPCODE::LA) + "\t" + toString(rvREG::X6) + "," + inst.op1.name + "\n");     // t1 load symbol
                    str += ("\t" + toString(rvOPCODE::ADD) + "\t" + toString(rvREG::X5) + "," + toString(rvREG::X5) + "," + toString(rvREG::X6) + "\n");   // 地址=数组基址+偏移*4
                    str += ("\t" + toString(rvOPCODE::LI) + "\t" + toString(rvREG::X7) + "," + inst.des.name + "\n");
                    str += ("\t" + toString(rvOPCODE::SW) + "\t" + toString(rvREG::X7) + ",0(" + toString(rvREG::X5) + ")\n");
                }
            }

        } break;

        case Operator::alloc: {     // 数组分配空间IR
            
            // 出现在global函数中的alloc是全局变量
            if (!stackvarmap.find_global_operand(program, inst.des)){   // 不是全局数组
                stackvarmap._table.insert({inst.des, stackvarmap.curr_offset});     // 插入栈中符号偏移表
                stackvarmap.curr_offset += (stoi(inst.op1.name) * 4);  // 偏移
            }

        } break;

        case Operator::_return: {   // 函数返回

            if (inst.op1.type == ir::Type::IntLiteral){     // 返回值是常量
                str += ("\t" + toString(rvOPCODE::LI) + "\t" + toString(rvREG::X10) + "," + inst.op1.name + "\n");     // a0置为立即数
            }else if (inst.op1.type == ir::Type::Int){      // 返回值是整型常量
                lw(inst.op1, rvREG::X5, str);  // t0 load return value
                str += ("\t" + toString(rvOPCODE::MOV) + "\t" + toString(rvREG::X10) + "," + toString(rvREG::X5) + "\n");
            }

            // return null 不对a0做操作

        } break;

        case Operator::_goto: {     // 跳转语句

            if (inst.op1.type == ir::Type::null){   // 跳转条件op类型为null,即无条件跳转
                str += ("\t" + toString(rvOPCODE::J) + "\t" + label_map[curr_ir_iddr + stoi(inst.des.name)] + "\n");  // 直接跳转
            }else{  // 有整形跳转条件
                lw(inst.op1, rvREG::X5, str);   // t0 load cond value
                str += ("\t" + toString(rvOPCODE::BNEZ) + "\t" + toString(rvREG::X5) + "," + label_map[curr_ir_iddr + stoi(inst.des.name)] + "\n");   // 条件不等于0发生跳转
            }

        } break;

        case Operator::call: {      // 函数调用

            std::map<int, rvREG> tmp = {{0, rvREG::X10}, {1, rvREG::X11}, {2, rvREG::X12}, {3, rvREG::X13}, {4, rvREG::X14}, {5, rvREG::X15}, {6, rvREG::X16}, {7, rvREG::X17}};
            int i = 0;

            auto call_inst = dynamic_cast<ir::CallInst*>(&inst);     // 获取子类

            if (call_inst->op1.name != "global"){

                for (auto op: call_inst->argumentList){  // 遍历调用者实参
                    if (op.type == ir::Type::IntLiteral){   // 实参为立即数
                        str += ("\t" + toString(rvOPCODE::LI) + "\t" + toString(tmp[i]) + "," + op.name + "\n");  
                    }else{      // 实参为变量
                        lw(op, tmp[i], str);    // ai load op
                    }
                }

                str += ("\t" + toString(rvOPCODE::CALL) + "\t" + call_inst->op1.name + "\n");

                stackvarmap.add_operand(inst.des);  // 栈中增加临时变量
                str += ("\t" + toString(rvOPCODE::SW) + "\t" + toString(rvREG::X10) + "," + std::to_string(stackvarmap.find_operand(inst.des)) + "(" + toString(rvREG::X2) + ")\n");  // 将a0中返回值存到临时变量对应的栈偏移中
            }

        } break;

        default:

            assert(0);
            break;

    }
}