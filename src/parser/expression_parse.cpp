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
#include "system_operetor.h"
#include "data_stack_area.h"

void CParser::ExpressionParse(int leva_token,
                            CTokenLexer &token_lexer,
                            std::unique_ptr<Token> &next_token)
{
    // int expr_currnt_expression_type = 0;

    if (next_token == nullptr)
    {
        Debug_Error("unexpected next token\n");
        exit(0);
    }
    else if (next_token->type == TOK_NUM)
    {
        ExpressionNum(token_lexer, next_token,
                        expr_currnt_expression_type_);
    }
    else if (next_token->type == TOK_STRING)            ///< tested
    {
        ExpressionString(token_lexer, next_token,
                        expr_currnt_expression_type_);
    }
    else if (next_token->type == TOK_SIZEOF)
    {
        ExpressionSizeof(token_lexer, next_token,
                        expr_currnt_expression_type_);        
    }
    else if (next_token->type == TOK_ID)
    {
        ExpressionID(token_lexer, next_token,
                        expr_currnt_expression_type_);
    }
    else if (next_token->last_alpha == '(')         ///< 类型转换 或一般括号
    {
        ExpressionTypeTrans(token_lexer, next_token,
                            expr_currnt_expression_type_);        
    }
    else if (next_token->type == TOK_MUL)           ///< pointor
    {
        ExpressionPointorValue(token_lexer, next_token,
                                    expr_currnt_expression_type_);
    }
    else if (next_token->type == TOK_AND)
    {
        ExpressionAddress(token_lexer, next_token,
                            expr_currnt_expression_type_);
    }
    else if (next_token->last_alpha == '!')
    {
        next_token = token_lexer.NextToken();
        ExpressionParse(TOK_INC, token_lexer, next_token);
        DataStackArea::AddOneEmittedCode(PSH);
        DataStackArea::AddOneEmittedCode(IMM);
        DataStackArea::AddOneEmittedCode(0);
        DataStackArea::AddOneEmittedCode(EQ);   ///< !x 相当于x==0
        expr_currnt_expression_type_ = TYPE_INT;
    }
    else if (next_token->last_alpha == '~')
    {
        next_token = token_lexer.NextToken();
        ExpressionParse(TOK_INC, token_lexer, next_token);
        DataStackArea::AddOneEmittedCode(PSH);
        DataStackArea::AddOneEmittedCode(IMM);
        DataStackArea::AddOneEmittedCode(-1);
        DataStackArea::AddOneEmittedCode(XOR);   ///< ~x 相当于x ^ -1
        expr_currnt_expression_type_ = TYPE_INT;
    }
    else if (next_token->type == TOK_ADD)
    {
        next_token = token_lexer.NextToken();
        ExpressionParse(TOK_INC, token_lexer, next_token);
        expr_currnt_expression_type_ = TYPE_INT;        
    }
    else if (next_token->type == TOK_SUB)
    {
        next_token = token_lexer.NextToken();
        DataStackArea::AddOneEmittedCode(IMM);
        if (next_token->type == TOK_NUM)
        {
            DataStackArea::AddOneEmittedCode(
                        -(static_cast<NumberToken*>(next_token.get())
                                    ->value));              ///< 取负
            next_token = token_lexer.NextToken();
        }
        else
        {
            DataStackArea::AddOneEmittedCode(-1);
            DataStackArea::AddOneEmittedCode(PSH);
            ExpressionParse(TOK_INC, token_lexer, next_token);
            DataStackArea::AddOneEmittedCode(TOK_MUL);
        }
    }
    else if (next_token->type == TOK_INC || next_token->type == TOK_DEC)
    {
        ExpressionIncDec(token_lexer, next_token,
                         expr_currnt_expression_type_);
    }
    else
    {
        Debug_Error("unexpected next token：%c %d \n", 
                                next_token->last_alpha,
                                next_token->type);
        exit(0);
    }

    TopToDownPrecedence(leva_token, token_lexer, 
                        next_token, expr_currnt_expression_type_);

    return;
}




void CParser::ExpressionNum(CTokenLexer &token_lexer,
                            std::unique_ptr<Token> &next_token,
                            int &expr_currnt_expression_type)
{
    DataStackArea::AddOneEmittedCode((long int)IMM);
    DataStackArea::AddOneEmittedCode(
                    static_cast<NumberToken*>(next_token.get())
                    ->value);               ///< tested
    next_token = token_lexer.NextToken();   ///< b = 0;
    expr_currnt_expression_type = TYPE_INT;
    return;
}

void CParser::ExpressionString(CTokenLexer &token_lexer,
                            std::unique_ptr<Token> &next_token,
                            int &expr_currnt_expression_type)
{
    DataStackArea::AddOneEmittedCode((long int)IMM);
    DataStackArea::AddOneEmittedCode((long int)static_cast<StringToken*>
                                    (next_token.get())->data_area_address); ///< 此处放std::string对象的地址
    next_token = token_lexer.NextToken();
    while (next_token->type == TOK_STRING)
    {
        next_token = token_lexer.NextToken();
    }
    DataStackArea::DataAreaFourByteAlignment();
    expr_currnt_expression_type = TYPE_PTR;
    
    return;
}

void CParser::ExpressionSizeof(CTokenLexer &token_lexer,
                            std::unique_ptr<Token> &next_token,
                            int &expr_currnt_expression_type)
{

    next_token = token_lexer.NextToken();
    if (next_token->last_alpha != '(')
    {
        Debug_Error("unexpected next token\n");
        exit(0);
    }
    next_token = token_lexer.NextToken();
    expr_currnt_expression_type = TYPE_INT;
    if (next_token->type == TOK_INT)
    {
        next_token = token_lexer.NextToken();
    }
    else if (next_token->type == TOK_CHAR)
    {
        next_token = token_lexer.NextToken();
        expr_currnt_expression_type = TYPE_CHAR;
    }
    else if(next_token->type == TOK_MUL)
    {
        EatPointor(token_lexer, next_token,
                    expr_currnt_expression_type);
    }

    if (next_token->last_alpha != ')')
    {
        Debug_Error("unexpected next token\n");
        exit(0);
    }
    
    next_token = token_lexer.NextToken();
    DataStackArea::AddOneEmittedCode((long int)IMM);
    long int sizeof_value = 
            (expr_currnt_expression_type ==TYPE_CHAR) 
            ? (sizeof(int)) : (sizeof(int));
    DataStackArea::AddOneEmittedCode(sizeof_value);
    expr_currnt_expression_type = TYPE_INT;

    return;
}

void CParser::ExpressionID(CTokenLexer &token_lexer,
                            std::unique_ptr<Token> &next_token,
                            int &expr_currnt_expression_type)
{
    int hash = static_cast<IdentifierToken*>(next_token.get())->hash;
    IdentifierMapItr this_id = token_lexer.GetIDmapPointer()->FindID(hash);
    if (this_id == token_lexer.GetIDmapPointer()->GetEnd())
    {
        Debug_Error("Not found id: last alpha %c \n", next_token->last_alpha);
        exit(0);
    }
    
    next_token = token_lexer.NextToken();
    if (next_token->last_alpha == '(')   ///< id是函数
    {
        next_token = token_lexer.NextToken();
        int param_num = 0;
        while (next_token->last_alpha != ')')         ///< 形参入栈
        {
            ExpressionParse(TOK_ASSIGN, token_lexer, next_token);
            DataStackArea::AddOneEmittedCode((long int)PSH);
            ++param_num;
            if (next_token->last_alpha == ',')
            {
                next_token = token_lexer.NextToken();
            }
        }
        next_token = token_lexer.NextToken();
        if (this_id->second.var_class == TOK_SYS)
        {
            DataStackArea::AddOneEmittedCode(this_id->second.value);
        }
        else if(this_id->second.var_class == TOK_FUN)
        {
            DataStackArea::AddOneEmittedCode(JSR);
            DataStackArea::AddOneEmittedCode(this_id->second.value);
        }
        else
        {
            Debug_Error("unexpected next token\n");
            exit(0);
        }

        if (param_num)
        {
            DataStackArea::AddOneEmittedCode((long int)ADJ);
            DataStackArea::AddOneEmittedCode(param_num);
        }
        expr_currnt_expression_type = this_id->second.var_type;
    }
    else if (this_id->second.var_class == TOK_NUM)  ///< 枚举
    {
        DataStackArea::AddOneEmittedCode((long int)IMM);
        DataStackArea::AddOneEmittedCode(this_id->second.value);
        expr_currnt_expression_type = TYPE_INT;
    }
    else        ///< 变量
    {
        if (this_id->second.var_class == TOK_LOC)   ///< tested
        {
            DataStackArea::AddOneEmittedCode(LEA);  ///< load effective address
            DataStackArea::AddOneEmittedCode(DataStackArea::GetLocalVariableOffset() 
                                                - this_id->second.value);
            // 取地址,d[Val]是局部变量偏移量  kimi:*++e = loc - d[Val]：计算局部变量的偏
            // 移量，并将该偏移量添加到指令缓冲区中。loc - d[Val] 表示当前局部变量相对于栈
            // 帧基地址的偏移量
        }
        else if (this_id->second.var_class == TOK_GLOB)
        {
            DataStackArea::AddOneEmittedCode(IMM);
            DataStackArea::AddOneEmittedCode(this_id->second.value);
        }
        else
        {
            Debug_Error("unexpected next token\n");
            exit(0);
        }
        expr_currnt_expression_type = this_id->second.var_type;
        long int op = (expr_currnt_expression_type == TYPE_CHAR)
                                        ? (LC) : (LI);
        DataStackArea::AddOneEmittedCode(op);                 ///< tested
    }

    return;
}


void CParser::ExpressionTypeTrans(CTokenLexer &token_lexer,
                            std::unique_ptr<Token> &next_token,
                            int &expr_currnt_expression_type)
{
    next_token = token_lexer.NextToken();
    if ((next_token->type == TOK_INT) 
            || (next_token->type == TOK_CHAR))
    {
        int trans_type = (next_token->type == TOK_INT) 
                            ? (TYPE_INT) : (TYPE_CHAR);
        EatPointor(token_lexer, next_token, trans_type);
        if (next_token->last_alpha != ')')
        {
            Debug_Error("unexpected next token\n");
            exit(0);
        }
        next_token = token_lexer.NextToken();
        ExpressionParse(TOK_INC, token_lexer, next_token);
        expr_currnt_expression_type = trans_type;
    }
    else
    {
        ExpressionParse(TOK_ASSIGN, token_lexer, next_token);
        if (next_token->last_alpha != ')')
        {
            Debug_Error("unexpected next token\n");
            exit(0);
        }
    }
    return;
}

void CParser::ExpressionPointorValue(CTokenLexer &token_lexer,
                            std::unique_ptr<Token> &next_token,
                            int &expr_currnt_expression_type)
{
    next_token = token_lexer.NextToken();
    ExpressionParse(TOK_INC, token_lexer, next_token);
    if (expr_currnt_expression_type > TYPE_INT)
    {
        expr_currnt_expression_type = expr_currnt_expression_type - TYPE_PTR;
    }
    else
    {
        Debug_Error("unexpected next token\n");
        exit(0);
    }
    DataStackArea::AddOneEmittedCode((expr_currnt_expression_type == TYPE_CHAR)
                                    ? (LC) : (LI));
    return;
}

void CParser::ExpressionAddress(CTokenLexer &token_lexer,
                            std::unique_ptr<Token> &next_token,
                            int &expr_currnt_expression_type)
{
    next_token = token_lexer.NextToken();
    ExpressionParse(TOK_INC, token_lexer, next_token);
    int emitted_code = *(DataStackArea::NowPosEmittedCode());
    if ((emitted_code == LC) 
            || (emitted_code == LI))
    {
        DataStackArea::UseEmittedCodeArea(-1);    ///< --e; 上面解析id时，先放地址，后放LI/LC 所以-1就取到地址 待测试
    }
    else
    {
        Debug_Error("unexpected next token\n");
        exit(0);        
    }
    expr_currnt_expression_type = expr_currnt_expression_type + TYPE_PTR;

    return;
}



void CParser::ExpressionIncDec(CTokenLexer &token_lexer,
                            std::unique_ptr<Token> &next_token,
                            int &expr_currnt_expression_type)
{
    int tmp_type = next_token->type;

    next_token = token_lexer.NextToken();
    ExpressionParse(TOK_INC, token_lexer, next_token);
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
    long int tmp = (expr_currnt_expression_type > TYPE_PTR) 
                    ? (sizeof(int)) : (sizeof(char));
    DataStackArea::AddOneEmittedCode(tmp);
    tmp = (tmp_type == TOK_INC) ? (ADD) : (SUB);
    DataStackArea::AddOneEmittedCode(tmp);
    tmp = (expr_currnt_expression_type == TYPE_CHAR)
                        ? (SC) : (SI);

    return;
}

