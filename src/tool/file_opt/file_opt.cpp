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
#include "file_opt.h"
#include "error_code.h"


using namespace std;

CFileOpt::CFileOpt()
{
	m_Openfp = NULL;
}

CFileOpt::~CFileOpt()
{	
	CloseFile();
}

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
int CFileOpt::OpenFileOrCreate(const char *pathname, const char *flag)
{
    m_Openfp = NULL;
    m_Openfp = fopen(pathname, flag);
    if (NULL == m_Openfp)
	{
		Debug_Error("open file error, opened fd = %d\n", m_Openfp);
		perror("fopen perror:");
		return RETURN_FAIL;
	}
	return RETURN_SUCCESS;
}

/*
 * origin 从哪个位置开始偏移offset个字节
 * SEEK_SET 文件头
 * SEEK_CUR 当前文件指针
 * SEEK_END 文件末尾
*/
int CFileOpt::SeekFile(long offset, int origin)
{
	if (NULL == m_Openfp)
	{
		Debug_Error("file fp is NULL\n");
		return RETURN_FAIL;
	}
	
	fseek(m_Openfp, offset, origin);
	return RETURN_SUCCESS;
}

// 获取文件大小
size_t CFileOpt::GetFileSize()
{
	if (NULL == m_Openfp)
	{		
		Debug_Error("file fp is NULL\n");
		return RETURN_FAIL;
	}
	
	SeekFile(0, SEEK_END);
	size_t FileSize = ftell(m_Openfp);
	SeekFile(0, SEEK_SET);

	return FileSize;
}

// 读文件 readData读入内存地址 读入字节单位 读入数量
size_t CFileOpt::ReadFile(char *readData, const int &size, const int &readCount)
{
	if (NULL == readData)
	{
		Debug_Error("error:readData is NULL\n");
		return RETURN_FAIL;
	}

	int allNeedReadNum = readCount;
	int numNotRead = 0;
	size_t numReaded = 0;
	char *pRead = readData;

	size_t allReadedNum = 0;

	// 此处代码防止fread一次读太多 后来发现无用
	//while(allNeedReadNum > 0)
	{		
		// 如果大于102400 
		//if ((allNeedReadNum - 1024) > 0)
		{
			//numNotRead = 1024;
			//allNeedReadNum -= 1024;
		}
		//else
		{
			numNotRead = allNeedReadNum;
			//allNeedReadNum = 0;
		}
		
		while(numNotRead > 0)	// fread可能读多次
		{		
			if ((numReaded = fread(pRead, size, numNotRead, m_Openfp)) < 0)
			{
				Debug_Error("read error. read = %d\n", numReaded);
			}
			else if (numReaded == 0)
			{
				if (ferror (m_Openfp))
				{
					Debug_Error("File read error.");
					return RETURN_SUCCESS;
				}
			}
			else
			{
				pRead += numReaded;
				numNotRead -= numReaded;
				allReadedNum += numReaded; // 总计数
			}
		}
	}

	return allReadedNum;
}

size_t CFileOpt::WriteFile(char *data, const int &size, const int &writeCount)
{
	if (NULL == data)
	{
		Debug_Error("error:data is NULL.\n");
		return RETURN_FAIL;
	}

	int numNotwrite = writeCount;
	size_t numWritten = 0;
	char *pWrite = data;

	while (numNotwrite > 0)
	{
		if ((numWritten = fwrite(pWrite, size, numNotwrite, m_Openfp)) < 0)
		{
			Debug_Error("write error. write = %d\n", numWritten);
			return RETURN_FAIL;
		}
		else if (numWritten == 0)
		{
			return RETURN_SUCCESS;
		}
		else
		{
			pWrite += numWritten;
			numNotwrite -= numWritten;
		}
	}

	return numWritten;
}

#include <direct.h>     // _mkdir 在 MSVC
int CFileOpt::MakeDir(const char *pathName)
{
	int mkRet = _mkdir(pathName);		// 未确认 是否为Linux  window下 _mkdir 头文件为dircet.h
	// int mkRet = CreateDirectory(pathName, NULL);
	/*    
	S_ISUID 04000 文件的执行时设置用户ID（set user-id on execution）位
    S_ISGID 02000 文件的执行时设置组ID（set group-id on execution）位
    S_ISVTX 01000 文件的保存正文(粘着位sticky)位

    S_IRWXU            00700 文件所有者具有读、写、执行权限
	S_IRUSR（S_IREAD） 00400 文件所有者具可读取权限
	S_IWUSR（S_IWRITE）00200 文件所有者具可写入权限 
	S_IXUSR（S_IEXEC） 00100 文件所有者具可执行权限

	S_IRWXG 00070 用户组具有读、写、执行权限
	S_IRGRP 00040 用户组具可读取权限
	S_IWGRP 00020 用户组具可写入权限
	S_IXGRP 00010 用户组具可执行权限

	S_IRWXO 00007 其他用户具有读、写、执行权限
	S_IROTH 00004 其他用户具可读取权限
	S_IWOTH 00002 其他用户具可写入权限
	S_IXOTH 00001 其他用户具可执行权限

    */
	// int mkRet = mkdir(pathName, S_IRWXU|S_IRWXG|S_IRWXO);
	Debug_Error("mkret = %d\n",mkRet);
	if (mkRet < 0)
	{
		Debug_Error("error: mkdir failed. return:%d\n", mkRet);
		return RETURN_FAIL;
	}
	return RETURN_SUCCESS;
}

int CFileOpt::CloseFile()
{
	if (m_Openfp == NULL)
	{		
		Debug_Error("close file is NULL\n");
		return RETURN_SUCCESS;
	}

	if (fclose(m_Openfp) < 0)
	{
		Debug_Error("close return < 0\n");
		return RETURN_FAIL;
	}
    m_Openfp = NULL;

	Debug_Error("close file\n");
	
	return RETURN_SUCCESS;
}






