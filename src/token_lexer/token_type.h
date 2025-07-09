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
#ifndef _TOKEN_TYPE_H_
#define _TOKEN_TYPE_H_

#include <string>

/*
//运算符
Assign  =    Cond    ?      Lor     ||
Lan     &&   Or      |      Xor     ^
And     &    Eq      ==     Ne      !=
Lt      <    Gt      >      Le      <=
Ge      >=   Shl     <<     Shr     >>
Add     +    Sub     -      Mul     *
Div     /    Mod     %      Inc     ++
Dec     --   Brak    [

Num  常量数值   Fun 函数    Sys 系统调用
Glo 全局变量    Loc 局部变量
*/
typedef enum eToken_t
{
    TOK_EOF = 128,
    TOK_NUM = 129,
    TOK_FUN = 130, TOK_SYS = 131, TOK_GLOB = 132, TOK_LOC = 133, TOK_ID = 134,

    TOK_CHAR = 135, TOK_INT = 136, 
    TOK_ENUM = 137, TOK_IF = 138, TOK_ELSE = 139, TOK_WHILE = 140, 
    TOK_RETURN = 141, TOK_SIZEOF = 141, 

    TOK_STRING = 142,
    TOK_EMPTY = 143, TOK_NNKNOW = 144, TOK_IGNO = 145,
    
    TOK_ASSIGN = 146, TOK_COND = 147, TOK_LOR = 148, TOK_LAN = 149, TOK_OR = 150, TOK_XOR = 151, TOK_AND = 152,
    TOK_EQ = 153, TOK_NE = 154, TOK_LT = 155, TOK_GT = 156, TOK_LE = 157, TOK_GE = 158, TOK_SHL = 159, TOK_SHR = 160,
    TOK_ADD = 161, TOK_SUB = 162, TOK_MUL = 163, TOK_DIV = 164,
    TOK_MOD = 165, TOK_INC = 166, TOK_DEC = 167,
    TOK_BRAK = 168
    
    
}eToken;

const static char *keywords[] = {"char", "int",
                     "enum", "if", "else", 
                     "while", "return", "sizeof"};

typedef enum eVarType_t
{TYPE_CHAR, TYPE_INT, TYPE_PTR}eVarType;


/**
 * @brief token base class
 * 
 */
class Token
{
public:
    Token() = default;
    virtual ~Token(){};
public:
    int type = (int)TOK_EOF;            ///< token类型
    char last_alpha = 0x0;
};

class NumberToken : public Token
{
public:
    NumberToken(){};
    // NumberToken() : value(0){};
    NumberToken(int val) : value(val){};
    ~NumberToken(){};

public:
    long int value = 0;
};

class StringToken : public Token
{
public:
    StringToken() = default;
    ~StringToken(){};
public:
    std::string string_data;
    long int data_area_address = 0;             ///< 放data区地址
};


class IdentifierToken : public Token
{
public:
    IdentifierToken() = default;
    ~IdentifierToken(){};
public:
    int hash = 0;
    std::string name;
    int var_class = (int)TOK_EOF;  ///< Class 为类型,如Num(常量数值),Fun(函数),Sys(系统调用),Glo全局变量,Loc 局部变量
    int var_type = (int)TOK_EOF;   ///< Type 数据类型 返回类型 赋值类型 type of the variable - int, char, ptr
    long int value = 0;            ///< Val 这个符号存储的数值 函数则为函数地址指针
    int hclass = (int)TOK_EOF;  ///< HClass "backup slot" of 'class' for global variable hidden by a local
    int htype = (int)TOK_EOF;   ///< HType "backup slot" of 'type' for global variable hidden by a local
    int hval = 0;               ///< HVal "backup slot" of 'val' for global variable hidden by a local
};

// using pToken = std::unique_ptr<Token>;






#endif