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
#ifndef _DEBUG_DISPLAY_CHAR_H_
#define _DEBUG_DISPLAY_CHAR_H_

// /*

// Id = identifier 用户定义的identifier

// //运算符
// Assign  =    Cond    ?      Lor     ||
// Lan     &&   Or      |      Xor     ^
// And     &    Eq      ==     Ne      !=
// Lt      <    Gt      >      Le      <=
// Ge      >=   Shl     <<     Shr     >>
// Add     +    Sub     -      Mul     *
// Div     /    Mod     %      Inc     ++
// Dec     --   Brak    [

// Num  常量数值   Fun 函数    Sys 系统调用
// Glo 全局变量    Loc 局部变量
// */
// typedef enum eToken_t
// {
//     TOK_EOF = 0,
//     TOK_NUM = 1,
//     TOK_FUN, TOK_SYS, TOK_GLOB, TOK_LOC, TOK_ID,

//     TOK_CHAR, TOK_INT, TOK_PTR,
//     TOK_ENUM, TOK_IF, TOK_ELSE, TOK_WHILE,
//     TOK_RETURN, TOK_SIZEOF, 
    
//     TOK_ASSIGN, TOK_COND, TOK_LOR, TOK_LAN, TOK_OR, TOK_XOR, TOK_AND,
//     TOK_EQ, TOK_NE, TOK_LT, TOK_GT, TOK_LE, TOK_GE, TOK_SHL, TOK_SHR,
//     TOK_ADD, TOK_SUB, TOK_MUL,TOK_DIV,
//     TOK_MOD, TOK_INC, TOK_DEC,
//     TOK_BRAK,
//     TOK_STRING,
//     TOK_EMPTY
// }eToken;

static const char* debug_token_enum[]
= {
    "TOK_EOF",
    "TOK_NUM",
    "TOK_FUN", "TOK_SYS", "TOK_GLOB", "TOK_LOC", "TOK_ID",

    "TOK_CHAR", "TOK_INT",
    "TOK_ENUM", "TOK_IF", "TOK_ELSE", "TOK_WHILE",
    "TOK_RETURN", "TOK_SIZEOF", 
    
    "TOK_ASSIGN", "TOK_COND", "TOK_LOR", "TOK_LAN", "TOK_OR", "TOK_XOR", "TOK_AND",
    "TOK_EQ", "TOK_NE", "TOK_LT", "TOK_GT", "TOK_LE", "TOK_GE", "TOK_SHL", "TOK_SHR",
    "TOK_ADD", "TOK_SUB", "TOK_MUL", "TOK_DIV",
    "TOK_MOD", "TOK_INC", "TOK_DEC",
    "TOK_BRAK",
    "TOK_STRING",
    "TOK_EMPTY", "TOK_NNKNOW", "TOK_IGNO"
};

static const char* debug_emitted_code[]
= { "LEA" ,"IMM" ,"JMP" ,"JSR" ,"BZ"  ,"BNZ" ,"ENT" ,"ADJ" ,"LEV", 
    "LI"  ,"LC"  ,"SI"  ,"SC"  ,"PSH" ,
    "OR"  ,"XOR" ,"AND" ,"EQ"  ,"NE"  ,"LT"  ,"GT",
    "LE"  ,"GE"  ,"SHL" ,"SHR" ,"ADD" ,"SUB" ,
    "MUL" ,"DIV" ,"MOD"
};

static const char *system_operetor_char[] = 
{"open", "read", "close", "printf", "malloc",
 "memset", "memcmp", "exit", "void", "main"};


#endif