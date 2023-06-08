#include"backend/generator.h"

#include<assert.h>


#define TODO assert(0 && "todo")

backend::Generator::Generator(ir::Program& p, std::ofstream& f): program(p), fout(f) {}

void backend::Generator::gen() {
    TODO;
}