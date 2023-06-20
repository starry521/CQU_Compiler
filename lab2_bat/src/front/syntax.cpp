#include"front/syntax.h"

#include<iostream>
#include<cassert>

using frontend::Parser;

// #define DEBUG_PARSER
#define TODO assert(0 && "todo")
#define CUR_TOKEN_IS(tk_type) (token_stream[index].type == TokenType::tk_type)
#define PARSE_TOKEN(tk_type) root->children.push_back(parseTerm(root, TokenType::tk_type))
#define PARSE(name, type) auto name = new type(root); assert(parse##type(name)); root->children.push_back(name); 


Parser::Parser(const std::vector<frontend::Token>& tokens): index(0), token_stream(tokens) {}

Parser::~Parser() {}

frontend::CompUnit* Parser::get_abstract_syntax_tree(){
    //创建根节点
    frontend::CompUnit* root = new frontend::CompUnit();
    assert(parseCompUnit(root));
    return root;
}


frontend::Term* Parser::parseTerm(AstNode* parent, TokenType expected){
    
    if (token_stream[index].type == expected){

        Term* term = new Term(token_stream[index], parent);
        index++;

        return term;

    }else{

        assert(0 && "Terminator does not meet expectations");
    }
}


bool Parser::in_unaryexp(){

    if (CUR_TOKEN_IS(PLUS) || CUR_TOKEN_IS(MINU) || CUR_TOKEN_IS(NOT) || CUR_TOKEN_IS(IDENFR) || CUR_TOKEN_IS(LPARENT) || CUR_TOKEN_IS(INTLTR) || CUR_TOKEN_IS(FLOATLTR))
        return true;

    else
        return false;
}


bool Parser::in_stmt(){

    if (CUR_TOKEN_IS(LBRACE) || CUR_TOKEN_IS(IFTK) || CUR_TOKEN_IS(WHILETK) || CUR_TOKEN_IS(BREAKTK) || CUR_TOKEN_IS(CONTINUETK) || CUR_TOKEN_IS(RETURNTK) || CUR_TOKEN_IS(SEMICN) || in_unaryexp())
        return true;

    else
        return false;
}


// CompUnit -> (Decl | FuncDef) [CompUnit]
bool Parser::parseCompUnit(CompUnit* root){

    log(root);

    if (CUR_TOKEN_IS(INTTK) || CUR_TOKEN_IS(FLOATTK)){

        if (token_stream[index+2].type == TokenType::LBRACK || token_stream[index+2].type == TokenType::ASSIGN || token_stream[index+2].type == TokenType::COMMA || token_stream[index+2].type == TokenType::SEMICN){
        
            PARSE(decl, Decl);

        }else if (token_stream[index+2].type == TokenType::LPARENT){

            PARSE(funcdef, FuncDef);
        }

    }else if (CUR_TOKEN_IS(CONSTTK)){

        PARSE(decl, Decl);

    }else if (CUR_TOKEN_IS(VOIDTK)){

        PARSE(funcdef, FuncDef);
    }


    if (CUR_TOKEN_IS(CONSTTK) || CUR_TOKEN_IS(VOIDTK) || CUR_TOKEN_IS(INTTK) || CUR_TOKEN_IS(FLOATTK)){

        PARSE(compunit, CompUnit);
    }

    return true;
}


// Decl -> ConstDecl | VarDecl
bool Parser::parseDecl(Decl* root){

    log(root);
    
    if (CUR_TOKEN_IS(CONSTTK)){

        PARSE(constdecl, ConstDecl);

    }else if (CUR_TOKEN_IS(INTTK) || CUR_TOKEN_IS(FLOATTK)){

        PARSE(vardecl, VarDecl);
    }

    return true;
}


// ConstDecl -> 'const' BType ConstDef { ',' ConstDef } ';'
bool Parser::parseConstDecl(ConstDecl* root){

    log(root);

    PARSE_TOKEN(CONSTTK);

    PARSE(btype, BType);

    PARSE(constdef, ConstDef);

    while (CUR_TOKEN_IS(COMMA)){

        PARSE_TOKEN(COMMA);

        PARSE(constdef, ConstDef);
    }

    PARSE_TOKEN(SEMICN);

    return true;
}


// BType -> 'int' | 'float'
bool Parser::parseBType(BType* root){

    log(root);

    if (CUR_TOKEN_IS(INTTK)){

        PARSE_TOKEN(INTTK);

    }else if (CUR_TOKEN_IS(FLOATTK)){

        PARSE_TOKEN(FLOATTK);
    }

    return true;
}


// ConstDef -> Ident { '[' ConstExp ']' } '=' ConstInitVal
bool Parser::parseConstDef(ConstDef* root){

    log(root);

    PARSE_TOKEN(IDENFR);

    while (CUR_TOKEN_IS(LBRACK)){

        PARSE_TOKEN(LBRACK);

        PARSE(constexp, ConstExp);

        PARSE_TOKEN(RBRACK);
    }

    PARSE_TOKEN(ASSIGN);

    PARSE(constinitval, ConstInitVal);

    return true;
}


// ConstInitVal -> ConstExp | '{' [ ConstInitVal { ',' ConstInitVal } ] '}'
bool Parser::parseConstInitVal(ConstInitVal* root){

    log(root);

    if (CUR_TOKEN_IS(LBRACE)){

        PARSE_TOKEN(LBRACE);

        if (in_unaryexp() || CUR_TOKEN_IS(LBRACE)){

            PARSE(constinitval, ConstInitVal);

            while (CUR_TOKEN_IS(COMMA)){

                PARSE_TOKEN(COMMA);

                PARSE(constinitval, ConstInitVal);
            }
        }

        PARSE_TOKEN(RBRACE);

    }else if (in_unaryexp()){

        PARSE(constexp, ConstExp);
    }

    return true;
}


// VarDecl -> BType VarDef { ',' VarDef } ';'
bool Parser::parseVarDecl(VarDecl* root){

    log(root);

    PARSE(btype, BType);

    PARSE(vardef, VarDef);

    while (CUR_TOKEN_IS(COMMA)){
        
        PARSE_TOKEN(COMMA);

        PARSE(vardef, VarDef);
    }

    PARSE_TOKEN(SEMICN);

    return true;
}


// VarDef -> Ident { '[' ConstExp ']' } [ '=' InitVal ]
bool Parser::parseVarDef(VarDef* root){

    log(root);

    PARSE_TOKEN(IDENFR);

    while (CUR_TOKEN_IS(LBRACK)){

        PARSE_TOKEN(LBRACK);

        PARSE(constexp, ConstExp);

        PARSE_TOKEN(RBRACK);
    }

    if (CUR_TOKEN_IS(ASSIGN)){

        PARSE_TOKEN(ASSIGN);

        PARSE(initval, InitVal);
    }

    return true;
}


// InitVal -> Exp | '{' [ InitVal { ',' InitVal } ] '}'
bool Parser::parseInitVal(InitVal* root){

    log(root);

    if (in_unaryexp()){

        PARSE(exp, Exp);

    }else if (CUR_TOKEN_IS(LBRACE)){

        PARSE_TOKEN(LBRACE);

        if (in_unaryexp() || CUR_TOKEN_IS(LBRACE)){

            PARSE(initval, InitVal);

            while (CUR_TOKEN_IS(COMMA)){

                PARSE_TOKEN(COMMA);

                PARSE(initval, InitVal);
            }
        }

        PARSE_TOKEN(RBRACE);

    }

    return true;
}


// FuncDef -> FuncType Ident '(' [FuncFParams] ')' Block
bool Parser::parseFuncDef(FuncDef* root){

    log(root);

    PARSE(functype, FuncType)

    PARSE_TOKEN(IDENFR);

    PARSE_TOKEN(LPARENT);

    if (CUR_TOKEN_IS(INTTK) || CUR_TOKEN_IS(FLOATTK)){

        PARSE(funcfparms, FuncFParams);
    }

    PARSE_TOKEN(RPARENT);

    PARSE(block, Block);

    return true;
}


// FuncType -> 'void' | 'int' | 'float'
bool Parser::parseFuncType(FuncType* root){

    log(root);
    
    if (CUR_TOKEN_IS(VOIDTK)){

        PARSE_TOKEN(VOIDTK);

    }else if (CUR_TOKEN_IS(INTTK)){

        PARSE_TOKEN(INTTK);

    }else if (CUR_TOKEN_IS(FLOATTK)){

        PARSE_TOKEN(FLOATTK);
    }
    
    return true;
}


// FuncFParam -> BType Ident ['[' ']' { '[' Exp ']' }]
bool Parser::parseFuncFParam(FuncFParam* root){

    log(root);

    PARSE(btype, BType);

    PARSE_TOKEN(IDENFR);

    if (CUR_TOKEN_IS(LBRACK)){

        PARSE_TOKEN(LBRACK);

        PARSE_TOKEN(RBRACK);

        while (CUR_TOKEN_IS(LBRACK)){

            PARSE_TOKEN(LBRACK);

            PARSE(exp, Exp);
            
            PARSE_TOKEN(RBRACK);
        }
    }

    return true;
}


// FuncFParams -> FuncFParam { ',' FuncFParam }
bool Parser::parseFuncFParams(FuncFParams* root){

    log(root);

    PARSE(funcfparam, FuncFParam);

    while (CUR_TOKEN_IS(COMMA)){

        PARSE_TOKEN(COMMA);

        PARSE(funcfparam, FuncFParam);
    }

    return true;
}


// Block -> '{' { BlockItem } '}'
bool Parser::parseBlock(Block* root){

    log(root);

    PARSE_TOKEN(LBRACE);

    while (CUR_TOKEN_IS(CONSTTK) || CUR_TOKEN_IS(INTTK) || CUR_TOKEN_IS(FLOATTK) || in_stmt()){

        PARSE(blockitem, BlockItem);
    }

    PARSE_TOKEN(RBRACE);

    return true;
}


// BlockItem -> Decl | Stmt
bool Parser::parseBlockItem(BlockItem* root){

    log(root);

    if (CUR_TOKEN_IS(CONSTTK) || CUR_TOKEN_IS(INTTK) || CUR_TOKEN_IS(FLOATTK)){

        PARSE(decl, Decl);

    }else if (in_stmt()){

        PARSE(stmt, Stmt);

    }

    return true;
}


// Stmt -> LVal '=' Exp ';' | Block | 'if' '(' Cond ')' Stmt [ 'else' Stmt ] | 'while' '(' Cond ')' Stmt | 'break' ';' | 'continue' ';' | 'return' [Exp] ';' | [Exp] ';'
bool Parser::parseStmt(Stmt* root){

    log(root);

    if (CUR_TOKEN_IS(IDENFR)){
        
        if (token_stream[index+1].type == TokenType::ASSIGN || token_stream[index+1].type == TokenType::LBRACK){
            
            PARSE(lval, LVal);

            PARSE_TOKEN(ASSIGN);

            PARSE(exp, Exp);

            PARSE_TOKEN(SEMICN);

        }else{

            PARSE(exp, Exp);

            PARSE_TOKEN(SEMICN);
        }

    }else if (CUR_TOKEN_IS(LBRACE)){

        PARSE(block, Block);

    }else if (CUR_TOKEN_IS(IFTK)){

        PARSE_TOKEN(IFTK);
        
        PARSE_TOKEN(LPARENT);

        PARSE(cond, Cond);

        PARSE_TOKEN(RPARENT);

        PARSE(stmt, Stmt);

        if (CUR_TOKEN_IS(ELSETK)){

            PARSE_TOKEN(ELSETK);

            PARSE(stmt, Stmt);
        }

    }else if (CUR_TOKEN_IS(WHILETK)){

        PARSE_TOKEN(WHILETK);

        PARSE_TOKEN(LPARENT);

        PARSE(cond, Cond);

        PARSE_TOKEN(RPARENT);

        PARSE(stmt, Stmt);

    }else if (CUR_TOKEN_IS(BREAKTK)){

        PARSE_TOKEN(BREAKTK);

        PARSE_TOKEN(SEMICN);

    }else if (CUR_TOKEN_IS(CONTINUETK)){

        PARSE_TOKEN(CONTINUETK);

        PARSE_TOKEN(SEMICN);

    }else if (CUR_TOKEN_IS(RETURNTK)){
        
        PARSE_TOKEN(RETURNTK);

        if (in_unaryexp()){

            PARSE(exp, Exp);
        }

        PARSE_TOKEN(SEMICN);

    }else if (CUR_TOKEN_IS(SEMICN)){

        PARSE_TOKEN(SEMICN);

    }else if (in_unaryexp() && !CUR_TOKEN_IS(IDENFR)){

        PARSE(exp, Exp);

        PARSE_TOKEN(SEMICN);

    }

    return true;
}


// Exp -> AddExp
bool Parser::parseExp(Exp* root){

    log(root);

    PARSE(addexp, AddExp);

    return true;
}


// Cond -> LOrExp
bool Parser::parseCond(Cond* root){

    log(root);

    PARSE(lorexp, LOrExp);

    return true;
}


// LVal -> Ident {'[' Exp ']'}
bool Parser::parseLVal(LVal* root){

    log(root);

    PARSE_TOKEN(IDENFR);

    while (CUR_TOKEN_IS(LBRACK)){

        PARSE_TOKEN(LBRACK);

        PARSE(exp, Exp);

        PARSE_TOKEN(RBRACK);
    }

    return true;
}


// Number -> IntConst | floatConst
bool Parser::parseNumber(Number* root){

    log(root);

    if (CUR_TOKEN_IS(INTLTR)){

        PARSE_TOKEN(INTLTR);

    }else if (CUR_TOKEN_IS(FLOATLTR)){

        PARSE_TOKEN(FLOATLTR);

    }

    return true;
}


// PrimaryExp -> '(' Exp ')' | LVal | Number
bool Parser::parsePrimaryExp(PrimaryExp* root){

    log(root);

    if (CUR_TOKEN_IS(LPARENT)){
        
        PARSE_TOKEN(LPARENT);

        PARSE(exp, Exp);

        PARSE_TOKEN(RPARENT);

    }else if (CUR_TOKEN_IS(IDENFR)){

        PARSE(lval, LVal);

    }else if (CUR_TOKEN_IS(INTLTR) || CUR_TOKEN_IS(FLOATLTR)){

        PARSE(number, Number);
    }

    return true;
}


// UnaryExp -> PrimaryExp | Ident '(' [FuncRParams] ')' | UnaryOp UnaryExp
bool Parser::parseUnaryExp(UnaryExp* root){

    log(root);

    if (CUR_TOKEN_IS(IDENFR)){

        if (token_stream[index+1].type == TokenType::LPARENT){

            PARSE_TOKEN(IDENFR);

            PARSE_TOKEN(LPARENT);

            if (in_unaryexp()){

                PARSE(funcrparams, FuncRParams);
            }

            PARSE_TOKEN(RPARENT);

        }else{

            PARSE(primaryexp, PrimaryExp);
        }

    }else if (CUR_TOKEN_IS(LPARENT) || CUR_TOKEN_IS(INTLTR) || CUR_TOKEN_IS(FLOATLTR)){

        PARSE(primaryexp, PrimaryExp);

    }else if (CUR_TOKEN_IS(PLUS) || CUR_TOKEN_IS(MINU) || CUR_TOKEN_IS(NOT)){

        PARSE(unaryop, UnaryOp);

        PARSE(unaryexp, UnaryExp);
    }

    return true;
}


// UnaryOp -> '+' | '-' | '!'
bool Parser::parseUnaryOp(UnaryOp* root){

    log(root);

    if (CUR_TOKEN_IS(PLUS)){

        PARSE_TOKEN(PLUS);

    }else if (CUR_TOKEN_IS(MINU)){

        PARSE_TOKEN(MINU);

    }else if (CUR_TOKEN_IS(NOT)){

        PARSE_TOKEN(NOT);
    }

    return true;
}


// FuncRParams -> Exp { ',' Exp }
bool Parser::parseFuncRParams(FuncRParams* root){

    log(root);

    PARSE(exp, Exp);

    while (CUR_TOKEN_IS(COMMA)){

        PARSE_TOKEN(COMMA);

        PARSE(exp, Exp);
    }

    return true;
}


// MulExp -> UnaryExp { ('*' | '/' | '%') UnaryExp }
bool Parser::parseMulExp(MulExp* root){

    log(root);

    PARSE(unaryexp, UnaryExp);

    while (CUR_TOKEN_IS(MULT) || CUR_TOKEN_IS(DIV) || CUR_TOKEN_IS(MOD)){

        if (CUR_TOKEN_IS(MULT)){
            
            PARSE_TOKEN(MULT);

        }else if (CUR_TOKEN_IS(DIV)){

            PARSE_TOKEN(DIV);

        }else if (CUR_TOKEN_IS(MOD)){

            PARSE_TOKEN(MOD);
        }

        PARSE(unaryexp, UnaryExp);
    }

    return true;
}


// AddExp -> MulExp { ('+' | '-') MulExp }
bool Parser::parseAddExp(AddExp* root){

    log(root);

    PARSE(mulexp, MulExp);

    while (CUR_TOKEN_IS(PLUS) || CUR_TOKEN_IS(MINU)){

        if (CUR_TOKEN_IS(PLUS)){

            PARSE_TOKEN(PLUS);

        }else if (CUR_TOKEN_IS(MINU)){

            PARSE_TOKEN(MINU);
        }

        PARSE(mulexp, MulExp);
    }

    return true;

}

// RelExp -> AddExp { ('<' | '>' | '<=' | '>=') AddExp }
bool Parser::parseRelExp(RelExp* root){

    log(root);

    PARSE(addexp, AddExp);

    while (CUR_TOKEN_IS(LSS) || CUR_TOKEN_IS(GTR) || CUR_TOKEN_IS(LEQ) || CUR_TOKEN_IS(GEQ)){

        if (CUR_TOKEN_IS(LSS)){

            PARSE_TOKEN(LSS);

        }else if (CUR_TOKEN_IS(GTR)){

            PARSE_TOKEN(GTR);

        }else if (CUR_TOKEN_IS(LEQ)){

            PARSE_TOKEN(LEQ);

        }else if (CUR_TOKEN_IS(GEQ)){

            PARSE_TOKEN(GEQ);
        }

        PARSE(addexp, AddExp);
    }

    return true;
}


// EqExp -> RelExp { ('==' | '!=') RelExp }
bool Parser::parseEqExp(EqExp* root){

    log(root);

    PARSE(relexp, RelExp);

    while (CUR_TOKEN_IS(EQL) || CUR_TOKEN_IS(NEQ)){

        if (CUR_TOKEN_IS(EQL)){
            
            PARSE_TOKEN(EQL);

        }else if (CUR_TOKEN_IS(NEQ)){

            PARSE_TOKEN(NEQ);
        }

        PARSE(relexp, RelExp);
    }

    return true;
}


// LAndExp -> EqExp [ '&&' LAndExp ]
bool Parser::parseLAndExp(LAndExp* root){

    log(root);

    PARSE(eqexp, EqExp);

    if (CUR_TOKEN_IS(AND)){

        PARSE_TOKEN(AND);

        PARSE(landexp, LAndExp);
    }

    return true;
}


// LOrExp -> LAndExp [ '||' LOrExp ]
bool Parser::parseLOrExp(LOrExp* root){

    log(root);

    PARSE(landexp, LAndExp);

    if (CUR_TOKEN_IS(OR)){

        PARSE_TOKEN(OR);

        PARSE(lorexp, LOrExp);
    }

    return true;
    
}


// ConstExp -> AddExp
bool Parser::parseConstExp(ConstExp* root){

    log(root);

    PARSE(addexp, AddExp);

    return true;
}


void Parser::log(AstNode* node){
    #ifdef DEBUG_PARSER
        std::cout << "in parse" << toString(node->type) << ", cur_token_type::" << toString(token_stream[index].type) << ", token_val::" << token_stream[index].value << '\n';
    #endif
}
