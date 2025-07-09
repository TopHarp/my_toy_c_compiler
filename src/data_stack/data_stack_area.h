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
#ifndef _DATA_STACK_AREA_
#define _DATA_STACK_AREA_
#include <cstdio>
#include "debug_print.h"

/**
 * @brief data 数据段
 * 
 */
class DataStackArea
{
public:
    DataStackArea(/* args */);
    ~DataStackArea();

private:
    /* data */
    // const static int poolsize = 256 * 1024;
    const static int poolsize = 128;



///< =====================================================
private:
    static char data_area[poolsize];
    static char *p_data_area;                       ///< data

public:
    static void UseDataArea(long int used_size)
    {
        p_data_area = p_data_area + used_size;
        return;
    }

    static char *GetNowDataAreaNowPointorPos()
    {
        return p_data_area;
    }

    /**
     * @brief 四字节对齐
     * 
     */
    static void DataAreaFourByteAlignment()
    {
        p_data_area = (char *)(((long int)p_data_area + sizeof(int)) & -sizeof(int));
        return; ///< 此处未检查越界 仅使用
    }

    static void SetStringToDataArea(const char *str, std::size_t len)
    {
        int eat_count = 0;
        while (eat_count <= len)
        {
            *p_data_area = str[eat_count];
            p_data_area++;
            eat_count++;
        }
        return;
    }

///<=======================================================
private:
    static long int emitted_code_area[poolsize];
    static long int *p_emitted_code;                ///< e le 

public:
    static void PrintAllEmittedCode()
    {
        printf("emitted_code_area:\n");
        for (int i = 0; i < poolsize; i++)
        {
            // printf("%d ", emitted_code_area[i]);
            Debug_Emitted(emitted_code_area[i]);
        }
        printf("\n");
        return;
    }

    static long int* NowPosEmittedCode()
    {
        return p_emitted_code;
    }

    static long int* PreviousPosEmittedCode()
    {
        return --p_emitted_code;
    }

    static long int* WatchEmittedCodePos(const long int &watch_size)
    {
        return p_emitted_code + watch_size;
    }

    static long int* UseEmittedCodeArea(long int used_size)
    {
        return p_emitted_code = p_emitted_code + used_size;
    }

    static void AddOneEmittedCode(long int operat)
    {
        ++p_emitted_code;
        *p_emitted_code = operat;
        return;
    }

///<===========================================================
private:
    static int local_variable_offset;               ///< loc local variable offset

public:
    static int GetLocalVariableOffset()
    {
        return local_variable_offset;
    }

    static int SetLocalVariableOffset(int set_val)
    {
        local_variable_offset = set_val;
    }

///<============================================================


};
#endif





