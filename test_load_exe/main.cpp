
#include <iostream>
#include <tchar.h>
#include "blu_win32.h" 

#include "main.h"
#include "mmLoader.h"


extern int planb(char *argv);
class AutoReleaseModuleBuffer {
public:
  AutoReleaseModuleBuffer(LPCTSTR szDllPath) : m_pBuffer(NULL), m_hFileMapping(NULL), m_hFile(NULL) {
    // Open the module and read it into memory buffer
    m_hFile = ::CreateFile(szDllPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
    if (INVALID_HANDLE_VALUE == m_hFile || NULL == m_hFile) {
      _tprintf(_T("Failed to open the file: %s\r\n"), szDllPath);
      return;
    }

    // Check file size
    DWORD dwFileSize = ::GetFileSize(m_hFile, NULL);
    if (INVALID_FILE_SIZE == dwFileSize || dwFileSize < sizeof(IMAGE_DOS_HEADER)) {
      ::CloseHandle(m_hFile);
      m_hFile = NULL;
      _tprintf(_T("Invalid file size: %d\r\n"), dwFileSize);
      return;
    }

    m_hFileMapping = ::CreateFileMapping(m_hFile, 0, PAGE_READONLY, 0, 0, NULL);
    if (NULL == m_hFileMapping) {
      ::CloseHandle(m_hFile);
      m_hFile = NULL;
      _tprintf(_T("Failed to create file mapping.\r\n"));
      return;
    }

    m_pBuffer = ::MapViewOfFile(m_hFileMapping, FILE_MAP_READ, 0, 0, 0);
    if (NULL == m_pBuffer) {
      ::CloseHandle(m_hFileMapping);
      ::CloseHandle(m_hFile);
      m_hFileMapping = NULL;
      m_hFile = NULL;
      _tprintf(_T("Failed to map view of the file.\r\n"));
    }
  }

  ~AutoReleaseModuleBuffer() { Release(); }

  void
  Release() {
    if (m_pBuffer) {
      ::UnmapViewOfFile(m_pBuffer);
      m_pBuffer = NULL;
    }

    if (m_hFileMapping) {
      ::CloseHandle(m_hFileMapping);
      m_hFileMapping = NULL;
    }

    if (m_hFile) {
      ::CloseHandle(m_hFile);
      m_hFile = NULL;
    }
  }

  operator LPVOID() { return m_pBuffer; }

private:
  LPVOID m_pBuffer;
  HANDLE m_hFile;
  HANDLE m_hFileMapping;
};


void
testdll() {

  // Return value
  int iRet = -1;

  // Initialize MEM_MODULE
  HMEMMODULE hMemModule = NULL;
  DWORD dwErrorCode = 0;

  // Here we just read the module data from disk file
  // In your real project you can download the module data from remote without writing to disk file
#ifdef _DEBUG
  TCHAR szDllPath[] = _T("dll_be_loaded.dll");
#else
  TCHAR szDllPath[] = _T("dll_be_loaded.dll");
#endif
  AutoReleaseModuleBuffer moduleBuffer(szDllPath);

  int flag = 1;
  // Load the module from the buffer
  hMemModule = (HMEMMODULE)MemModuleHelper(MHM_BOOL_LOAD, moduleBuffer, &flag, &dwErrorCode);

  // After the module was loaded we can release the original buffer
  moduleBuffer.Release();

  if (hMemModule) {
    _tprintf(_T("Module was loaded successfully. Module Base: 0x%p!\r\n"), (LPVOID)hMemModule);

    // Get address of function demoFunction
    char nn[]= "testdll";
    char *name = nn;
    LPVOID lpAddr = (LPVOID)MemModuleHelper(MHM_FARPROC_GETPROC, hMemModule, name, 0);
    if (lpAddr) {
      _tprintf(_T("Get address of demoFunction successfully. Address: 0x%p!\r\n"), lpAddr);

      // Function pointer type of demoFunction
      //typedef BOOL(_stdcall * Type_TargetFunction)(unsigned char *, unsigned int);
      typedef BOOL(_stdcall * Type_TargetFunction)();

      // Call the testdll() 
      Type_TargetFunction pfnFunction = (Type_TargetFunction)lpAddr;
      pfnFunction();

      //unsigned char buf[MAX_PATH] = {0};
      //if (pfnFunction(buf, MAX_PATH)) {
      //  //char *ppp = "{f56fee02-16d1-44a3-b191-4d7535f92ca5}";
      //  char pp[] = "{f56fee02-16d1-44a3-b191-4d7535f92ca5}";
      //  char *p = pp;
      //  iRet = ::memcmp(buf, p, strlen(p));
      //  if (0 == iRet)
      //    _tprintf(_T("Called target function demoFunction successfully with correct return value!\r\n"));
      //  else
      //    _tprintf(_T("Called target function demoFunction successfully, but returned unexpected value!\r\n"));
      //}
    } else
      _tprintf(_T("Failed to get address of demoFunction from memory module.\r\n"));

    // Free the module
    MemModuleHelper(MHM_VOID_FREE, hMemModule, 0, 0);
  } else {
    _tprintf(_T("Failed to load the module!\r\n"));
  }

}

void
testExe() 
{
  // Return value
  int iRet = -1;

  // Initialize MEM_MODULE
  HMEMMODULE hMemModule = NULL;
  DWORD dwErrorCode = 0;

  // Here we just read the module data from disk file
  // In your real project you can download the module data from remote without writing to disk file
  TCHAR szDllPath[] = _T("test_exe.exe");
  AutoReleaseModuleBuffer moduleBuffer(szDllPath);

  int flag = 2;
  // Load the module from the buffer
  hMemModule = (HMEMMODULE)MemModuleHelper(MHM_BOOL_LOAD, moduleBuffer, &flag, &dwErrorCode);

  // After the module was loaded we can release the original buffer
  moduleBuffer.Release();

  MemModuleHelper(MHM_VOID_FREE, hMemModule, 0, 0);
}



void
testExeWin32() 
{
  HMEMMODULE hMemModule = NULL;
  DWORD dwErrorCode = 0;

  TCHAR szDllPath[] = _T("testWin32.exe");
  AutoReleaseModuleBuffer moduleBuffer(szDllPath);

  int flag = 3;
  hMemModule = (HMEMMODULE)MemModuleHelper(MHM_BOOL_LOAD, moduleBuffer, &flag, &dwErrorCode);
  moduleBuffer.Release();
  MemModuleHelper(MHM_VOID_FREE, hMemModule, 0, 0);

}
void
testAct(TCHAR* exepath) 
{
  HMEMMODULE hMemModule = NULL;
  DWORD dwErrorCode = 0;

  //TCHAR szDllPath[] = _T("C:\\Windows\\system32\\calc.exe");
  TCHAR *szDllPath = exepath;

  AutoReleaseModuleBuffer moduleBuffer(szDllPath);

  int flag = 3;
  hMemModule = (HMEMMODULE)MemModuleHelper(MHM_BOOL_LOAD, moduleBuffer, &flag, &dwErrorCode);
  moduleBuffer.Release();
  MemModuleHelper(MHM_VOID_FREE, hMemModule, 0, 0);

}
int main(int argc, char* argv[]) 
{

    //changStdout("xxxxxx.txt");

  auto hi = GetModuleHandle(NULL);
  std::cout << "test_load_exe main act! hins:" << hi << std::endl;
	for (int i = 0; i < argc;i++) {
		std::cout << "argc["<<i<<"]:" <<  argv[i] << std::endl;
	}
        if (argc < 2) {
        
            std::cout << "no file to load!" << std::endl;
            return 0;
        }
    //testdll();
    //testExe();
    //testExeWin32();

    TCHAR *szExePath = charToWchar( argv[1] );

    //TCHAR szExePath[] = _T("C:\\Windows\\system32\\calc.exe");
    //TCHAR szExePath[] = _T("test_exe.exe");
    //TCHAR szExePath[] = _T("testWin32.exe");
    //TCHAR szExePath[] = _T("calc.exe");
    testAct(szExePath); //.faq 为什么calc.exe不能成功运行？ testWin32.exe 可以

    //char filename[] = "C:\\Windows\\system32\\calc.exe";
    //planb(filename); //无效

  return 0;
}



//win32编程入口方法,要改设置.  属性-> 链接 -> 系统 -> 窗口
int APIENTRY
wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
  std::cout << "test_load_exe wWinMain act! " << std::endl;
    //set_win32_params(hInstance,hPrevInstance,lpCmdLine,nCmdShow);

    char filename[] = "C:\\Windows\\system32\\calc.exe";
    //char filename[] = "testWin32.exe";
    
    TCHAR path[] = _T("C:\\Windows\\system32\\calc.exe");
    //set_win32_params(hInstance,hPrevInstance,path,1);
  
    //testExeWin32();
    //TCHAR szExePath[] = _T("testWin32.exe");
    TCHAR szExePath[] = _T("C:\\Windows\\system32\\calc.exe");
    testAct(szExePath); //.faq 为什么calc.exe不能成功运行？ testWin32.exe 可以

    //planb(filename); //无效
    return 0;
}

