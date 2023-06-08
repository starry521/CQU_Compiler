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
    
    /**
     * @brief for debug, should be called in the beginning of recursive descent functions 
     * @param node: current parsing node 
     */
    void log(AstNode* node);
};

} // namespace frontend

#endif