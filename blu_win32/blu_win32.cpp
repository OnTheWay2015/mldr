#include <algorithm> //transform转换大小写

#include "blu_win32.h"

#pragma comment(lib, "Psapi.lib") // psapi.h

//------ ----- ----- ----
//获得进程上下文　CONTEXT 里面的字段32位和64位是不一样,和实际寄存器的名字相同.比如 32位有寄存器 eax,ebx.. 64位对应的是
//rax,rbx...
//-----------------
// WINBASEAPI
// BOOL
// WINAPI
// GetThreadContext(
//    _In_ HANDLE hThread,
//    _Inout_ LPCONTEXT lpContext
//);
//获得其他进程 peb  //测试?
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

//获得当前进程peb
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
//说道wchar_t和char两个类型大家都不会陌生
// wchar_t：在windows下是Unicode 16编码，也就是俗称宽字节
// char：当然就是指一个字节，在windows下面默认是gbk编码的

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
理解 L 和 _T( )

查看tchar.h头文件的定义，可以发现_TEXT（）和_T（）功能是一样的，而且都是一个预定义的宏。

1 #define _T(x)       __T(x)
2 #define _TEXT(x)    __T(x)
　　我们再看看__T(x)的定义，发现它有两个：
*/
// 1 #ifdef  _UNICODE
// 2 // ... 省略其它代码
// 3 #define __T(x)      L ## x
// 4 // ... 省略其它代码
// 5 #else   /* ndef _UNICODE */
// 6 // ... 省略其它代码
// 7 #define __T(x)      x
// 8 // ... 省略其它代码
// 9 #endif  /* _UNICODE */
/*


可以看出，_T( ) 是一个适配的宏，当工程采用Unicode字符时 _T（）就是 L，
会将多字节的字符串转化为Unicode字符，而工程采用多字节字符集时，
就会当作一般的字符串处理，不做转换。

https://www.cnblogs.com/xxn-180727/p/9378519.html

*/

WCHAR *
getStringBuf() {
  WCHAR *xx = new WCHAR[MAX_PATH];
  memset(xx, 0, MAX_PATH);
  return xx;
}

// 获取进程路径 参数1：进程ID 参数2：缓冲区指针，接收路径
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
  PEB Peb = {0}; // todo 32位,64位
  BOOL ret = false;
  typedef NTSTATUS(WINAPI * NtQueryInfoProc)(HANDLE, PROCESSINFOCLASS, LPVOID, ULONG, PULONG);

  NtQueryInfoProc pfn = (NtQueryInfoProc)GetProcAddress(GetModuleHandle(_T("ntdll.dll")), "NtQueryInformationProcess");
  if (!pfn) //获得进程信息,未公开的函数NtQueryInformationProcess
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

  PRTL_USER_PROCESS_PARAMETERS ProcessParameters = Peb.ProcessParameters; //指针
  auto XImagePathName = &RTL_USER_PROCESS_PARAMETERS::ImagePathName;
  auto XCommandLine = &RTL_USER_PROCESS_PARAMETERS::CommandLine;

  // void* p = &(ProcessParameters->*XImagePathName); //读取执行文件完整目录
  void *p = &(ProcessParameters->*XCommandLine); //读取执行文件 CommandLine

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

//获得目标
void
getProcessID(const char *ProcessMatch, std::vector<DWORD> &ret) {
  PROCESSENTRY32 pe32;

  pe32.dwSize = sizeof(PROCESSENTRY32);

  //获取进程快照
  HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (hProcessSnap == INVALID_HANDLE_VALUE) {
    std::cout << "CreateToolhelp32Snapshot 调用失败.\n" << std::endl;
    return;
  }

  //遍历进程快照，轮流显示每个进程的信息
  BOOL bMore = ::Process32First(hProcessSnap, &pe32);
  auto fstr = std::string(ProcessMatch);
  if (fstr.empty()) {
    return;
  }

  std::transform(fstr.begin(), fstr.end(), fstr.begin(), ::tolower);

  while (bMore) {
    // printf("进程名称：%s\n", pe32.szExeFile);
    // printf("进程名称：%ls\n", pe32.szExeFile); //如果是unicode工程,这里得到的应该是宽字符，用%ls,不然无法正常打印
    // printf("进程ID：%u\n\n", pe32.th32ProcessID);

    // std::cout << "进程名称：" << pe32.szExeFile << std::endl;
    // std::cout << "进程ID：" << pe32.th32ProcessID << std::endl;
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

  //不要忘记清除掉snapshot对象
  ::CloseHandle(hProcessSnap);
}



//http://c.biancheng.net/view/7572.html
void changStdout(const char* filename) {
    freopen(filename, "w", stdout); 
}



