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
#ifndef _SOURCE_CODE_BUF_H
#define _SOURCE_CODE_BUF_H

#include <memory>

class CFileOpt;

/**
 * @brief 从文件读取源码到内存
 * 
 */
class CSourceCodeBuf
{
public:
    CSourceCodeBuf();
    ~CSourceCodeBuf();

public:
    int ReadSourceFile(const char *pathname);
    std::unique_ptr<char[]>& GetBuffer();
    size_t GetCounter();
    int SetCounter(size_t &counter);
    
    /**
     * @brief 下一个字符
     * 
     * @return char 
     */
    char Next();

    /**
     * @brief 当前字符
     * 
     * @return char 
     */
    char Now();
    
    /**
     * @brief 上一个字符
     * 
     * @return char 
     */
    char Previous();

private:
    std::unique_ptr<CFileOpt> p_file_opt_;
    std::unique_ptr<char[]> p_code_buf_;
    size_t file_size_ = 0;
    size_t read_counter_ = 0;



};

#endif