#include"front/lexical.h"

#include<map>
#include<cassert>
#include<string>
#include <cctype>
#define TODO assert(0 && "todo")

// #define DEBUG_DFA
// #define DEBUG_SCANNER

std::string frontend::toString(State s) {
    switch (s) {
    case State::Empty: return "Empty";
    case State::Ident: return "Ident";
    case State::IntLiteral: return "IntLiteral";
    case State::FloatLiteral: return "FloatLiteral";
    case State::op: return "op";
    default:
        assert(0 && "invalid State");
    }
    return "";
}

std::set<std::string> frontend::keywords= {
    "const", "int", "float", "if", "else", "while", "continue", "break", "return", "void"
};

std::set<char> frontend::operators = {
    '+', '-', '*', '/', '%', '<', '>', ':', '=', ';', ',', '(', ')', '[', ']', '{', '}', '!', '&', '|'
};

std::map<std::string, char> frontend::comb_op= {
    {"<", '='},
    {">", '='},
    {"=", '='},
    {"!", '='},
    {"&", '&'},
    {"|", '|'}
};


//自己定义的函数
//判断当前字符是否是操作符
bool  frontend::DFA::isoperator(char  c)  {
    if (operators.find(c) != operators.end())
        return true;
    else
        return false;
}

//取得当前字符串的类型
frontend::TokenType frontend::DFA::get_op_type(std::string  s) {
    if (s=="+")
        return TokenType::PLUS;
    else if (s=="-")
        return TokenType::MINU;
    else if (s=="*")
        return TokenType::MULT;
    else if (s=="/")
        return TokenType::DIV;
    else if (s=="%")
        return TokenType::MOD;
    else if (s=="<")
        return TokenType::LSS;
    else if (s==">")
        return TokenType::GTR;
    else if (s==":")
        return TokenType::COLON;
    else if (s=="=")
        return TokenType::ASSIGN;
    else if (s==";")
        return TokenType::SEMICN;
    else if (s==",")
        return TokenType::COMMA;
    else if (s=="(")
        return TokenType::LPARENT;
    else if (s==")")
        return TokenType::RPARENT;
    else if (s=="[")
        return TokenType::LBRACK;
    else if (s=="]")
        return TokenType::RBRACK;
    else if (s=="{")
        return TokenType::LBRACE;
    else if (s=="}")
        return TokenType::RBRACE;
    else if (s=="!")
        return TokenType::NOT;
    else if (s=="<=")
        return TokenType::LEQ;
    else if (s==">=")
        return TokenType::GEQ;
    else if (s=="==")
        return TokenType::EQL;
    else if (s=="!=")
        return TokenType::NEQ;
    else if (s=="&&")
        return TokenType::AND;
    else if (s=="||")
        return TokenType::OR;
}


bool frontend::DFA::iskeyword(std::string  s){
    if (keywords.find(s) != keywords.end())
        return true;
    else
        return false;
}


frontend::TokenType frontend::DFA::get_keyword_type(std::string s){
    if (s=="const")
        return TokenType::CONSTTK;
    else if (s=="void")
        return TokenType::VOIDTK;
    else if (s=="int")
        return TokenType::INTTK;
    else if (s=="float")
        return TokenType::FLOATTK;
    else if (s=="if")
        return TokenType::IFTK;
    else if (s=="else")
        return TokenType::ELSETK;
    else if (s=="while")
        return TokenType::WHILETK;
    else if (s=="continue")
        return TokenType::CONTINUETK;
    else if (s=="break")
        return TokenType::BREAKTK;
    else if (s=="return")
        return TokenType::RETURNTK;
}


bool frontend::DFA::ishex(char  c){
    if ( (c>='A' && c<='F') || (c>='a' && c<='f') )
        return true;
    else
        return false;
}


bool frontend::DFA::isnum(std::string  s){
    if (s.size() >= 2 && (s[1]!='x' || s[1]!='X'))
        return true;
    else if (s.size() == 1 && std::isdigit(s[0]))
        return true;
    else
        return false;
}


bool frontend::DFA::is_comb_op(std::string s, char c){
    if (comb_op[s] == c)
        return true;
    else
        return false;
}


frontend::DFA::DFA(): cur_state(frontend::State::Empty), cur_str() {}

frontend::DFA::~DFA() {}

bool frontend::DFA::next(char input, Token& buf) {

    #ifdef DEBUG_DFA
    #include<iostream>
        std::cout << "in state [" << toString(cur_state) << "], input = \'" << input << "\', str = " << cur_str << "\t";
    #endif

    bool flag = false;

    switch (cur_state) {

        case State::Empty:
            if (input==' ' || input=='\t' || input=='\n' || input=='\r'){
                cur_state = State::Empty;
            }else if (input=='_' || std::isalpha(input)){
                cur_state = State::Ident;
                cur_str += input;
            }else if (std::isdigit(input)){
                cur_state = State::IntLiteral;
                cur_str += input;
            }else if (isoperator(input)){
                cur_state = State::op;
                cur_str += input;
            }else if (input=='.'){
                cur_state = State::FloatLiteral;
                cur_str += input;
            }

            break;
        

        case State::Ident:
            if (input=='_' || std::isalpha(input) || std::isdigit(input)){
                cur_state = State::Ident;
                cur_str += input;
            }else{
                if (iskeyword(cur_str)){         //关键字
                    buf.type = get_keyword_type(cur_str);
                    buf.value = cur_str;
                }else{      //非关键字的标识符
                    buf.type = TokenType::IDENFR;
                    buf.value = cur_str;
                }

                if (input==' ' || input=='\t' || input=='\n' || input=='\r'){
                    cur_state = State::Empty;
                    cur_str = "";
                }else if (isoperator(input)){
                    cur_state = State::op;
                    cur_str = input;
                }

                // return true;
                flag = true;
            }

            break;


        case State::IntLiteral:
            if (cur_str=="0" && (input=='x' || input=='X' || input=='b' || input=='B'))
                cur_str += input;
            else if ((cur_str[1]=='x' || cur_str[1]=='X') && ishex(input))
                cur_str += input;
            else if (std::isdigit(input))
                cur_str += input;
            else if (input=='.' && isnum(cur_str)){
                cur_state = State::FloatLiteral;
                cur_str += input;
            }
            else{
                buf.type = TokenType::INTLTR;
                buf.value = cur_str;

                if (input==' ' || input=='\t' || input=='\n' || input=='\r'){
                    cur_state = State::Empty;
                    cur_str = "";
                }else if (isoperator(input)){
                    cur_state = State::op;
                    cur_str = input;
                }

                // return true;
                flag = true;
            }

            break;


        case State::FloatLiteral:
            if (std::isdigit(input))
                cur_str += input;
            else{
                buf.type = TokenType::FLOATLTR;
                buf.value = cur_str;

                if (input==' ' || input=='\t' || input=='\n' || input=='\r'){
                    cur_state = State::Empty;
                    cur_str = "";
                }else if (isoperator(input)){
                    cur_state = State::op;
                    cur_str = input;
                }   

                // return true; 
                flag = true;
            }

            break;
        

        case State::op:
            if (is_comb_op(cur_str, input))     //组合操作符
                cur_str += input;
            else{
                buf.type = get_op_type(cur_str);
                buf.value = cur_str;

                if (input==' ' || input=='\t' || input=='\n' || input=='\r'){
                    cur_state = State::Empty;
                    cur_str = "";
                }else if (std::isdigit(input)){
                    cur_state = State::IntLiteral;
                    cur_str = input;
                }else if (isoperator(input)){
                    cur_state = State::op;
                    cur_str = input;
                }else if (input=='_' || std::isalpha(input)){
                    cur_state = State::Ident;
                    cur_str = input;
                }else if (input=='.'){
                    cur_state = State::FloatLiteral;
                    cur_str = input;
                }

                // return true;
                flag = true;
            }
            break;
        
        default:
            break;
    }

    // return false;


    #ifdef DEBUG_DFA
        std::cout << "next state is [" << toString(cur_state) << "], next str = " << cur_str << std::endl;
    #endif

    return flag;
}

void frontend::DFA::reset() {
    cur_state = State::Empty;
    cur_str = "";
}

frontend::Scanner::Scanner(std::string filename): fin(filename) {
    if(!fin.is_open()) {
        assert(0 && "in Scanner constructor, input file cannot open");
    }
}

frontend::Scanner::~Scanner() {
    fin.close();
}

std::vector<frontend::Token> frontend::Scanner::run() {

    std::vector<frontend::Token> token_stream;  //token串
    std::string str, temp;
    int pos, pos1, pos2;

    while (std::getline(fin, temp)){
        
        //注释单行注释
        pos = temp.find("//");
        temp = temp.substr(0, pos);

        str += temp;
        str += "\n";
        
        //去除多行注释

        if (str.find("*/") != std::string::npos){
            pos1 = str.find("/*");
            pos2 = str.rfind("*/");
            if (pos2 - pos1 >= 2)
                str = str.substr(0, pos1) + str.substr(pos2+2);
        }
        
    }


    //初始化DFA
    DFA  dfa;
    Token  tk;

    for (size_t i=0; i < str.size(); i++){
        if(dfa.next(str[i],  tk)){
            token_stream.push_back(tk);

            #ifdef DEBUG_SCANNER
            #include<iostream>
                std::cout << "token: " << toString(tk.type) << "\t" << tk.value << std::endl;
            #endif
        }
    }

    return token_stream;
}