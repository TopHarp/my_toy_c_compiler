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
#ifndef _DEBUGPRINT_H
#define _DEBUGPRINT_H

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "file_opt.h"

#define PRINT_SWITCH_DEBUG (0)
#define PRINT_SWITCH_ERROR (1)

#define DEBUG_LOG_PATH "./log.txt"
#define PRINT_LOG_DATA_SIZE (256)


/*
#define Debug_Print(usrChr...) gdebug_print( __FILE__, __FUNCTION__, __LINE__, __TIME__, usrChr)
#define Debug_Perror(usrChr...) gdebug_print( __FILE__, __FUNCTION__, __LINE__, __TIME__, usrChr)

void gdebug_print(const char *file, const char *fun, int line, const char *time,const char *usrChr, ...);
void gdebug_perror(const char *file, const char *fun, int line, const char *time,const char *usrChr, ...);
*/


#define Debug_Error(usrChr...) gdebug_print(__FUNCTION__, __LINE__, __TIME__, usrChr)
#define Debug_Debug(usrChr...) gdebug_debug(__FUNCTION__, __LINE__, __TIME__, usrChr)
#define Debug_Token(token_type) gdebug_token(token_type)
#define Debug_Emitted(emitted) gdebug_emitted_code(emitted)

//#define Debug_Perror(usrChr...) gdebug_print(__FUNCTION__, __LINE__, __TIME__, usrChr)


void gdebug_print(const char *fun, int line, const char *time,const char *usrChr, ...);
void gdebug_debug(const char *fun, int line, const char *time,const char *usrChr, ...);
void gdebug_perror(const char *fun, int line, const char *time,const char *usrChr, ...);
void gdebug_token(int &token_type);
void gdebug_emitted_code(long int &emitted_code);

class CFileOpt;

static CFileOpt *g_pFileOpt;

// 打开日志文件
int gdebug_OpenLogFile();
// 将日志写入文件
int gdebug_LogWrite(char *data, const int Count);
// 程序结束关闭日志文件
int gdebug_CloseLogFile();








#define TIME_CHECK_SWITCH (1)
class cTimeCheck
{
	public:
		cTimeCheck();
		~cTimeCheck();

		void Start();
		void End();	
		void End(const char *data);
		
		void GetAvage();
		
	private:
		bool m_switch;
		struct timeval m_startT;
		int m_runConut;
		long m_totalUsec;

};




#endif




