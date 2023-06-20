#ifndef IROPERAND_H
#define IROPERAND_H

#include <string>


namespace ir {

enum class Type {
    Int,        // 整型变量
    Float,      // 浮点型变量
    IntLiteral,     // 立即数整型
    FloatLiteral,   // 立即数浮点型
    IntPtr,     // 整型指针
    FloatPtr,   // 浮点型指针
    null
};

std::string toString(Type t);

struct Operand {
    std::string name;   //重命名后的变量名
    Type type;  //类型
    Operand(std::string = "null", Type = Type::null);
};

}
#endif
