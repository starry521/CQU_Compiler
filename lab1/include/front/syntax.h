/**
 * @file syntax.h
 * @author Yuntao Dai (d1581209858@live.com)
 * @brief 
 * in the second part, we already has a token stream, now we should analysis it and result in a syntax tree, 
 * which we also called it AST(abstract syntax tree)
 * @version 0.1
 * @date 2022-12-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef SYNTAX_H
#define SYNTAX_H

#include"front/abstract_syntax_tree.h"
#include"front/token.h"

#include<vector>

namespace frontend {

// definition of Parser
// a parser should take a token stream as input, then parsing it, output a AST
struct Parser {
    uint32_t index; // current token index
    const std::vector<Token>& token_stream;

    /**
     * @brief constructor
     * @param tokens: the input token_stream
     */
    Parser(const std::vector<Token>& tokens);

    /**
     * @brief destructor
     */
    ~Parser();
    
    /**
     * @brief creat the abstract syntax tree
     * @return the root of abstract syntax tree
     */
    CompUnit* get_abstract_syntax_tree();
    
    //自定义转换函数
    bool in_unaryexp();
    bool in_stmt();
    Term* parseTerm(AstNode* parent, TokenType expected);
    bool parseCompUnit(CompUnit* root);
    bool parseDecl(Decl* root);
    bool parseConstDecl(ConstDecl* root);
    bool parseBType(BType* root);
    bool parseConstDef(ConstDef* root);
    bool parseConstInitVal(ConstInitVal* root);
    bool parseVarDecl(VarDecl* root);
    bool parseVarDef(VarDef* root);
    bool parseInitVal(InitVal* root);
    bool parseFuncDef(FuncDef* root);
    bool parseFuncType(FuncType* root);
    bool parseFuncFParam(FuncFParam* root);
    bool parseFuncFParams(FuncFParams* root);
    bool parseBlock(Block* root);
    bool parseBlockItem(BlockItem* root);
    bool parseStmt(Stmt* root);
    bool parseExp(Exp* root);
    bool parseCond(Cond* root);
    bool parseLVal(LVal* root);
    bool parseNumber(Number* root);
    bool parsePrimaryExp(PrimaryExp* root);
    bool parseUnaryExp(UnaryExp* root);
    bool parseUnaryOp(UnaryOp* root);
    bool parseFuncRParams(FuncRParams* root);
    bool parseMulExp(MulExp* root);
    bool parseAddExp(AddExp* root);
    bool parseRelExp(RelExp* root);
    bool parseEqExp(EqExp* root);
    bool parseLAndExp(LAndExp* root);
    bool parseLOrExp(LOrExp* root);
    bool parseConstExp(ConstExp* root);

    /**
     * @brief for debug, should be called in the beginning of recursive descent functions 
     * @param node: current parsing node 
     */
    void log(AstNode* node);
};

} // namespace frontend

#endif