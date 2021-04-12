#include <algorithm> //transformת����Сд

#include "blu_win32.h"

#pragma comment(lib, "Psapi.lib") // psapi.h

//------ ----- ----- ----
//��ý��������ġ�CONTEXT ������ֶ�32λ��64λ�ǲ�һ��,��ʵ�ʼĴ�����������ͬ.���� 32λ�мĴ��� eax,ebx.. 64λ��Ӧ����
//rax,rbx...
//-----------------
// WINBASEAPI
// BOOL
// WINAPI
// GetThreadContext(
//    _In_ HANDLE hThread,
//    _Inout_ LPCONTEXT lpContext
//);
//����������� peb  //����?
PPEB
getPebByHandle(HANDLE hThread) {
  CONTEXT ctx;
  GetThreadContext(hThread, &ctx);
#ifdef _BLU_X64
  PPEB pPeb = (PPEB)ctx.Rbx;
#else
  PPEB pPeb = (PPEB)ctx.Ebx;
#endif
  return pPeb;
}

//��õ�ǰ����peb
PPEB
getPeb() {
#ifdef _WIN64
  PPEB pPeb = (PPEB)__readgsqword(0x60);
#else
  PPEB pPeb = (PPEB)__readfsdword(0x30);
#endif
  return pPeb;
}

//------ ----- ----- ----
// std::string std::wstring
//˵��wchar_t��char�������ʹ�Ҷ�����İ��
// wchar_t����windows����Unicode 16���룬Ҳ�����׳ƿ��ֽ�
// char����Ȼ����ָһ���ֽڣ���windows����Ĭ����gbk�����

wchar_t *
charToWchar(const char *c, size_t m_encode) {
  wchar_t *str;
  int len = MultiByteToWideChar(m_encode, 0, c, strlen(c), NULL, 0);
  wchar_t *m_wchar = new wchar_t[len + 1];
  MultiByteToWideChar(m_encode, 0, c, strlen(c), m_wchar, len);
  m_wchar[len] = '\0';
  return m_wchar;
}
std::string
wcharToChar(const wchar_t *wp, size_t m_encode) {
  int len = WideCharToMultiByte(m_encode, 0, wp, wcslen(wp), NULL, 0, NULL, NULL);
  char *m_char = new char[len + 1];
  WideCharToMultiByte(m_encode, 0, wp, wcslen(wp), m_char, len, NULL, NULL);
  m_char[len] = '\0';
  return m_char;
}
// https ://blog.csdn.net/lightspear/java/article/details/54695123
//-----

/*
��� L �� _T( )

�鿴tchar.hͷ�ļ��Ķ��壬���Է���_TEXT������_T����������һ���ģ����Ҷ���һ��Ԥ����ĺꡣ

1 #define _T(x)       __T(x)
2 #define _TEXT(x)    __T(x)
���������ٿ���__T(x)�Ķ��壬��������������
*/
// 1 #ifdef  _UNICODE
// 2 // ... ʡ����������
// 3 #define __T(x)      L ## x
// 4 // ... ʡ����������
// 5 #else   /* ndef _UNICODE */
// 6 // ... ʡ����������
// 7 #define __T(x)      x
// 8 // ... ʡ����������
// 9 #endif  /* _UNICODE */
/*


���Կ�����_T( ) ��һ������ĺ꣬�����̲���Unicode�ַ�ʱ _T�������� L��
�Ὣ���ֽڵ��ַ���ת��ΪUnicode�ַ��������̲��ö��ֽ��ַ���ʱ��
�ͻᵱ��һ����ַ�����������ת����

https://www.cnblogs.com/xxn-180727/p/9378519.html

*/

WCHAR *
getStringBuf() {
  WCHAR *xx = new WCHAR[MAX_PATH];
  memset(xx, 0, MAX_PATH);
  return xx;
}

// ��ȡ����·�� ����1������ID ����2��������ָ�룬����·��
void
getProcessPath(DWORD dwProcessID, WCHAR *buffer) {
  TCHAR Filename[MAX_PATH];
  HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessID);
  if (hProcess == NULL)
    return;
  HMODULE hModule;
  DWORD cbNeeded;
  if (EnumProcessModules(hProcess, &hModule, sizeof(hModule), &cbNeeded)) {
    if (GetModuleFileNameEx(hProcess, hModule, Filename, MAX_PATH)) {
      RtlMoveMemory((void *)buffer, Filename, sizeof(TCHAR) * MAX_PATH);
    }
  }
  CloseHandle(hProcess);
}

BOOL
userGetCommandLine(HANDLE hProc, LPCTSTR buffer) {
  PEB Peb = {0}; // todo 32λ,64λ
  BOOL ret = false;
  typedef NTSTATUS(WINAPI * NtQueryInfoProc)(HANDLE, PROCESSINFOCLASS, LPVOID, ULONG, PULONG);

  NtQueryInfoProc pfn = (NtQueryInfoProc)GetProcAddress(GetModuleHandle(_T("ntdll.dll")), "NtQueryInformationProcess");
  if (!pfn) //��ý�����Ϣ,δ�����ĺ���NtQueryInformationProcess
    return FALSE;

  PROCESS_BASIC_INFORMATION pbi;
  if (pfn(hProc, ProcessBasicInformation, &pbi, sizeof(pbi), NULL) != ERROR_SUCCESS) {
    std::cout << "get PROCESS_BASIC_INFORMATION err!" << std::endl;
    ;
    ;
    return FALSE;
  }

  SIZE_T dwVAddr;
  ret = ReadProcessMemory(hProc, pbi.PebBaseAddress, &Peb, sizeof(PEB), &dwVAddr);
  if (!ret) {
    std::cout << "get PEB err!" << std::endl;
    ;
    return FALSE;
  }
  // class aaa
  //{
  // public:
  //	int a;
  //};
  // auto XA = &aaa::a;
  //
  // aaa xxx;
  // aaa* xxxp = &xxx;
  // xxxp->*XA = 10;
  // auto xxxxxxx = &(xxxp->*XA);

  PRTL_USER_PROCESS_PARAMETERS ProcessParameters = Peb.ProcessParameters; //ָ��
  auto XImagePathName = &RTL_USER_PROCESS_PARAMETERS::ImagePathName;
  auto XCommandLine = &RTL_USER_PROCESS_PARAMETERS::CommandLine;

  // void* p = &(ProcessParameters->*XImagePathName); //��ȡִ���ļ�����Ŀ¼
  void *p = &(ProcessParameters->*XCommandLine); //��ȡִ���ļ� CommandLine

  UNICODE_STRING ReadMenory;
  ret = ReadProcessMemory(hProc, p, &ReadMenory, sizeof(UNICODE_STRING), &dwVAddr);
  if (!ret) {
    std::cout << "get ReadMenory 1 err!" << std::endl;
    ;
    return FALSE;
  }

  if (ReadMenory.Buffer == NULL) {
    std::cout << "get ReadMenory.Buffer == NULL !" << std::endl;
    ;
    return FALSE;
  }

  SIZE_T v1 = 0;
  if (ReadMenory.Length < MAX_PATH) {
    v1 = ReadMenory.Length;
  } else {
    v1 = MAX_PATH;
  }
  ret = ReadProcessMemory(hProc, ReadMenory.Buffer, (WCHAR *)buffer, v1, &v1);
  if (!ret) {
    std::cout << "get ReadMenory 2 err!" << std::endl;
    return FALSE;
  }
  return TRUE;
}

void
getProcessCommandLine(DWORD dwProcessID, WCHAR *buffer) {
  HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessID);
  userGetCommandLine(hProcess, (LPCTSTR)buffer);
  CloseHandle(hProcess);
}

//���Ŀ��
void
getProcessID(const char *ProcessMatch, std::vector<DWORD> &ret) {
  PROCESSENTRY32 pe32;

  pe32.dwSize = sizeof(PROCESSENTRY32);

  //��ȡ���̿���
  HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (hProcessSnap == INVALID_HANDLE_VALUE) {
    std::cout << "CreateToolhelp32Snapshot ����ʧ��.\n" << std::endl;
    return;
  }

  //�������̿��գ�������ʾÿ�����̵���Ϣ
  BOOL bMore = ::Process32First(hProcessSnap, &pe32);
  auto fstr = std::string(ProcessMatch);
  if (fstr.empty()) {
    return;
  }

  std::transform(fstr.begin(), fstr.end(), fstr.begin(), ::tolower);

  while (bMore) {
    // printf("�������ƣ�%s\n", pe32.szExeFile);
    // printf("�������ƣ�%ls\n", pe32.szExeFile); //�����unicode����,����õ���Ӧ���ǿ��ַ�����%ls,��Ȼ�޷�������ӡ
    // printf("����ID��%u\n\n", pe32.th32ProcessID);

    // std::cout << "�������ƣ�" << pe32.szExeFile << std::endl;
    // std::cout << "����ID��" << pe32.th32ProcessID << std::endl;
    // std::cout <<  std::endl;

    auto wText = pe32.szExeFile;
    auto exef = wText;

    auto pid = pe32.th32ProcessID;
    WCHAR *xx = getStringBuf();
    getProcessPath(pid, xx);
    auto exefullpath = wcharToChar(xx);
    std::transform(exefullpath.begin(), exefullpath.end(), exefullpath.begin(), ::tolower);
    std::cout << "try exef :" << exef << ",fullpath:" << exefullpath << std::endl;

    auto res = exefullpath.find(fstr);
    if (res == std::string::npos) {
      bMore = ::Process32Next(hProcessSnap, &pe32);
      continue;
    }

    WCHAR *xxx = getStringBuf();
    getProcessCommandLine(pid, xxx);
    auto cmdline = wcharToChar(xxx);

    // std::cout << "find :" << exef.c_str() << ", pid:" << pid << ",fullpath:" << exefullpath << ", cmdline:" <<
    // cmdline << std::endl;

    // BOOL matchCmdline=FALSE;
    // for (auto &k : filter)
    //{
    //	if (cmdline.find(k) != std::string::npos)
    //	{
    //		matchCmdline = TRUE;
    //		break;
    //	}
    //
    //}
    // if (!matchCmdline)
    //{
    //	bMore = ::Process32Next(hProcessSnap, &pe32);
    //	continue;
    //}
    // if (lstrcmp(pe32.szExeFile, ProcessName) == 0)
    //{
    //	::CloseHandle(hProcessSnap);
    //	return pe32.th32ProcessID;;
    //}

    ret.push_back(pid);

    bMore = ::Process32Next(hProcessSnap, &pe32);
  }

  //��Ҫ���������snapshot����
  ::CloseHandle(hProcessSnap);
}



//http://c.biancheng.net/view/7572.html
void changStdout(const char* filename) {
    freopen(filename, "w", stdout); 
}



