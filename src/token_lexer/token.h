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
#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <string>
#include <unordered_map>
#include <memory>
#include "token_type.h"

class CSourceCodeBuf;
class CIdentifierMap;

/**
 * @brief 解析token
 * 
 */
class CTokenLexer
{
public:
    CTokenLexer(CSourceCodeBuf *pbuf, 
                CIdentifierMap *_identifier_map);
    ~CTokenLexer();
public:
    std::unique_ptr<Token> NextToken();
    char NowChar();
    CIdentifierMap* GetIDmapPointer() const
    {
        return identifier_map_;
    };

private:
    CSourceCodeBuf *src_buf_ = nullptr;
    /**
    * @brief table of identifier
    * 
    */
    CIdentifierMap *identifier_map_;


private:

    /**
    * @brief 
    * 
    */
    void AddKeywordsToSymTable();
    
    /**
    * @brief 
    * 
    */
    void AddSystemOperetor();

    /**
    * @brief 判断该字符是否为字母
    * 
    * @return true  是字母就进入标志符结构
    * @return false 
    */
    bool isAlpha(char token);

    /**
    * @brief 解析标志符
    * 
    * @return Token 
    */
    std::unique_ptr<Token> ReturnIdentifierToken(char token);

    /**
    * @brief 数字
    * 
    * @param token 
    * @return true 
    * @return false 
    */
    bool isNumber(char token);

    /**
    * @brief 
    * 
    * @return Token 
    */
    std::unique_ptr<Token> ReturnNumberToken(char token);

    /**
    * @brief Get the This Number Token Value object
    *        获取解析到的数值token的具体数值
    * @return int 
    */
    int GetThisNumberTokenValue();

    /**
    * @brief 
    * 
    */
    void GotToNextLine();

    bool isStringToken(char token);

    /**
    * @brief 
    * 
    * @return Token 
    */
    std::unique_ptr<Token> ReturnStringToken(char token);

    /**
    * @brief operetor token
    * 
    * @param _type 
    * @return std::unique_ptr<Token> 
    */
    std::unique_ptr<Token> ReturnOperetorToken(eToken _type, char token);

};




#endif
