// 第二次作业，四则运算词法+语法分析

/*
Exp  ->  AddExp

        Exp.v

Number  ->  IntConst  |  floatConst

PrimaryExp  ->  '('  Exp  ')'  |  Number
        PrimaryExp.v

UnaryExp  ->  PrimaryExp  |  UnaryOp  UnaryExp
        UnaryExp.v

UnaryOp  ->  '+'  |  '-'

MulExp  ->  UnaryExp  {  ('*'  |  '/')  UnaryExp  }
        MulExp.v

AddExp  ->  MulExp  {  ('+'  |  '-')  MulExp  }
        AddExp.v
*/


#include<map>
#include<cassert>
#include<string>
#include<iostream>
#include<vector>
#include<set>
#include<queue>
#include <stdint.h>

#define  TODO  assert(0  &&  "TODO")
//  #define  DEBUG_DFA
//  #define  DEBUG_PARSER

//  enumerate  for  Status
enum  class  State  {
        Empty,                            //  space,  \n,  \r  ...
        IntLiteral,                  //  int  literal,  like  '1'  '01900',  '0xAB',  '0b11001'
        op                                    //  operators  and  '(',  ')'
};
std::string  toString(State  s)  {
        switch  (s)  {
        case  State::Empty:  return  "Empty";
        case  State::IntLiteral:  return  "IntLiteral";
        case  State::op:  return  "op";
        default:
            assert(0  &&  "invalid  State");
        }
        return  "";
}

//  enumerate  for  Token  type
enum  class  TokenType{
        INTLTR,                //  int  literal
        PLUS,                //  +
        MINU,                //  -
        MULT,                //  *
        DIV,                //  /
        LPARENT,                //  (
        RPARENT,                //  )
};
std::string  toString(TokenType  type)  {
        switch  (type)  {
        case  TokenType::INTLTR:  return  "INTLTR";
        case  TokenType::PLUS:  return  "PLUS";
        case  TokenType::MINU:  return  "MINU";
        case  TokenType::MULT:  return  "MULT";
        case  TokenType::DIV:  return  "DIV";
        case  TokenType::LPARENT:  return  "LPARENT";
        case  TokenType::RPARENT:  return  "RPARENT";
        default:
            assert(0  &&  "invalid  token  type");
            break;
        }
        return  "";
}

//  definition  of  Token
struct  Token  {
        TokenType  type;
        std::string  value;
};

//  definition  of  DFA
struct  DFA  {
        /**
          *  @brief  constructor,  set  the  init  state  to  State::Empty
          */
        DFA();
        
        /**
          *  @brief  destructor
          */
        ~DFA();
        
        //  the  meaning  of  copy  and  assignment  for  a  DFA  is  not  clear,  so  we  do  not  allow  them
        DFA(const  DFA&)  =  delete;      //  copy  constructor
        DFA&  operator=(const  DFA&)  =  delete;        //  assignment

        /**
          *  @brief  take  a  char  as  input,  change  state  to  next  state,  and  output  a  Token  if  necessary
          *  @param[in]  input:  the  input  character
          *  @param[out]  buf:  the  output  Token  buffer
          *  @return    return  true  if  a  Token  is  produced,  the  buf  is  valid  then
          */
        bool  next(char  input,  Token&  buf);

        /**
          *  @brief  reset  the  DFA  state  to  begin
          */
        void  reset();

private:
        State  cur_state;        //  record  current  state  of  the  DFA
        std::string  cur_str;        //  record  input  characters
};


DFA::DFA():  cur_state(State::Empty),  cur_str()  {}

DFA::~DFA()  {}

//  helper  function,  you  are  not  require  to  implement  these,  but  they  may  be  helpful
bool  isoperator(char  c)  {
    if (c=='+' || c=='-' || c=='*' || c=='/' || c=='(' || c==')')
        return true;
    else
        return false;
}

TokenType  get_op_type(std::string  s)  {
    if (s=="+")
        return TokenType::PLUS;
    else if (s=="-")
        return TokenType::MINU;
    else if (s=="*")
        return TokenType::MULT;
    else if (s=="/")
        return TokenType::DIV;
    else if (s=="(")
        return TokenType::LPARENT;
    else if (s==")")
        return TokenType::RPARENT;
    else
        return TokenType::INTLTR;
}


bool  DFA::next(char  input,  Token&  buf)  {
    switch (cur_state) {
        case State::Empty:
            if (input==' '){
                cur_state = State::Empty;
            } else if (input>='0' && input<='9') {
                cur_state = State::IntLiteral;
                cur_str += input;
            } else if (isoperator(input)) {
                cur_state = State::op;
                cur_str += input;
            }
            break;

        case State::IntLiteral:
            if (cur_str=="0" && (input=='x' || input=='b'))
                cur_str += input;
            else if (cur_str[1]=='x' && input>='A' && input<='F')
                cur_str += input;
            else if (input>='0' && input<='9'){
                cur_str += input;
            }else{
                buf.type = TokenType::INTLTR;
                buf.value = cur_str;
                if (isoperator(input)){
                    cur_state = State::op;
                    cur_str = input;
                }
                else if (input==' ' || input=='\n'){
                    cur_state = State::Empty;
                    cur_str = "";
                }
                return true;
            }
            break;

        case State::op:
            buf.type = get_op_type(cur_str);
            buf.value = cur_str;
            if (isoperator(input)){
                cur_state = State::op;
                cur_str = input;
            }
            if (input==' ' || input=='\n'){
                cur_state = State::Empty;
                cur_str = "";
            }
            else if (input>='0' && input<='9'){
                cur_state = State::IntLiteral;
                cur_str = input;
            }
            return true;
            break;

        default:
            break;
    }

    return false;
}

void  DFA::reset()  {
    cur_state  =  State::Empty;
    cur_str  =  "";
}

//  hw2
enum  class  NodeType  {
        TERMINAL,     //终结符
        EXP,         //表达式
        NUMBER,      //数字
        PRIMARYEXP,  //基础表达式
        UNARYEXP,   //一元表达式
        UNARYOP,   //一元运算符
        MULEXP,   //乘法表达式
        ADDEXP,   //加法表达式
        NONE
};
std::string  toString(NodeType  nt)  {
        switch  (nt)  {
        case  NodeType::TERMINAL:  return  "Terminal";
        case  NodeType::EXP:  return  "Exp";
        case  NodeType::NUMBER:  return  "Number";
        case  NodeType::PRIMARYEXP:  return  "PrimaryExp";
        case  NodeType::UNARYEXP:  return  "UnaryExp";
        case  NodeType::UNARYOP:  return  "UnaryOp";
        case  NodeType::MULEXP:  return  "MulExp";
        case  NodeType::ADDEXP:  return  "AddExp";
        case  NodeType::NONE:  return  "NONE";
        default:
            assert(0  &&  "invalid  node  type");
            break;
        }
        return  "";
}

//  tree  node  basic  class
struct  AstNode{
    int  value;
    NodeType  type;    //  the  node  type
    AstNode*  parent;        //  the  parent  node
    std::vector<AstNode*>  children;          //  children  of  node

    /**
         *  @brief  constructor
         */
    AstNode(NodeType  t  =  NodeType::NONE,  AstNode*  p  =  nullptr):  type(t),  parent(p),  value(0)  {}  

    /**
         *  @brief  destructor
         */
    virtual  ~AstNode()  {
        for(auto  child:  children)  {
                delete  child;
        }
    }

    //  rejcet  copy  and  assignment
    AstNode(const  AstNode&)  =  delete;
    AstNode&  operator=(const  AstNode&)  =  delete;
};



//  definition  of  Parser
//  a  parser  should  take  a  token  stream  as  input,  then  parsing  it,  output  a  AST
struct  Parser  {
    uint32_t  index;  //  current  token  index
    const  std::vector<Token>&  token_stream;

    /**
         *  @brief  constructor
         *  @param  tokens:  the  input  token_stream
         */
    Parser(const  std::vector<Token>&  tokens):  index(0),  token_stream(tokens)  {}

    /**
         *  @brief  destructor
         */
    ~Parser()  {}
    
    /**
         *  @brief  creat  the  abstract  syntax  tree
         *  @return  the  root  of  abstract  syntax  tree
         */
    AstNode*  get_abstract_syntax_tree()  {
        AstNode* root = new AstNode(NodeType::EXP);  //创建根结点
        parseExp(root);
        return root;
    }


    //  u  can  define  member  funcition  of  Parser  here
    void parseExp(AstNode* node);        //Exp
    void parseNumber(AstNode* node);     //Numbers
    void parsePrimaryExp(AstNode* node); //PrimaryExp
    void parseUnaryExp(AstNode* node);   //UnaryExp
    void parseUnaryOp();                 //UnaryOp
    void parseMulExp(AstNode* node);     //MulExp
    void parseAddExp(AstNode* node);     //AddExp

    // for debug, u r not required to use this
    // how to use this: in ur local enviroment, defines the macro DEBUG_PARSER and add this function in every parse fuction
    void log(AstNode* node){
        #ifdef DEBUG_PARSER
            std::cout << "in parse" << toString(node->type) << ", cur_token_type::" << toString(token_stream[index].type) << ", token_val::" << token_stream[index].value << 'n';
        #endif
    }
};


//  u  can  define  funcition  here

//将数字字符串转换为十进制，包括十六进制、八进制、二进制
int trans2ten(std::string value){
    if(value.size() >= 2){
        if(value[0] == '0' && value[1] == 'x'){
            return std::stoi(value, nullptr, 16);
        }else if(value[0] == '0' && value[1] == 'b'){
            value = value.substr(2);
            return std::stoi(value, nullptr, 2);
        }else if(value[0] == '0'){
            return std::stoi(value, nullptr, 8);
        }
    }
    return std::stoi(value, nullptr, 10);
}


void Parser::parseExp(AstNode* f_node){
    
    AstNode* node = new AstNode(NodeType::ADDEXP, f_node);   //创建addExp节点
    parseAddExp(node);
    f_node->value = node->value;   //Exp节点值为addExp节点值
}


void Parser::parseNumber(AstNode* f_node){
    f_node->value = trans2ten(token_stream[index].value);
    index++;
}


void Parser::parsePrimaryExp(AstNode* f_node){
    if (token_stream[index].type == TokenType::LPARENT){
        index++;
        AstNode* node = new AstNode(NodeType::EXP, f_node);  //创建Exp节点
        parseExp(node);
        f_node->value = node->value;
        
    

        if (token_stream[index].type == TokenType::RPARENT)
            index++;
    }
    else{
        AstNode* node = new AstNode(NodeType::NUMBER, f_node);  //创建Number节点
        parseNumber(node);
        f_node->value = node->value;
    }
}  


void Parser::parseUnaryExp(AstNode* f_node){
    
    if (token_stream[index].type == TokenType::LPARENT || token_stream[index].type == TokenType::INTLTR){
        
        AstNode* node = new AstNode(NodeType::PRIMARYEXP, f_node);  //创建primaryexp节点
        parsePrimaryExp(node);
        f_node->value = node->value;
    }else{
        
        TokenType type = token_stream[index].type;
        parseUnaryOp();
        AstNode* node = new AstNode(NodeType::UNARYEXP, f_node);  //创建Unaryexp节点
        parseUnaryExp(node);

        if (type == TokenType::PLUS){
            f_node->value += node->value;
        }else if(type == TokenType::MINU){
            f_node->value -= node->value;
        }
    }
}



void Parser::parseUnaryOp(){
    if (token_stream[index].type == TokenType::PLUS || token_stream[index].type == TokenType::MINU){
        index++;
    }
}

void Parser::parseMulExp(AstNode* f_node){
    
    AstNode* node = new AstNode(NodeType::UNARYEXP, f_node);  //创建Unaryexp节点
    parseUnaryExp(node);
    f_node->value = node->value;

    TokenType type = token_stream[index].type;
    
    while (type == TokenType::MULT || type == TokenType::DIV){
        index++;
        AstNode* node = new AstNode(NodeType::UNARYEXP, f_node);  //创建Unaryexp节点
        parseUnaryExp(node);

        if (type == TokenType::MULT){
            f_node->value *= node->value;
        }else if (type == TokenType::DIV){
            f_node->value /= node->value;
        }

        type = token_stream[index].type;
    }
}


void Parser::parseAddExp(AstNode* f_node){
    
    AstNode* node = new AstNode(NodeType::MULEXP, f_node);  //创建Unaryexp节点
    parseMulExp(node);
    f_node->value = node->value;
    TokenType type = token_stream[index].type;
    while (type == TokenType::PLUS || type == TokenType::MINU){
        index++;
        AstNode* node = new AstNode(NodeType::MULEXP, f_node);  //创建multexp节点
        parseMulExp(node);
        if (type == TokenType::PLUS){
            f_node->value += node->value;
        }else if (type == TokenType::MINU){
            f_node->value -= node->value;
        }

        type = token_stream[index].type;
    }
}



int  main(){
        std::string  stdin_str;
        std::getline(std::cin,  stdin_str);
        stdin_str  +=  "\n";
        DFA  dfa;
        Token  tk;
        std::vector<Token>  tokens;
        for  (size_t  i  =  0;  i  <  stdin_str.size();  i++)  {
            if(dfa.next(stdin_str[i],  tk)){
                std::cout  <<  toString(tk.type)  <<  "  "  <<  tk.value  <<  std::endl;  
                tokens.push_back(tk);  
            }
        }

        std::cout<<std::endl;
        //  hw2
        Parser  parser(tokens);
        auto  root  =  parser.get_abstract_syntax_tree();
        //  u  may  add  function  here  to  analysis  the  AST,  or  do  this  in  parsing
        //  like  get_value(root);

        std::cout  <<  root->value;

        return  0;
}