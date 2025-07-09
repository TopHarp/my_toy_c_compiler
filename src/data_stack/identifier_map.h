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
#ifndef _IDENTIFIER_MAP_H_
#define _IDENTIFIER_MAP_H_

#include <unordered_map>
#include "token_type.h"

/**
 * @brief 标识符表
 *        标识符数据结构IdentifierToken
 */
class CIdentifierMap
{
public:
    CIdentifierMap(){};
    ~CIdentifierMap(){};

private:
    std::unordered_map<int, IdentifierToken> identifier_map_;

public:

    std::unordered_map<int, IdentifierToken>::iterator 
                FindID(int hash);

    std::unordered_map<int, IdentifierToken>::iterator 
                FindID(std::string id_token_name);
    
    void AddID(int &hash, IdentifierToken &id);

    std::unordered_map<int, IdentifierToken>::iterator 
                GetEnd();

    void UnwindIdentifierTable(const int &token_type);

};

using IdentifierMap = std::unordered_map<int, IdentifierToken>;
using IdentifierMapItr = std::unordered_map<int, IdentifierToken>::iterator;



#endif