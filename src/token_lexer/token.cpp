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
#include <cstring>
#include "token.h"
#include "system_operetor.h"
#include "source_code_buf.h"
#include "hash.h"
#include "identifier_map.h"
#include "data_stack_area.h"
#include "debug_display_char.h"

CTokenLexer::CTokenLexer(CSourceCodeBuf *pbuf, 
                CIdentifierMap *_identifier_map):
        src_buf_(pbuf),
        identifier_map_(_identifier_map)
{
    Debug_Error("AddKeywordsToSymTable \n");
    AddKeywordsToSymTable();
    Debug_Error("AddSystemOperetor \n");
    AddSystemOperetor();
}

CTokenLexer::~CTokenLexer()
{}

void CTokenLexer::AddKeywordsToSymTable()
{
    char next = 0;
    int num_tk = (int)TOK_CHAR;
    int index = 0;
    while(num_tk <= (int)TOK_SIZEOF)
    {
        int hash = 0;
        int len = (int)strlen(keywords[index]);       ///< 
        if (len <=0)
        {
            continue;
        }

        hash = Common::TokenHash(keywords[index], len);
        if (RETURN_FAIL == hash)
        {
            Debug_Error("hash error.\n");
        }

        // printf("%s %%hash %d len %d num_tk %d ", keywords[index], 
        //                                         hash, len, num_tk);

        IdentifierToken identifier;
        identifier.type = num_tk;
        identifier.hash = hash;
        identifier.name = keywords[index];
        identifier_map_->AddID(hash, identifier);

        num_tk++;
        index++;
    }

    return;
}

void CTokenLexer::AddSystemOperetor()
{
    char next = 0;
    int num_tk = (int)OPEN;
    int index = 0;
    while(num_tk <= (int)EXIT)
    {
        int hash = 0;
        int len = (int)strlen(system_operetor_char[index]);       ///< 
        if (len <=0)
        {
            continue;
        }

        hash = Common::TokenHash(system_operetor_char[index], len);
        if (RETURN_FAIL == hash)
        {
            Debug_Error("hash error.\n");
        }

        // printf("num_tk %d %s %%hash %d len %d"
                    // ,num_tk, system_operetor_char[index], hash, len);

        IdentifierToken identifier;
        identifier.type = TOK_ID;
        identifier.hash = hash;
        identifier.name = system_operetor_char[index];
        identifier.var_class =(int)TOK_SYS;
        identifier.var_type = (int)TYPE_INT;
        identifier.value = num_tk;

        identifier_map_->AddID(hash, identifier);

        num_tk++;
        index++;
    }

    ///< add void 
    int len = (int)strlen(system_operetor_char[index]);
    int hash = Common::TokenHash(system_operetor_char[index], len);
    if (RETURN_FAIL == hash)
    {
        Debug_Error("hash error.\n");
    }

    IdentifierToken identifier;
    identifier.type = (int)TOK_CHAR;
    identifier.hash = hash;
    identifier.name = system_operetor_char[index];
    identifier.var_class =(int)TOK_SYS;
    identifier.var_type = (int)TYPE_INT;
    identifier.value = num_tk;

    identifier_map_->AddID(hash, identifier);

    num_tk++;
    index++;

    ///< add main
    len = (int)strlen(system_operetor_char[index]);
    hash = Common::TokenHash(system_operetor_char[index], len);
    if (RETURN_FAIL == hash)
    {
        Debug_Error("hash error.\n");
    }

    IdentifierToken identifier_tmp;
    identifier_tmp.type = (int)TOK_ID;
    identifier_tmp.hash = hash;
    identifier_tmp.name = system_operetor_char[index];
    identifier_tmp.var_class =(int)TOK_SYS;
    identifier_tmp.var_type = (int)TYPE_INT;
    identifier_tmp.value = num_tk;

    identifier_map_->AddID(hash, identifier_tmp);

    return;
}

char CTokenLexer::NowChar()
{
    if (nullptr == src_buf_)
    {
        Debug_Error("src buf is null \n");
    }
    return src_buf_->Now();
}

std::unique_ptr<Token> CTokenLexer::NextToken()
{
    char token = 0;
    while(token = src_buf_->Next())
    {
        if (token == RETURN_FAIL)           ///< end of file
        {
            Debug_Error("Next() is failed. will return nullptr.\n");
            return nullptr;
        }
        else if (token == '\n')
        {
            continue;
        }
        // else if (token == ' ')
        // {
        //     continue;
        // }
        else if (token == '#')                          ///< 忽略#的一行 注释或#include
        {
            GotToNextLine();
        }
        else if (isAlpha(token))
        {
            return (ReturnIdentifierToken(token));
        }
        else if (isNumber(token))
        {
            return (ReturnNumberToken(token));              ///< TODO:: 返回数值
        }
        else if (token == '/')
        {
            token = src_buf_->Next();
            if (token = '/')                        ///< //单行注释
            {
                GotToNextLine();
            }
            else
            {
                return (ReturnOperetorToken(TOK_DIV, token));
            }
        }
        else if (isStringToken(token))                          ///< 字符串
        {
            return (ReturnStringToken(token));
        }
        else if (token == '=') 
        { 
            if (src_buf_->Now() == '=') 
            { 
                token = src_buf_->Next();
                return (ReturnOperetorToken(TOK_EQ, token)); 
            } 
            else return (ReturnOperetorToken(TOK_ASSIGN, token)); 
        }//等于,赋值
        else if (token == '+') 
        { 
            if (src_buf_->Now() == '+') 
            {
                token = src_buf_->Next();
                return (ReturnOperetorToken(TOK_INC, token)); 
            } 
            else return (ReturnOperetorToken(TOK_ADD, token)); 
        }//加,自增
        else if (token == '-') 
        { 
            if (src_buf_->Now() == '-') 
            { 
                token = src_buf_->Next();
                return (ReturnOperetorToken(TOK_DEC, token)); 
            }
            else return (ReturnOperetorToken(TOK_SUB, token)); 
        }//减,自减
        else if (token == '!') 
        { 
            if (src_buf_->Now() == '=') 
            { 
                token = src_buf_->Next();
                return (ReturnOperetorToken(TOK_NE, token)); 
            } /*TODO::这里没有取非*/
        }//不等于
        else if (token == '<') 
        { 
            if (src_buf_->Now() == '=') 
            {
                token = src_buf_->Next();
                return (ReturnOperetorToken(TOK_LE, token));
            } 
            else if (src_buf_->Now() == '<') 
            { 
                token = src_buf_->Next();
                return (ReturnOperetorToken(TOK_SHL, token)); 
            }
             else return (ReturnOperetorToken(TOK_LT, token)); 
        }// <,<=, <<
        else if (token == '>') 
        { 
            if (src_buf_->Now() == '=') 
            { 
                token = src_buf_->Next();
                return (ReturnOperetorToken(TOK_GE, token)); 
            } 
            else if (src_buf_->Now() == '>') 
            { 
                token = src_buf_->Next();
                return (ReturnOperetorToken(TOK_SHR, token)); 
            } 
            else return (ReturnOperetorToken(TOK_GT, token)); 
        }//>,>= >>
        else if (token == '|') 
        { 
            if (src_buf_->Now() == '|') 
            {
                token = src_buf_->Next();
                return (ReturnOperetorToken(TOK_LOR, token)); 
            } 
            else return (ReturnOperetorToken(TOK_OR, token));
        }//逻辑或,或
        else if (token == '&') 
        { 
            if (src_buf_->Now() == '&') 
            { 
                token = src_buf_->Next();
                return (ReturnOperetorToken(TOK_LAN, token)); 
            }
            else 
            { return (ReturnOperetorToken(TOK_AND, token)); }
        }
        else if (token == '^') { return (ReturnOperetorToken(TOK_XOR, token)); }
        else if (token == '%') { return (ReturnOperetorToken(TOK_MOD, token)); }
        else if (token == '*') { return (ReturnOperetorToken(TOK_MUL, token)); }
        else if (token == '[') { return (ReturnOperetorToken(TOK_BRAK, token)); }
        else if (token == '?') { return (ReturnOperetorToken(TOK_COND, token)); }
        else if (token == '~' 
                || token == ';' 
                || token == '{' || token == '}' 
                || token == '(' || token == ')' 
                || token == ']' || token == ',' 
                || token == ':') 
        {
            return (ReturnOperetorToken(TOK_IGNO, token));//不做处理
        }
        // else
        // {
        //     return (ReturnOperetorToken(TOK_NNKNOW, token));
        // }
    }

    // return (ReturnOperetorToken(TOK_EMPTY, token));
}

bool CTokenLexer::isAlpha(char token)
{
    if ((token >= 'a' && token <='z')
                    || (token >= 'A' && token <='Z')
                    || (token == '_'))
    {
        return true;
    }
    return false;
}

std::unique_ptr<Token> CTokenLexer::ReturnIdentifierToken(char token)
{
    char next_alpha = token;
    std::string id_name;
    id_name.push_back(next_alpha);                              ///< 首字母开头
    next_alpha = src_buf_->Next();
    while (isAlpha(next_alpha) || isNumber(next_alpha))         ///< 首字母开头 可以数字结尾
    {
        id_name.push_back(next_alpha);
        next_alpha = src_buf_->Next();
    }
    // if (src_buf_->Now() != ' ')
    {
        src_buf_->Previous();                                ///< 发现最后一个不是字母 后退一个
    }

    int hash = Common::TokenHash(id_name.c_str(), id_name.length());    ///< hash
    if (RETURN_FAIL == hash)
    {
        Debug_Error("hash error.\n");
        return nullptr;
    }

    // printf("%s %%hash %d len %d cont %d ", id_name.c_str(), hash, 
    //                 src_buf_->GetCounter());

    std::unique_ptr<IdentifierToken> p_token = std::make_unique<IdentifierToken>();
    auto it = identifier_map_->FindID(hash);         ///< 查找hash是否已经在符号表中
    if (it != identifier_map_->GetEnd())
    {
        p_token->last_alpha = id_name.back();
        p_token->hash = it->second.hash;
        p_token->name = it->second.name;
        p_token->type = it->second.type;
        p_token->var_class = it->second.var_class;
        p_token->var_type = it->second.var_type;
        p_token->value = it->second.value;
        p_token->hclass = it->second.hclass;
        p_token->htype = it->second.htype;
        p_token->hval = it->second.hval;

        // Debug_Token(it->second.type);
        // printf("find type:%d hash:%d name %s ", p_token->type
        //                     , hash, it->second.name.c_str());
        // Debug_Token(p_token->type);

    }
    else
    {
        IdentifierToken identifier;
        identifier.hash = hash;
        identifier.name = id_name;
        identifier.type = TOK_ID;
        identifier.last_alpha = id_name.back();
        identifier_map_->AddID(hash, identifier);

        p_token->type = TOK_ID;
        p_token->hash = hash;
        p_token->name = id_name;
        p_token->last_alpha = id_name.back();
    }

    return std::move(p_token);
}

bool CTokenLexer::isNumber(char token)
{
    if ((token >= '0') 
            && (token <= '9'))
    {
        return true;
    }

    return false;
}

std::unique_ptr<Token> CTokenLexer::ReturnNumberToken(char token)
{
    int value = 0;
    value = token - '0';  ///< 第一位数字
    if (value != 0)      ///< 第一位非0数字 且
    {
        while ((src_buf_->Now() >= '0') && (src_buf_->Now() <= '9')) ///< 第二位数字
        {
            token = src_buf_->Next();
            value = value * 10 + (token - '0');
        }
    }
    else if ((src_buf_->Now() == 'x') || (src_buf_->Now() == 'X'))  ///< tested
    {
        token = src_buf_->Next();     ///< 吃掉x
        while (((src_buf_->Now() >= '0' && src_buf_->Now() <= '9')
                || (src_buf_->Now() >= 'a' && src_buf_->Now() <= 'f')
                || (src_buf_->Now() >= 'A' && src_buf_->Now() <= 'F')))
        {
            // value = value * 16 + (token & 15) + (token >= 'A' ? 9 : 0);
            token = src_buf_->Next();
            value = value * 16 + (token & 0x0f) + (token >= 'A' ? 9 : 0);
        }
    }
    else
    {
        while((src_buf_->Now() >= '0') && (src_buf_->Now() <= '7'))         ///< tested
        {
            token = src_buf_->Next();
            value = value * 8 + (token - '0');
        }
    }

    std::unique_ptr<NumberToken> p_token = std::make_unique<NumberToken>();
    p_token->type = TOK_NUM;
    p_token->value = value;
    p_token->last_alpha = value;  ///< TODO:: 数字此处返回数字好了

    return std::move(p_token);
}

void CTokenLexer::GotToNextLine()
{
    char token = src_buf_->Next();
    while(token != 0 && token != '\n')
    {
        token = src_buf_->Next();
    }
    return;
}

bool CTokenLexer::isStringToken(char token)
{
    if (token == '\'' || token == '"')
    {
        return true;
    }

    return false;
}

std::unique_ptr<Token> CTokenLexer::ReturnStringToken(char token)
{
    std::string string_data;
    char next_tk = src_buf_->Next();
    if (next_tk == '"' || next_tk == '\'')              ///< 空字符串“” 
    {
        return (ReturnOperetorToken(TOK_STRING, next_tk));
    }
    while((next_tk != 0)
            && (next_tk != '\'' && next_tk != '"'))
    {
        if (next_tk == '\\')                    ///< 转义符  目的是反斜杠
        {
            next_tk = src_buf_->Next();
            if (next_tk == 'n')
            {
                next_tk = '\n';
            }
        }
        if (token == '"')                           ///< 单引号情况如何TODO::
        {
            string_data.push_back(next_tk);
        }
        next_tk = src_buf_->Next();                 ///< 吃掉最后的双引号
    }

    // next_tk = src_buf_->Next();
    if (next_tk == '"')                     ///< 吃掉最后的双引号
    {
        ///< if (tk == '"') ival = (int)pp; else tk = Num;//双引号则ival指向data中字符串开始,单引号则认为是数字   
    }


    std::unique_ptr<StringToken> p_token = std::make_unique<StringToken>();
    p_token->type = TOK_STRING;
    p_token->string_data = string_data;
    p_token->last_alpha = next_tk;
    p_token->data_area_address = (long int)DataStackArea::GetNowDataAreaNowPointorPos();    ///< 先放首地址
    DataStackArea::SetStringToDataArea(string_data.c_str(), string_data.length());///< 再装数据到data区
    
    return std::move(p_token);
}

std::unique_ptr<Token> CTokenLexer::ReturnOperetorToken(eToken _type, char token)
{
    std::unique_ptr<Token> p_token = std::make_unique<Token>();
    p_token->type = _type;
    p_token->last_alpha = token;
    return std::move(p_token);
}








