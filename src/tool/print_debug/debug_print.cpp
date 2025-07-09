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
#include "debug_print.h"
#include "error_code.h"
#include "debug_display_char.h"
#include "system_operetor.h"


/*
void gdebug_print(const char *file, const char *fun, int line, const char *time,const char *usrChr, ...)
{
    printf("[%s:%s:%d][T%s] ",file, fun, line, time);
    va_list args;       //定义一个va_list类型的变量，用来储存单个参数
    va_start(args,usrChr); //使args指向可变参数的第一个参数
    vprintf(usrChr,args);  //必须用vprintf等带V的
    va_end(args);       //结束可变参数的获取
}


void gdebug_perror(const char *file, const char *fun, int line, const char *time,const char *usrChr, ...)
{
	printf("[%s:%s:%d][T%s] ",file, fun, line, time);
	perror("perror:");
	printf(". ");
	va_list args;		//定义一个va_list类型的变量，用来储存单个参数
	va_start(args,usrChr); //使args指向可变参数的第一个参数
	vprintf(usrChr,args);	//必须用vprintf等带V的
	va_end(args);		//结束可变参数的获取
}*/


void gdebug_print(const char *fun, int line, const char *time,const char *usrChr, ...)
{
#if PRINT_SWITCH_ERROR
    printf("[%s:%d][T%s] ",fun, line, time);
    va_list args;       //定义一个va_list类型的变量，用来储存单个参数
    va_start(args,usrChr); //使args指向可变参数的第一个参数
    vprintf(usrChr,args);  //必须用vprintf等带V的
    va_end(args);       //结束可变参数的获取
#endif
}

void gdebug_debug(const char *fun, int line, const char *time,const char *usrChr, ...)
{
#if PRINT_SWITCH_DEBUG
	char buffer[PRINT_LOG_DATA_SIZE] = {0};
	printf("[%s:%d][T%s] ",fun, line, time);
	snprintf(buffer, PRINT_LOG_DATA_SIZE, "[%s:%d][T%s] ", fun, line, time);
	
	va_list args;		//定义一个va_list类型的变量，用来储存单个参数
	va_start(args,usrChr); //使args指向可变参数的第一个参数	
	vprintf(usrChr,args);  //必须用vprintf等带V的
	vsnprintf(buffer+strlen(buffer), PRINT_LOG_DATA_SIZE, usrChr, args);
	va_end(args);		//结束可变参数的获取

	gdebug_LogWrite(buffer, strlen(buffer)); // 此处用strlen，因为实际看win端strlen包含了换行符，不在加1获得结束符，否则会有个0x00在换行第一个
#endif
}


void gdebug_perror(const char *fun, int line, const char *time,const char *usrChr, ...)
{
	printf("[%s:%d][T%s] ",fun, line, time);
	perror("perror:");
	printf(". ");
	va_list args;		    //定义一个va_list类型的变量，用来储存单个参数
	va_start(args,usrChr);  //使args指向可变参数的第一个参数
	vprintf(usrChr,args);	//必须用vprintf等带V的
	va_end(args);		    //结束可变参数的获取
}

void gdebug_token(int &token_type)
{
	printf(" %s ", debug_token_enum[token_type - 128]);
	return;
}

void gdebug_emitted_code(long int &emitted_code)
{
	if ((emitted_code >= LEA) && (emitted_code <= MOD))
	{
		printf(" %s ", debug_emitted_code[emitted_code - LEA]);
	}
	else if ((emitted_code >= OPEN) && (emitted_code <= MAIN))
	{
		printf(" %s ", system_operetor_char[emitted_code - OPEN]);
	}
	else
	{
		printf(" 0x%x ", emitted_code);
	}
	return;
}

// 打开日志文件
int gdebug_OpenLogFile()
{
	g_pFileOpt = NULL;
	g_pFileOpt = new CFileOpt;
	if (NULL == g_pFileOpt)
	{
    	printf("[%s:%d][T%s] new Log Opt failed.",__FUNCTION__, __LINE__, __TIME__);
		return RETURN_FAIL;
	}

	if (g_pFileOpt->OpenFileOrCreate(DEBUG_LOG_PATH, "w") < 0)
	{
		printf("write file open failed. error.\n");
		return RETURN_FAIL;
	}

	return RETURN_SUCCESS;
}


// 将日志写入文件
int gdebug_LogWrite(char *data, const int Count)
{
	if (NULL == g_pFileOpt)
	{
		return RETURN_FAIL;
	}
	
	g_pFileOpt->WriteFile(data, sizeof(char), Count);

	return RETURN_SUCCESS;
}


// 程序结束关闭日志文件
int gdebug_CloseLogFile()
{
	if (NULL == g_pFileOpt)
	{
		return RETURN_SUCCESS;
	}

	delete g_pFileOpt;

}


