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

void CParser::StatementParse(CTokenLexer &token_lexer,
                             std::unique_ptr<Token> &next_token)
{
    long int *p_while_begin_addr = nullptr;             ///< a
    long int *p_branch_false_jmp_addr = nullptr;         ///< b

    if (next_token->type == TOK_IF)
    {
        next_token = token_lexer.NextToken();
        if (next_token->last_alpha != '(')
        {
            Debug_Error("error (if) : type %d alpha:%c\n",
                            next_token->type, next_token->last_alpha);
            exit(0);
        }

        next_token = token_lexer.NextToken();
        ExpressionParse((int)TOK_ASSIGN, token_lexer, 
                                            next_token);       ///< if() 中判断
        
        if (next_token->last_alpha != ')')
        {
            Debug_Error("error (if) : type %d alpha:%c\n",
                            next_token->type, next_token->last_alpha);
            exit(0);
        }
        DataStackArea::AddOneEmittedCode((long int)BZ);   ///< branch if cond zero
        p_branch_false_jmp_addr = DataStackArea::UseEmittedCodeArea(1);    ///< 待写入的条件跳转的地址
        next_token = token_lexer.NextToken();
        StatementParse(token_lexer, next_token);            ///< continue parse

        if (next_token->type == TOK_ELSE)
        {
            *p_branch_false_jmp_addr = (long int)DataStackArea::WatchEmittedCodePos(3); ///< 
            DataStackArea::AddOneEmittedCode((long int)JMP);
            p_branch_false_jmp_addr = DataStackArea::UseEmittedCodeArea(1);
            next_token = token_lexer.NextToken();
            StatementParse(token_lexer, next_token);
        }
        *p_branch_false_jmp_addr = (long int)DataStackArea::WatchEmittedCodePos(1);
    }
    else if (next_token->type == TOK_WHILE)
    {
        next_token = token_lexer.NextToken();
        p_while_begin_addr = DataStackArea::WatchEmittedCodePos(1); ///< a = e + 1
        if (next_token->last_alpha != '(')
        {
            Debug_Error("error (if) : type %d alpha:%c\n",
                            next_token->type, next_token->last_alpha);
            exit(0);
        }
        next_token = token_lexer.NextToken();
        ExpressionParse((int)TOK_ASSIGN, token_lexer, 
                                            next_token);       ///< if() 中判断
        
        if (next_token->last_alpha != ')')
        {
            Debug_Error("error (if) : type %d alpha:%c\n",
                            next_token->type, next_token->last_alpha);
            exit(0);
        }        
        next_token = token_lexer.NextToken();

        DataStackArea::AddOneEmittedCode((long int)BZ);
        p_branch_false_jmp_addr = DataStackArea::UseEmittedCodeArea(1);
        StatementParse(token_lexer, next_token);
        DataStackArea::AddOneEmittedCode((long int)JMP);
        DataStackArea::AddOneEmittedCode((long int)p_while_begin_addr);
        *p_branch_false_jmp_addr = (long int)DataStackArea::WatchEmittedCodePos(1);
    }
    else if (next_token->type == TOK_RETURN)    ///< tested
    {
        next_token = token_lexer.NextToken();
        if (next_token->last_alpha != ';')
        {
            ExpressionParse((int)TOK_ASSIGN, token_lexer, next_token);
        }
        DataStackArea::AddOneEmittedCode((long int)LEV);
        if (next_token->last_alpha == ';')
        {
            next_token = token_lexer.NextToken();
        }
        else
        {
            Debug_Error("error (return) : type %d alpha:%c\n",
                            next_token->type, next_token->last_alpha);
            exit(0);
        }
    }
    else if (next_token->last_alpha == '{')
    {
        next_token = token_lexer.NextToken();   ///< tested  {(empty)}
        while (next_token->last_alpha != '}')
        {
            StatementParse(token_lexer, next_token);
        }
        next_token = token_lexer.NextToken();
    }
    else if (next_token->last_alpha == ';')
    {
        next_token = token_lexer.NextToken();
    }
    else
    {
        ExpressionParse(TOK_ASSIGN, token_lexer, next_token);
        if (next_token->last_alpha == ';')
        {
            next_token = token_lexer.NextToken();
        }
        else
        {
            Debug_Error("error () : type %d alpha:%c\n",
                            next_token->type, next_token->last_alpha);
            exit(0);
        }
    }

    return;
}








