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
#include <stdio.h>
#include <cstdio>
#include <stdlib.h>
#include <memory>
#include "debug_print.h"
#include "file_opt.h"
#include "error_code.h"
#include "source_code_buf.h"
#include "token.h"
#include "debug_display_char.h"
#include "identifier_map.h"
#include "parser.h"
#include "vm_registers.h"
#include "vm_machine.h"

int ParseMainLoop()
{
    CSourceCodeBuf source_code;
    // if (RETURN_SUCCESS != source_code.ReadSourceFile("/home/sda/gyxu_workspace/tmp_p/toy_comp/toy-comp/toy_compiler/test/test_nowchar.cpp"))
    if (RETURN_SUCCESS != source_code.ReadSourceFile("D:\\F\\github\\my_toy_c_compiler\\test\\test_nowchar.cpp"))
    {
        return 0;
    }
    // Debug_Error("%s", source_code.GetBuffer().get());             ///< test 

    CIdentifierMap identifier_map;
    CTokenLexer token_lexer(&source_code, &identifier_map);
    CParser parser;

    Debug_Error("Main Loop \n\n\n\n");

    parser.ParseCode(token_lexer);

    IdentifierMapItr main_itr = identifier_map.FindID("main");
    if (main_itr == identifier_map.GetEnd())
    {
        Debug_Error("cant find main function.\n");
        return 0;
    }

    long int *main_function_address = (long int*)(main_itr->second.value);
    if (0 == main_function_address)
    {
        Debug_Error("cant find main function address. %d\n",
                                main_function_address);
        return 0;
    }

    VirtualMachine virtual_machine;
    virtual_machine.VirtualMachineRun(main_function_address);


    return 0;
}

#include <cpuid.h>

int main()
{

    {
        unsigned int eax, ebx, ecx, edx;
        __get_cpuid(1, &eax, &ebx, &ecx, &edx);
        if (!(ecx & bit_AVX)) {
            printf("AVX not supported!\n");
            // return 1;
        }
        // 安全使用 AVX 的代码
        // return 0;
    }

    Debug_Error("hello \n");

    ParseMainLoop();

    return 0;
}






