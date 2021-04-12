#pragma once
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>

#include <tlhelp32.h> //���ϵͳ���̿���
#include <psapi.h>    //��ý�������·��,��Ҫ���� Psapi.lib
#include <winternl.h> //NtQueryInformationProcess () ��ý��������в���, PPEB, PEB�ṹ������ 
#include <tchar.h>

#ifdef  _DEBUG
	#define _BLU_DEBUG 
#endif //  _DEBUG

//WIN32 _WIN32 ��win32,windows���
//_WIN64 ��ʾ�Ƿ�Ϊ64λ
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


//�ض���cout������ļ�
void 
changStdout(const char* filename);

#ifdef __cplusplus
}
#endif
