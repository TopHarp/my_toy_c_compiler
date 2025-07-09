/*
 * Copyright (c) [2025] [TopHarp]
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef _DECLARATIONG_PARSER_H_
#define _DECLARATIONG_PARSER_H_

#include <memory>
#include "token_type.h"


// class Token;
class CTokenLexer;


class CParser
{
public:
    CParser(){};
    ~CParser(){};


public:
    int ParseCode(CTokenLexer &token_lexer);
private:
    int expr_currnt_expression_type_;

private:
    std::unique_ptr<Token> ParseEnumDeclaration(CTokenLexer &token_lexer);

    void EatLeftOfLine(std::unique_ptr<Token> &next_token, eVarType &base_type, CTokenLexer &token_lexer);
    
    void EatPointor(CTokenLexer &token_lexer,
                         std::unique_ptr<Token> &next_token, 
                         int &main_currnt_expression_type);
    
    int FunctionOrIdentifier(std::unique_ptr<Token> &next_token, 
                       int &main_currnt_expression_type,
                       CTokenLexer &token_lexer);

    int EatFunction(int &main_currnt_expression_type, 
                              int &param_index,
                              CTokenLexer &token_lexer,
                              std::unique_ptr<Token> &next_token);
    
    int EatFunctionParam(int &param_index,
                         CTokenLexer &token_lexer,
                         std::unique_ptr<Token> &next_token);


    int EatFuntionLocParam(int &param_index,
                              CTokenLexer &token_lexer,
                              std::unique_ptr<Token> &next_token);

    
    void StatementParse(CTokenLexer &token_lexer,
                        std::unique_ptr<Token> &next_token);
    
    /**
     * @brief 
     * 
     * @param token_leve 运算符类型 eToken_t中按照优先级排序 越大优先级越高
     * @param next_token 
     * @param token_lexer 
     */
    void ExpressionParse(int leva_token,
                            CTokenLexer &token_lexer,
                            std::unique_ptr<Token> &next_token);

    void ExpressionNum(CTokenLexer &token_lexer,
                            std::unique_ptr<Token> &next_token,
                            int &expr_currnt_expression_type);
    
    void ExpressionString(CTokenLexer &token_lexer,
                            std::unique_ptr<Token> &next_token,
                            int &expr_currnt_expression_type);

    
    void ExpressionSizeof(CTokenLexer &token_lexer,
                            std::unique_ptr<Token> &next_token,
                            int &expr_currnt_expression_type);

    void ExpressionID(CTokenLexer &token_lexer,
                            std::unique_ptr<Token> &next_token,
                            int &expr_currnt_expression_type);
    

    void ExpressionTypeTrans(CTokenLexer &token_lexer,
                            std::unique_ptr<Token> &next_token,
                            int &expr_currnt_expression_type);

    void ExpressionPointorValue(CTokenLexer &token_lexer,
                            std::unique_ptr<Token> &next_token,
                            int &expr_currnt_expression_type);
    
    void ExpressionAddress(CTokenLexer &token_lexer,
                            std::unique_ptr<Token> &next_token,
                            int &expr_currnt_expression_type);

    void ExpressionIncDec(CTokenLexer &token_lexer,
                            std::unique_ptr<Token> &next_token,
                            int &expr_currnt_expression_type);
    
    
    void TopToDownPrecedence(int leva_token,
                                CTokenLexer &token_lexer,
                                std::unique_ptr<Token> &next_token,
                                int &expr_currnt_expression_type);

                            


};







#endif