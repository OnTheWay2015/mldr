#pragma once
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>

#include <tlhelp32.h> //获得系统进程快照
#include <psapi.h>    //获得进程完整路径,需要依赖 Psapi.lib
#include <winternl.h> //NtQueryInformationProcess () 获得进程命令行参数, PPEB, PEB结构体声明 
#include <tchar.h>

#ifdef  _DEBUG
	#define _BLU_DEBUG 
#endif //  _DEBUG

//WIN32 _WIN32 表win32,windows编程
//_WIN64 表示是否为64位
#ifdef  _WIN64
	#define _BLU_X64
#endif


#ifdef BLU_DLL
#define BLU_API __declspec(dllexport)
#else
#define BLU_API __declspec(dllimport)
#endif



#ifdef __cplusplus
extern "C" {
#endif

wchar_t *
charToWchar(const char *c, size_t m_encode = CP_ACP);
std::string
wcharToChar(const wchar_t *wp, size_t m_encode = CP_ACP);


//重定向cout输出到文件
void 
changStdout(const char* filename);

#ifdef __cplusplus
}
#endif
