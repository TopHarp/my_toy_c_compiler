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
#include "source_code_buf.h"
#include "debug_print.h"
#include "file_opt.h"
#include "error_code.h"


CSourceCodeBuf::CSourceCodeBuf()
        : p_file_opt_(std::make_unique<CFileOpt>())
          , p_code_buf_(nullptr)
{}


CSourceCodeBuf::~CSourceCodeBuf()
{}

int CSourceCodeBuf::ReadSourceFile(const char *pathname)
{
    if (nullptr == pathname)
    {
        return RETURN_FAIL;
    }

    int rtn = p_file_opt_->OpenFileOrCreate(pathname, "r");
    if (rtn != RETURN_SUCCESS)
    {
        Debug_Error("OPEN FILE FAIL. return: %d\n", rtn);
        return RETURN_FAIL;
    }

    file_size_ = p_file_opt_->GetFileSize();
    if ((file_size_ == RETURN_FAIL) 
        || (file_size_ >= 0xffffffff))
    {
        Debug_Error("GET FILE SIZE FAIL. return: %zu\n"
                                            , file_size_);
        return RETURN_FAIL;
    }

    // std::unique_ptr<char[]> p_code_buf_(new char[file_size_ + 1]);
    p_code_buf_ = std::make_unique<char[]>(file_size_ + 1);
    if (nullptr == p_code_buf_)
    {
        Debug_Error("MALLOC FILE SIZE FAIL. return: %p\n"
                                        , p_code_buf_.get());
        return RETURN_FAIL;
    }

    size_t read_size = p_file_opt_->ReadFile(p_code_buf_.get()
                                    , sizeof(char), file_size_);
    if (read_size != file_size_)
    {
        Debug_Error("READ FILE SIZE FAIL. return: %zu\n"
                                        , read_size);
        p_file_opt_->CloseFile();
        return RETURN_FAIL;
    }
    p_file_opt_->CloseFile();

    return RETURN_SUCCESS;
}

std::unique_ptr<char[]> & CSourceCodeBuf::GetBuffer()
{
    return p_code_buf_;
}

char CSourceCodeBuf::Now()
{
    ///< 经过Next()后，read_counter_已经是指在下一个字符了
    ///< 所以此处Now实际上是看一眼下一个字符，而不影响吃字节的指针位置
    return p_code_buf_[read_counter_];    
}

char CSourceCodeBuf::Next()
{
    if (read_counter_ >= file_size_)
    {
        Debug_Error("read_counter_:%zu >= file_size_:%zu \n"
                    , read_counter_, file_size_);
        // exit(0);
        return RETURN_FAIL;
    }

    if (strlen(p_code_buf_.get()) <= read_counter_)
    {
        Debug_Error("read_counter_:%zu >= file_size_:%zu \n"
                    , read_counter_, file_size_);
        // exit(0);
        return RETURN_FAIL;
    }

    char read_char = p_code_buf_[read_counter_++];

    printf("%c", read_char);

    return read_char;
}


char CSourceCodeBuf::Previous()
{
    if (read_counter_ >= file_size_)
    {
        Debug_Error("read_counter_:%zu >= file_size_:%zu \n"
                    , read_counter_, file_size_);
        return RETURN_FAIL;
    }

    if (strlen(p_code_buf_.get()) <= read_counter_)
    {
        Debug_Error("read_counter_:%zu >= file_size_:%zu \n"
                    , read_counter_, file_size_);        
        return RETURN_FAIL;
    }

    --read_counter_;
    return p_code_buf_[read_counter_];
}

size_t CSourceCodeBuf::GetCounter()
{
    return read_counter_;
}

int CSourceCodeBuf::SetCounter(size_t &counter)
{
    if (counter >= file_size_)
    {
        Debug_Error("read_counter_:%zu >= file_size_:%zu \n"
                    , read_counter_, file_size_);
        return RETURN_FAIL;
    }

    read_counter_ = counter;
    return RETURN_SUCCESS;
}


