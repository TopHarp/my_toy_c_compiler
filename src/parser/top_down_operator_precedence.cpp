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
#include "parser.h"
#include "identifier_map.h"
#include "token.h"
#include "error_code.h"
#include "debug_print.h"
#include "data_stack_area.h"
#include "system_operetor.h"


void CParser::TopToDownPrecedence(int leva_token,
                                    CTokenLexer &token_lexer,
                                    std::unique_ptr<Token> &next_token,
                                    int &expr_currnt_expression_type)
{
    int tmp_type = 0;

    while (next_token->type >= leva_token)
    {
        ///< ty在递归过程中可能会改变,所以备份当前处理的表达式类型
        tmp_type = expr_currnt_expression_type;
        if (next_token->type == TOK_ASSIGN)   ///< 赋值
        {
            long int *now_emitted_code = (DataStackArea::NowPosEmittedCode());
            if ((*now_emitted_code == LC) 
                    || (*now_emitted_code == LI))   ///< tested
            {
                *now_emitted_code = PSH;
            }
            else
            {
                Debug_Error("unexpected next token\n");
                exit(0);
            }
            next_token = token_lexer.NextToken();
            ExpressionParse(TOK_ASSIGN, token_lexer, next_token);
            ///< *++e = ((ty = t) == CHAR) ? SC : SI;//取得右值expr的值,作为a=expr的结果
            expr_currnt_expression_type = tmp_type;
            long int op = (expr_currnt_expression_type  == TYPE_CHAR)
                                ? (SC) : (SI);
            DataStackArea::AddOneEmittedCode(op);                 ///< tested
        }
        else if (next_token->type == TOK_COND)
        {
            next_token = token_lexer.NextToken();
            DataStackArea::AddOneEmittedCode(BZ);
            long int *tmp_emitted_code = DataStackArea::UseEmittedCodeArea(1);
            ExpressionParse(TOK_ASSIGN, token_lexer, next_token);
            if (next_token->last_alpha != ':')
            {
                Debug_Error("unexpected next token\n");
                exit(0);                
            }
            next_token = token_lexer.NextToken();
            *tmp_emitted_code = (long int)DataStackArea::WatchEmittedCodePos(3);
            DataStackArea::AddOneEmittedCode(JMP);
            tmp_emitted_code = DataStackArea::UseEmittedCodeArea(1);
            ExpressionParse(TOK_COND, token_lexer, next_token);
            *tmp_emitted_code = (long int)DataStackArea::WatchEmittedCodePos(1);
        }
        else if (next_token->type == TOK_LOR)
        {
            next_token = token_lexer.NextToken();
            DataStackArea::AddOneEmittedCode(BNZ);
            long int *tmp_emitted_code = DataStackArea::UseEmittedCodeArea(1);
            ExpressionParse(TOK_LAN, token_lexer, next_token);
            *tmp_emitted_code = (long int)DataStackArea::WatchEmittedCodePos(1);
            expr_currnt_expression_type = TYPE_INT;
        }
        else if (next_token->type == TOK_LAN)
        {
            next_token = token_lexer.NextToken();
            DataStackArea::AddOneEmittedCode(BZ);
            long int *tmp_emitted_code = DataStackArea::UseEmittedCodeArea(1);
            ExpressionParse(TOK_OR, token_lexer, next_token);
            *tmp_emitted_code = (long int)DataStackArea::WatchEmittedCodePos(1);
            expr_currnt_expression_type = TYPE_INT;            
        }
        else if (next_token->type == TOK_OR)
        {
            next_token = token_lexer.NextToken();
            DataStackArea::AddOneEmittedCode(PSH);
            ExpressionParse(TOK_XOR, token_lexer, next_token);
            DataStackArea::AddOneEmittedCode(OR);
            expr_currnt_expression_type = TYPE_INT;
        }
        else if (next_token->type == TOK_XOR)
        {
            next_token = token_lexer.NextToken();
            DataStackArea::AddOneEmittedCode(PSH);
            ExpressionParse(TOK_AND, token_lexer, next_token);
            DataStackArea::AddOneEmittedCode(XOR);
            expr_currnt_expression_type = TYPE_INT;
        }
        else if (next_token->type == TOK_AND)
        {
            next_token = token_lexer.NextToken();
            DataStackArea::AddOneEmittedCode(PSH);
            ExpressionParse(TOK_EQ, token_lexer, next_token);
            DataStackArea::AddOneEmittedCode(AND);
            expr_currnt_expression_type = TYPE_INT;
        }
        else if (next_token->type == TOK_EQ)
        {
            next_token = token_lexer.NextToken();
            DataStackArea::AddOneEmittedCode(PSH);
            ExpressionParse(TOK_LT, token_lexer, next_token);
            DataStackArea::AddOneEmittedCode(EQ);
            expr_currnt_expression_type = TYPE_INT;
        }
        else if (next_token->type == TOK_NE)
        {
            next_token = token_lexer.NextToken();
            DataStackArea::AddOneEmittedCode(PSH);
            ExpressionParse(TOK_LT, token_lexer, next_token);
            DataStackArea::AddOneEmittedCode(NE);
            expr_currnt_expression_type = TYPE_INT;
        }
        else if (next_token->type == TOK_LT)
        {
            next_token = token_lexer.NextToken();
            DataStackArea::AddOneEmittedCode(PSH);
            ExpressionParse(TOK_SHL, token_lexer, next_token);
            DataStackArea::AddOneEmittedCode(LT);
            expr_currnt_expression_type = TYPE_INT;
        }
        else if (next_token->type == TOK_GT)
        {
            next_token = token_lexer.NextToken();
            DataStackArea::AddOneEmittedCode(PSH);
            ExpressionParse(TOK_SHL, token_lexer, next_token);
            DataStackArea::AddOneEmittedCode(GT);
            expr_currnt_expression_type = TYPE_INT;
        }
        else if (next_token->type == TOK_LE)
        {
            next_token = token_lexer.NextToken();
            DataStackArea::AddOneEmittedCode(PSH);
            ExpressionParse(TOK_SHL, token_lexer, next_token);
            DataStackArea::AddOneEmittedCode(LE);
            expr_currnt_expression_type = TYPE_INT;
        }
        else if (next_token->type == TOK_GE)
        {
            next_token = token_lexer.NextToken();
            DataStackArea::AddOneEmittedCode(PSH);
            ExpressionParse(TOK_SHL, token_lexer, next_token);
            DataStackArea::AddOneEmittedCode(GE);
            expr_currnt_expression_type = TYPE_INT;
        }
        else if (next_token->type == TOK_SHL)
        {
            next_token = token_lexer.NextToken();
            DataStackArea::AddOneEmittedCode(PSH);
            ExpressionParse(TOK_ADD, token_lexer, next_token);
            DataStackArea::AddOneEmittedCode(SHL);
            expr_currnt_expression_type = TYPE_INT;
        }
        else if (next_token->type == TOK_SHR)
        {
            next_token = token_lexer.NextToken();
            DataStackArea::AddOneEmittedCode(PSH);
            ExpressionParse(TOK_ADD, token_lexer, next_token);
            DataStackArea::AddOneEmittedCode(SHR);
            expr_currnt_expression_type = TYPE_INT;
        }
        else if (next_token->type == TOK_ADD)
        {      
            next_token = token_lexer.NextToken();
            DataStackArea::AddOneEmittedCode(PSH);
            ExpressionParse(TOK_MUL, token_lexer, next_token);
            if ((expr_currnt_expression_type = tmp_type) > TYPE_PTR)
            {
                DataStackArea::AddOneEmittedCode(PSH);
                DataStackArea::AddOneEmittedCode(IMM);
                DataStackArea::AddOneEmittedCode(sizeof(long int));
                DataStackArea::AddOneEmittedCode(MUL);
            }
            DataStackArea::AddOneEmittedCode(ADD);
        }
        else if (next_token->type == TOK_SUB)
        {
            next_token = token_lexer.NextToken();
            DataStackArea::AddOneEmittedCode(PSH);
            ExpressionParse(TOK_MUL, token_lexer, next_token);
            if (tmp_type > TYPE_PTR 
                 && tmp_type == expr_currnt_expression_type)
            {
                DataStackArea::AddOneEmittedCode(SUB);
                DataStackArea::AddOneEmittedCode(PSH);
                DataStackArea::AddOneEmittedCode(IMM);
                DataStackArea::AddOneEmittedCode(sizeof(int));///< TODO:: ??
                DataStackArea::AddOneEmittedCode(DIV);
                expr_currnt_expression_type = TYPE_INT;
            }
            else if ((expr_currnt_expression_type = tmp_type) > TYPE_PTR)
            {
                DataStackArea::AddOneEmittedCode(PSH);
                DataStackArea::AddOneEmittedCode(IMM);
                DataStackArea::AddOneEmittedCode(sizeof(int));///< TODO:: ??
                DataStackArea::AddOneEmittedCode(MUL);
                DataStackArea::AddOneEmittedCode(SUB);
            }
            else
            {
                DataStackArea::AddOneEmittedCode(SUB);
            }
        }
        else if (next_token->type == TOK_MUL)
        {
            next_token = token_lexer.NextToken();
            DataStackArea::AddOneEmittedCode(PSH);
            ExpressionParse(TOK_INC, token_lexer, next_token);
            DataStackArea::AddOneEmittedCode(MUL);
            expr_currnt_expression_type = TYPE_INT;
        }
        else if (next_token->type == TOK_DIV)
        {
            next_token = token_lexer.NextToken();
            DataStackArea::AddOneEmittedCode(PSH);
            ExpressionParse(TOK_INC, token_lexer, next_token);
            DataStackArea::AddOneEmittedCode(DIV);
            expr_currnt_expression_type = TYPE_INT;
        }
        else if (next_token->type == TOK_MOD)
        {
            next_token = token_lexer.NextToken();
            DataStackArea::AddOneEmittedCode(PSH);
            ExpressionParse(TOK_INC, token_lexer, next_token);
            DataStackArea::AddOneEmittedCode(MOD);
            expr_currnt_expression_type = TYPE_INT;
        }

        else if ((next_token->type == TOK_INC)
                || (next_token->type == TOK_DEC))
        {
            if (*(DataStackArea::NowPosEmittedCode()) == LC)
            {
                *(DataStackArea::NowPosEmittedCode()) = PSH;
                DataStackArea::AddOneEmittedCode(LC);
            }
            else if (*(DataStackArea::NowPosEmittedCode()) == LI)
            {
                *(DataStackArea::NowPosEmittedCode()) = PSH;
                DataStackArea::AddOneEmittedCode(LI);
            }
            else
            {
                Debug_Error("unexpected next token\n");
                exit(0); 
            }
            DataStackArea::AddOneEmittedCode(PSH);
            DataStackArea::AddOneEmittedCode(IMM);
            DataStackArea::AddOneEmittedCode(
                (expr_currnt_expression_type > TYPE_PTR) 
                ? (sizeof(int)) : (sizeof(char)));
            DataStackArea::AddOneEmittedCode(
                (next_token->type == TOK_INC) 
                ? (ADD) : (SUB));
            DataStackArea::AddOneEmittedCode(
                (expr_currnt_expression_type == TYPE_CHAR) 
                ? (SC) : (SI));
            DataStackArea::AddOneEmittedCode(PSH);
            DataStackArea::AddOneEmittedCode(IMM);
            DataStackArea::AddOneEmittedCode(
                (expr_currnt_expression_type > TYPE_PTR) 
                ? (sizeof(int)) : (sizeof(char)));
            DataStackArea::AddOneEmittedCode(
                (next_token->type == TOK_INC) 
                ? (SUB) : (ADD));
            next_token = token_lexer.NextToken();
        }
        else if (next_token->type == TOK_BRAK)
        {
            next_token = token_lexer.NextToken();
            DataStackArea::AddOneEmittedCode(PSH);
            ExpressionParse(TOK_ASSIGN, token_lexer, next_token);
            if (next_token->last_alpha != ']')
            {
                Debug_Error("unexpected next token\n");
                exit(0);
            }
            next_token = token_lexer.NextToken();
            if (tmp_type > TYPE_PTR)
            {
                DataStackArea::AddOneEmittedCode(PSH);
                DataStackArea::AddOneEmittedCode(IMM);
                DataStackArea::AddOneEmittedCode(sizeof(int)); ///< TODO:: ??
                DataStackArea::AddOneEmittedCode(MUL);
            }
            else if (tmp_type < TYPE_PTR)
            {
                Debug_Error("unexpected next token\n");
                exit(0);                
            }
            DataStackArea::AddOneEmittedCode(ADD);
            expr_currnt_expression_type = tmp_type - TYPE_PTR;
            DataStackArea::AddOneEmittedCode((expr_currnt_expression_type == TYPE_CHAR)
                                            ? (LC) : (LI));
        }
        else
        {
                Debug_Error("unexpected next token\n");
                exit(0);
       }
    }

    return;
}









