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
#ifndef _VM_REGISTERS_
#define _VM_REGISTERS_

#include "debug_print.h"

class VmRegister
{
public:
    VmRegister(/* args */);
    ~VmRegister();

public:
    /* data */
    // const static int vm_poolsize = 256 * 1024;
    const static int vm_poolsize = 128;


///<=====================================================
public:
    static long int stack_area[vm_poolsize];              ///< 
    static long int *p_stack_pointer;                  ///< sp

public:
    static long int *AddStackPointer(const long int &set_val)
    {
        return p_stack_pointer = p_stack_pointer + set_val;
    }

    static long int *GetStackPointer()
    {
        return p_stack_pointer;
    }

    static void PushStack(long int val)
    {
        --p_stack_pointer;
        *p_stack_pointer = val;
    }

    static long int PopStack()
    {
        return *p_stack_pointer++;
    }

///<=====================================================
public:
    static long int *p_program_countor;              ///< pc
public:
    static long int *AddProgramCountor(const long int &set_val)
    {
        return p_program_countor = p_program_countor + set_val;
    }

    static long int *SetProgramCountor(long int *set_val)
    {
        return p_program_countor = set_val;
    }

    static long int NextProgramInstruction()
    {
        Debug_Emitted(*p_program_countor);
        return *p_program_countor++;    ///< *a; a = a + 1; 
    }

    static long int *GetProgramCountor()
    {
        return p_program_countor;
    }

///<=====================================================
public:
    static long int *p_base_pointer;            ///< bp
public:
    static long int *AddBasePointer(const long int &set_val)
    {
        return p_base_pointer = p_base_pointer + set_val;
    }

    static long int *SetBasePointer(long int *set_val)
    {
        return p_base_pointer = set_val;
    }

    static long int *GetBasePointer()
    {
        return p_base_pointer;
    }


///<=====================================================
public:
    static long int accumulator;            ///< a accumulator 累加寄存器
public:
    static long int SetAccumulator(long int &set_val)
    {
        return accumulator = set_val;
    }

    static long int GetAccumulator()
    {
        return accumulator;
    }

///<=====================================================
private:
    static int machine_cycle;            ///< cycle
public:
    static void OneCycleRun()
    {
        ++machine_cycle;
    }

    static int GetCycle()
    {
        return machine_cycle;
    }
    

};
#endif





