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
#ifndef _FILEOPT_H
#define _FILEOPT_H

#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
// #include <direct.h>
// #include <windows.h>
#include <stddef.h>

#include "debug_print.h"


// 一个文件操作类 只存一个fd类在内部
class CFileOpt
{
    public:
        CFileOpt();
        ~CFileOpt();
		
		/* flag
			r 以只读方式打开文件，该文件必须存在。

		　　r+ 以可读写方式打开文件，该文件必须存在。

		　　rb+ 读写打开一个二进制文件，只允许读写数据。

		　　rt+ 读写打开一个文本文件，允许读和写。

		　　w 打开只写文件，若文件存在则文件长度清为0，即该文件内容会消失。若文件不存在则建立该文件。

		　　w+ 打开可读写文件，若文件存在则文件长度清为零，即该文件内容会消失。若文件不存在则建立该文件。

		　　a 以附加的方式打开只写文件。若文件不存在，则会建立该文件，如果文件存在，写入的数据会被加到文件尾，即文件原先的内容会被保留。（EOF符保留）

		　　a+ 以附加方式打开可读写的文件。若文件不存在，则会建立该文件，如果文件存在，写入的数据会被加到文件尾后，即文件原先的内容会被保留。 （原来的EOF符不保留）

		　　wb 只写打开或新建一个二进制文件；只允许写数据。

		　　wb+ 读写打开或建立一个二进制文件，允许读和写。

		　　wt+ 读写打开或着建立一个文本文件；允许读写。

		　　at+ 读写打开一个文本文件，允许读或在文本末追加数据。

		　　ab+ 读写打开一个二进制文件，允许读或在文件末追加数据。 
		*/
        int OpenFileOrCreate(const char *pathname, const char *flag);
		/*
		 * origin 从哪个位置开始偏移offset个字节
		 * SEEK_SET 文件头
		 * SEEK_CUR 当前文件指针
		 * SEEK_END 文件末尾
		*/
        int SeekFile(long offset, int origin);
		// 获取文件大小
		size_t GetFileSize();
        size_t ReadFile(char *readData, const int &size, const int &readCount);
        size_t WriteFile(char *data, const int &size, const int &writeCount);
        int CloseFile();
        int MakeDir(const char *pathName);


    private:
        //int m_Openfd;
		FILE *m_Openfp;


};

#endif







