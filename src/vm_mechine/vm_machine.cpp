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
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "debug_print.h"
#include "vm_machine.h"
#include "vm_registers.h"
#include "token_type.h"
#include "system_operetor.h"

void VirtualMachine::VirtualMachineRun(long int *main_function_address)
{
    VmRegister::SetProgramCountor(main_function_address);

    VmRegister::AddStackPointer(VmRegister::vm_poolsize);

    VmRegister::PushStack(EXIT);
    VmRegister::PushStack(PSH);
    long int *tmp = VmRegister::GetStackPointer();
    // VmRegister::PushStack(argc);
    // VmRegister::PushStack((long int)argv);
    VmRegister::PushStack((long int)tmp);

    while (true)
    {
        long pc_current_code = VmRegister::NextProgramInstruction();
        VmRegister::OneCycleRun();

        // Debug_Emitted(pc_current_code);

        if (pc_current_code == LEA)
        {
            VmRegister::accumulator = 
                        (long)(VmRegister::p_base_pointer 
                        + VmRegister::NextProgramInstruction());
        }
        else if (pc_current_code == IMM)
        {
            VmRegister::accumulator = 
                        VmRegister::NextProgramInstruction();
        }
        else if (pc_current_code == JMP)
        {
            VmRegister::p_program_countor =
                        (long *)(*VmRegister::p_program_countor);
        }
        else if (pc_current_code == JSR)
        {
            VmRegister::PushStack((long)(VmRegister::p_program_countor + 1));
            VmRegister::p_program_countor = (long*)(*VmRegister::p_program_countor);
        }
        else if (pc_current_code == BZ)
        {
            VmRegister::p_program_countor = ((VmRegister::accumulator)
                    ? (VmRegister::p_program_countor + 1) 
                    : ((long *)(*VmRegister::p_program_countor)));
        }
        else if (pc_current_code == BNZ)
        {
            (VmRegister::p_program_countor = (long*)VmRegister::accumulator)
                    ? ((long *)(*VmRegister::p_program_countor))
                    : (VmRegister::p_program_countor + 1);
        }
        else if (pc_current_code == ENT)
        {
            VmRegister::PushStack((long)VmRegister::p_base_pointer);
            VmRegister::p_base_pointer = VmRegister::GetStackPointer();
            VmRegister::p_stack_pointer = VmRegister::p_stack_pointer 
                            - VmRegister::NextProgramInstruction();
        }
        else if (pc_current_code == ADJ)
        {
            VmRegister::p_stack_pointer = 
                        VmRegister::p_stack_pointer
                        + VmRegister::NextProgramInstruction();
        }
        else if (pc_current_code == LEV)
        {
            VmRegister::p_stack_pointer = VmRegister::p_base_pointer;
            VmRegister::p_base_pointer = 
                        (long *)(VmRegister::PopStack());
            VmRegister::p_program_countor = 
                        (long *)(VmRegister::PopStack());            
        }
        else if (pc_current_code == LI)
        {
            VmRegister::accumulator = *(int *)(VmRegister::accumulator);
        }
        else if (pc_current_code == LC)
        {
            VmRegister::accumulator = *(char *)(VmRegister::accumulator);            
        }
        else if (pc_current_code == SI)
        {
            *(int *)VmRegister::PopStack() = VmRegister::accumulator;
        }
        else if (pc_current_code == SC)
        {
            VmRegister::accumulator = 
                    *(char *)VmRegister::PopStack() = VmRegister::accumulator;            
        }
        else if (pc_current_code == PSH)
        {
            VmRegister::PushStack(VmRegister::accumulator);
        }
        else if (pc_current_code == OR)
        {
            VmRegister::accumulator 
                    = VmRegister::PopStack() | VmRegister::accumulator;
        }
        else if (pc_current_code == XOR)
        {
            VmRegister::accumulator 
                    = VmRegister::PopStack() ^ VmRegister::accumulator;
        }
        else if (pc_current_code == AND)
        {
            VmRegister::accumulator 
                    = VmRegister::PopStack() & VmRegister::accumulator;
        }
        else if (pc_current_code == EQ)
        {
            VmRegister::accumulator 
                    = VmRegister::PopStack() == VmRegister::accumulator;
        }
        else if (pc_current_code == NE)
        {
            VmRegister::accumulator 
                    = VmRegister::PopStack() != VmRegister::accumulator;
        }
        else if (pc_current_code == LT)
        {
            VmRegister::accumulator 
                    = VmRegister::PopStack() < VmRegister::accumulator;
        }
        else if (pc_current_code == GT)
        {
            VmRegister::accumulator 
                    = VmRegister::PopStack() > VmRegister::accumulator;
        }
        else if (pc_current_code == LE)
        {
            VmRegister::accumulator 
                    = VmRegister::PopStack() <= VmRegister::accumulator;
        }
        else if (pc_current_code == GE)
        {
            VmRegister::accumulator 
                    = VmRegister::PopStack() >= VmRegister::accumulator;
        }
        else if (pc_current_code == SHL)
        {
            VmRegister::accumulator 
                    = VmRegister::PopStack() << VmRegister::accumulator;
        }
        else if (pc_current_code == SHR)
        {
            VmRegister::accumulator 
                    = VmRegister::PopStack() >> VmRegister::accumulator;
        }
        else if (pc_current_code == ADD)
        {
            VmRegister::accumulator 
                    = VmRegister::PopStack() + VmRegister::accumulator;
        }
        else if (pc_current_code == SUB)
        {
            VmRegister::accumulator 
                    = VmRegister::PopStack() - VmRegister::accumulator;
        }
        else if (pc_current_code == MUL)
        {
            VmRegister::accumulator 
                    = VmRegister::PopStack() * VmRegister::accumulator;
        }
        else if (pc_current_code == DIV)
        {
            VmRegister::accumulator 
                    = VmRegister::PopStack() / VmRegister::accumulator;
        }
        else if (pc_current_code == MOD)
        {
            VmRegister::accumulator 
                    = VmRegister::PopStack() % VmRegister::accumulator;
        }
        else if (pc_current_code == OPEN)
        {
            VmRegister::accumulator 
                    = open((char *)VmRegister::p_stack_pointer[1],
                                *VmRegister::p_stack_pointer);
        }
        else if (pc_current_code == READ)
        {
            VmRegister::accumulator
                    = read(VmRegister::p_stack_pointer[2],
                                (char *)VmRegister::p_stack_pointer[1],
                                *VmRegister::p_stack_pointer);
        }
        else if (pc_current_code == CLOS)
        {
            VmRegister::accumulator 
                    = close(*VmRegister::p_stack_pointer);
        }
        else if (pc_current_code == PRTF)
        {
            tmp = VmRegister::p_stack_pointer 
                    + VmRegister::p_program_countor[1];
            VmRegister::accumulator = 
                    printf((char*)tmp[-1],
                            tmp[-2], tmp[-3],
                            tmp[-4], tmp[-5],
                            tmp[-6]);
        }
        else if (pc_current_code == MALC)
        {
            VmRegister::accumulator = 
                    (long)malloc(*VmRegister::p_stack_pointer);
        }
        else if (pc_current_code == MSET)
        {
            VmRegister::accumulator =
                    (long)memset((char*)VmRegister::p_stack_pointer[2],
                                    VmRegister::p_stack_pointer[1],
                                    *VmRegister::p_stack_pointer);
        }
        else if (pc_current_code == MCMP)
        {
            VmRegister::accumulator = 
                    memcmp((char*)VmRegister::p_stack_pointer[2],
                            (char*)VmRegister::p_stack_pointer[1],
                            *VmRegister::p_stack_pointer);
        }
        else if (pc_current_code == EXIT)
        {
            Debug_Error("exit(%d) cycle = %d\n", 
                            *VmRegister::p_stack_pointer,
                            VmRegister::GetCycle());
            return ;
        }
        else
        {
            Debug_Error("unknow instruction (%d) cycle = %d\n", 
                            pc_current_code,
                            VmRegister::GetCycle());
            return;
        }
    }
    
    return;
}

