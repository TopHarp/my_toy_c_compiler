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



int CParser::ParseCode(CTokenLexer &token_lexer)
{
    std::unique_ptr<Token> next_token 
                    = token_lexer.NextToken();  ///< eat one

    // int main_currnt_expression_type;
    while (nullptr != next_token)                          // 退出条件为文件读完
    {
        printf(" 00 %d %c \n" ,next_token->type, 
                               next_token->last_alpha);
        ///< 函数局部变量只能定义在函数体开始
        eVarType base_type = TYPE_INT;
        ///< declaration                                   ///< read left of line
        if (next_token->type == TOK_INT)                   ///< int *xxxxx
        {
            base_type = TYPE_INT;
        }
        else if (next_token->type == TOK_CHAR)             ///< char *xxxxx
        {
            base_type = TYPE_CHAR;
        }
        else if (next_token->type == TOK_ENUM)
        {
            next_token = ParseEnumDeclaration(token_lexer);
        }
        else
        {}

        next_token = token_lexer.NextToken();               ///< eat enum }

        while((next_token->last_alpha != ';')
                && (next_token->last_alpha != '}'))         ///< read line to end
        {
            printf("11\n");
            int main_currnt_expression_type = (int)base_type;    ///< ty = bt

            if (next_token->type == TOK_MUL)                ///< is pointor
            {
                EatPointor(token_lexer, next_token, 
                           main_currnt_expression_type);
            }

            if (RETURN_SUCCESS != FunctionOrIdentifier(next_token, 
                                                 main_currnt_expression_type,
                                                 token_lexer))
            {
                return RETURN_FAIL;
            }


            printf(" ee %c" ,next_token->last_alpha);
            printf("\n %d %d %d \n", next_token->last_alpha != ';',
                                     next_token->last_alpha != '}', 
                                (next_token->last_alpha != ';')
                                && (next_token->last_alpha != '}'));
             
        }
        printf("22\n");
        next_token = token_lexer.NextToken();           ///< eat next
    }

    ///< TODO::vm run
    DataStackArea::PrintAllEmittedCode();
    
    return RETURN_SUCCESS;
}



int CParser::FunctionOrIdentifier(std::unique_ptr<Token> &next_token, 
                            int &main_currnt_expression_type, 
                            CTokenLexer &token_lexer)
{

    if (next_token->type != TOK_ID)                 ///< int a;
    {
        Debug_Error("parse error. type:%d, lastalpha:%c\n", 
                                next_token->type,
                                next_token->last_alpha);
        return RETURN_FAIL;
    }

    IdentifierMapItr identifier_itr 
            = token_lexer.GetIDmapPointer()
                            ->FindID(static_cast<IdentifierToken*>
                            (next_token.get())->name);
    if (identifier_itr == token_lexer.GetIDmapPointer()->GetEnd())
    {
        Debug_Error("not found identifier.\n");
        return RETURN_FAIL;
    }
    
    if (identifier_itr->second.var_class == TOK_GLOB)   ///< 已经定义过了的全局变量 重复全局变量定义,解释见后
    {
        Debug_Error("duplicate global dfinition.\n");
        return RETURN_FAIL;
    }

    identifier_itr->second.var_type = main_currnt_expression_type;   ///< now we know value type

    next_token = token_lexer.NextToken();           ///< eat xxx(   ????

    if (next_token->last_alpha == '(')              ///< is (   function
    {
        int param_index = 0;
        identifier_itr->second.var_class = TOK_FUN;     ///< the id is function
        identifier_itr->second.value = 
                        (long int)(DataStackArea::WatchEmittedCodePos(1));///< id[Val] = (int)(e + 1); //函数指针? 在字节码中的偏移量/地址
        next_token = token_lexer.NextToken();

        EatFunction(main_currnt_expression_type, param_index,
                        token_lexer, next_token);

        
    }
    else
    {
        identifier_itr->second.var_class = (int)TOK_GLOB; ///< 全局变量
        identifier_itr->second.value = 
                    (long int)(DataStackArea::GetNowDataAreaNowPointorPos());///< 数据段地址给到val
        DataStackArea::UseDataArea(sizeof(int));        ///< TODO:: 数据段指针递增
    }

    if (next_token->last_alpha == ',')
    {
        next_token = token_lexer.NextToken();
    }

    return RETURN_SUCCESS;
}


int CParser::EatFunction(int &main_currnt_expression_type, 
                              int &param_index,
                              CTokenLexer &token_lexer,
                              std::unique_ptr<Token> &next_token)
{

    while (next_token->last_alpha != ')')         ///< eat function parameter
    {
        main_currnt_expression_type = TYPE_INT;
        EatFunctionParam(param_index, token_lexer, next_token);
        next_token = token_lexer.NextToken();
        if (next_token->last_alpha == ',')
        {
            next_token = token_lexer.NextToken();
        }
    }

    next_token = token_lexer.NextToken();
    if (next_token->last_alpha != '{')          ///< eat func body
    {
        Debug_Error(" bad function definition, need {}.type:%d alpha:%c \n",
                            next_token->type,
                            next_token->last_alpha);
        return RETURN_FAIL;
    }

    DataStackArea::SetLocalVariableOffset(++param_index);           ///< Loc = ++i

    next_token = token_lexer.NextToken();

    while ((next_token->type == TOK_INT) ||
                (next_token->type == TOK_CHAR))
    {
        EatFuntionLocParam(param_index, token_lexer,
                                        next_token);
        next_token = token_lexer.NextToken();
    }

    DataStackArea::AddOneEmittedCode((int)ENT);         ///< *++e = ENT
    DataStackArea::AddOneEmittedCode(param_index - DataStackArea::GetLocalVariableOffset());  ///< *++e = i - loc

    while (next_token->last_alpha != '}')
    {
        ///< stmt()  语法分析
        StatementParse(token_lexer, next_token);
    }

    DataStackArea::AddOneEmittedCode((int)LEV); ///< *++e = LEV 函数返回
    ///< 恢复符号信息
    token_lexer.GetIDmapPointer()->UnwindIdentifierTable(TOK_LOC);

    return RETURN_SUCCESS;
}

int CParser::EatFuntionLocParam(int &param_index,
                                CTokenLexer &token_lexer,
                                std::unique_ptr<Token> &next_token)
{
    int base_type = 
            (next_token->type == TOK_INT) ? 
                    (TYPE_INT) : (TYPE_CHAR);
    next_token = token_lexer.NextToken();
    while (next_token->last_alpha != ';')
    {
        int main_currnt_expression_type = base_type;
        if (next_token->last_alpha == TOK_MUL)
        {
            EatPointor(token_lexer, next_token,
                        main_currnt_expression_type);
        }
        
        if (next_token->type != TOK_ID)                 ///< int a;
        {
            Debug_Error("parse error. type:%d, lastalpha:%c\n", 
                                    next_token->type,
                                    next_token->last_alpha);
            return RETURN_FAIL;
        }

        IdentifierMapItr identifier_itr 
                = token_lexer.GetIDmapPointer()
                                ->FindID(static_cast<IdentifierToken*>
                                (next_token.get())->name);
        if (identifier_itr == token_lexer.GetIDmapPointer()->GetEnd())
        {
            Debug_Error("not found identifier.\n");
            return RETURN_FAIL;
        }

        if (identifier_itr->second.var_class == TOK_LOC)
        {
            Debug_Error("duplicate local definition.\n");
            return RETURN_FAIL;   
        }
        
        identifier_itr->second.hclass = identifier_itr->second.var_class;
        identifier_itr->second.var_class = TOK_LOC;
        identifier_itr->second.htype = identifier_itr->second.var_type;
        identifier_itr->second.var_type = main_currnt_expression_type;
        identifier_itr->second.hval = identifier_itr->second.value;
        identifier_itr->second.value = ++param_index;      ///< 存储变量偏移量 先自加
        next_token = token_lexer.NextToken();
        if (next_token->last_alpha == ',')
        {
            next_token = token_lexer.NextToken();
        }
    }
    
    return RETURN_SUCCESS;
}

int CParser::EatFunctionParam(int &param_index,
                              CTokenLexer &token_lexer,
                              std::unique_ptr<Token> &next_token)
{
    int main_currnt_expression_type = TYPE_INT;
    if (next_token->type == TOK_INT)
    {
        next_token = token_lexer.NextToken();
    }
    else if (next_token->type == TOK_CHAR)
    {
        next_token = token_lexer.NextToken();
        main_currnt_expression_type = TYPE_CHAR;
    }
    
    if (next_token->type == TOK_MUL)
    {
        EatPointor(token_lexer, next_token, 
                   main_currnt_expression_type);
    }

    if (next_token->type != TOK_ID)
    {
        Debug_Error("bad parammter declaration.\n");
        return RETURN_FAIL;
    }


    int hash = static_cast<IdentifierToken*>
                            (next_token.get())->hash;
    IdentifierMapItr identifier_itr 
                = token_lexer.GetIDmapPointer()
                                ->FindID(hash);
    
    if (identifier_itr->second.var_class == TOK_LOC)
    {
        Debug_Error("duplicate parameter definition.\n");
        return RETURN_FAIL;
    }
    
    identifier_itr->second.hclass = identifier_itr->second.var_class;
    identifier_itr->second.var_class = TOK_LOC;
    identifier_itr->second.htype = identifier_itr->second.var_type;
    identifier_itr->second.var_type = main_currnt_expression_type;
    identifier_itr->second.hval = identifier_itr->second.value;
    identifier_itr->second.value = param_index++;

    return RETURN_SUCCESS;
}

void CParser::EatLeftOfLine(std::unique_ptr<Token> &next_token, eVarType &base_type, CTokenLexer &token_lexer)
{
    if (next_token->type == TOK_INT)                   ///< int *xxxxx
    {
        base_type = TYPE_INT;
    }
    else if (next_token->type == TOK_CHAR)             ///< char *xxxxx
    {
        base_type = TYPE_CHAR;
    }
    else if (next_token->type == TOK_ENUM)
    {
        next_token = ParseEnumDeclaration(token_lexer);
    }
    else
    {}
}


void CParser::EatPointor(CTokenLexer &token_lexer,
                         std::unique_ptr<Token> &next_token, 
                         int &main_currnt_expression_type)
{
    while (next_token->type == TOK_MUL)             ///< int **xxx eat pointer 
    {
        main_currnt_expression_type += (int)TYPE_PTR;    ///< represented by int+prt 
        next_token = token_lexer.NextToken();
    }

    return;
}


std::unique_ptr<Token> CParser::ParseEnumDeclaration(CTokenLexer &token_lexer)
{
    std::unique_ptr<Token> next_token 
                        = token_lexer.NextToken();
    
    printf(" ll %d %d" ,next_token->type, 
            (static_cast<NumberToken*>(next_token.get()))->type);
    Debug_Token(next_token->type);

    if ((next_token->last_alpha != '{') 
        || (next_token->type == TOK_ID))
    {
        next_token = token_lexer.NextToken();       ///< TODO::是否需要循环找
    }
    if (next_token->last_alpha == '{')
    {
        next_token = token_lexer.NextToken();
        int i = 0;
        while(next_token->last_alpha != '}')
        {
            if (next_token->type != TOK_ID)
            {
                Debug_Error("parse enum error, not found identifier. token type:%d %c\n"
                                , next_token->type
                                , token_lexer.NowChar());
                return nullptr;
            }
            int id_hash = 0;
            if (next_token->type == TOK_ID)
            {
                id_hash = (static_cast<IdentifierToken *>(next_token.get()))->hash;
            }

            CIdentifierMap *identifier_map_ = token_lexer.GetIDmapPointer();
            std::unordered_map<int, IdentifierToken>::iterator it 
                                    = identifier_map_->FindID(id_hash);
            if (it == identifier_map_->GetEnd())
            {
                Debug_Error("not found enum id in map\n");
            }
            next_token = token_lexer.NextToken();
            if (next_token->type == TOK_ASSIGN)     ///< 赋值 enum {Num = 128,}
            {
                next_token = token_lexer.NextToken();
                if (next_token->type != TOK_NUM)
                {
                    Debug_Error("parse enum error, not found \
                                        assign number. token type:%d\n"
                                    , next_token->type);
                    return nullptr;
                }
                i = (static_cast<NumberToken*>(next_token.get()))->value;
                next_token = token_lexer.NextToken();
            }
            it->second.var_class = (int)TOK_NUM;
            it->second.var_type = (int)TYPE_INT;
            it->second.value = i;
            i++;
        }
    }

    return std::move(next_token);
}

