// testWindows.cpp : 定义应用程序的入口点。
//

#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <vector>


#define MAX_LOADSTRING 100

CRITICAL_SECTION g_cri;   //临界
std::vector<HWND> g_wnds;


// 全局变量: 
HINSTANCE g_hInst;                                // 当前实例
HWND g_hWnd;                                    // 创建的自定义窗口句柄
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

char *consoleStr = 0;
HWND g_hdlg = 0;
HWND g_hdlg_create = 0;


// 此代码模块中包含的函数的前向声明: 
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

#define WM_EXIT_CMD  (WM_USER + 10000) 
#define WM_SHOW_MAIN (WM_USER + 10001) 

//---------------
//托盘,隐藏主窗口，只显示右下角图标  
    #define WM_TRAY (WM_USER + 100) 
    #define APP_NAME    TEXT("托盘程序")  
    #define APP_TIP     TEXT("Win32 API 实现系统托盘程序") 
    
    NOTIFYICONDATA nid;     //托盘属性  
    HMENU hMenu;            //托盘菜单  
//---------------


int consoleAct() {
	AllocConsole();
	freopen("conout$", "w", stdout);
	printf(" consoleAct! -_-\n");
	//std::cout << "i'm cout" << std::endl;
	freopen("conout$", "w", stderr);
	//std::cerr << "i'm cerr" << std::endl;
	return 0;
}




void addWnd(HWND id)
{
	::EnterCriticalSection(&g_cri);

	g_wnds.push_back(id);

	::LeaveCriticalSection(&g_cri);
}
int getWndIdx(HWND id)
{
	::EnterCriticalSection(&g_cri);

    for (auto i=0;i<g_wnds.size();i++)
    {
        if (id == g_wnds[i])
        {
	        ::LeaveCriticalSection(&g_cri);
            return i;
        }
    }

	::LeaveCriticalSection(&g_cri);
    return -1;
}




ATOM MyRegisterClass(HINSTANCE hInstance)
{
    
    WNDCLASSEXW wcex;
   
    wcex.cbSize = sizeof(WNDCLASSEX);//WNDCLASSEX 的大小。我们可以用sizeof（WNDCLASSEX）来获得准确的值。

    wcex.style          = CS_HREDRAW | CS_VREDRAW; //从这个窗口类派生的窗口具有的风格。您可以用“or”操作符来把几个风格或到一起。
    wcex.lpfnWndProc    = WndProc; // WindowProc  窗口处理函数的指针。
    wcex.cbClsExtra     = 0; //指定紧跟在窗口类结构后的附加字节数。
    wcex.cbWndExtra     = 0; //指定紧跟在窗口实例的附加字节数。如果一个应用程序在资源中用CLASS伪指令注册一个对话框类时，则必须把这个成员设成 DLGWINDOWEXTRA。
    wcex.hInstance      = hInstance;//本模块的实例句柄。
    //wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTWINDOWS)); //图标的句柄。
    wcex.hIcon          = 0;
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW); //光标的句柄。
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1); //背景画刷的句柄。
    //wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TESTWINDOWS); //指向菜单的指针。
    wcex.lpszMenuName = 0;
    wcex.lpszClassName  = szWindowClass; //指向类名称的指针。
    //wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL)); //和窗口类关联的小图标。如果该值为NULL。则把hIcon中的图标转换成大小合适的小图标。
    wcex.hIconSm = 0;

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{

	::InitializeCriticalSection(&g_cri);

   g_hInst = hInstance; // 将实例句柄存储在全局变量中

   auto xx = _T("xxx");// 主窗口类名
   memset(szWindowClass, 0,MAX_LOADSTRING);
   memcpy(szWindowClass, xx,6);
   MyRegisterClass(hInstance);
   
   //HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
   //   CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr); // szWindowClass 为前面注册的窗口类

   //if (!hWnd)
   //{
   //   return FALSE;
   //}

   //g_hWnd = hWnd;
   //ShowWindow(hWnd, nCmdShow);
   //UpdateWindow(hWnd);

   return TRUE;
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    auto wndidx = getWndIdx(hWnd);
    if (message != WM_GETICON)
    {//.faq  为什么一直有 WM_GETICON 消息
        std::cout << "wnd["<<wndidx<<"] WndProc message v[" << message << "]"<< std::endl;
    }

    switch (message )
    {
    case WM_CREATE:
    {
        addWnd(hWnd);
        wndidx = getWndIdx(hWnd);
        int x = 50 * wndidx;
        int y = 50 * wndidx;
        ::SetWindowPos(hWnd, nullptr,x,y,50,100, SWP_NOSIZE);
            //改变一个子窗口,弹出式窗口或顶层窗口的尺寸，位置和Z序。子窗口，弹出式窗口，
            //及顶层窗口根据它们在屏幕上出现的顺序排序,顶层窗口设置的级别最高，并且被设置为Z序的第一个窗口
    }
        break;
    default:
            DefWindowProc(hWnd, message, wParam, lParam); // 其它的走默认处理
    }

    return 1;
}


DWORD WINAPI ThreadPorc(LPVOID lpParam)
{

	DWORD dwThreadId = ::GetCurrentThreadId();

	HANDLE* hp = (HANDLE*)lpParam;
	DWORD tid = *(DWORD*)hp;

	HWND hWnd = ::CreateWindowW(
        szWindowClass, //classname 
        szTitle,        //windowname 
        WS_POPUP,//WS_OVERLAPPEDWINDOW, //style, WS_POPUP 不带标题栏
		0,  //x
        0,  //y 
        200, //w
        100,  //h 
        nullptr, 
        nullptr, 
        nullptr, 
        nullptr); // szWindowClass 为前面注册的窗口类


   ShowWindow(hWnd, true);
   UpdateWindow(hWnd);

   DWORD wtid = 0;
	DWORD lpdwProcessId = 0;
	wtid = GetWindowThreadProcessId(
		hWnd,
		&lpdwProcessId
	);


	auto mid = ::GetModuleHandleW(0);
	std::cout << "get thread id:" << dwThreadId << ", wtid:" << wtid << ", lpdwProcessId:" << lpdwProcessId << ", mid:" << mid << ", tid:" << tid << std::endl;

	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, 0, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	while (1) {
		Sleep(10);
	}
	return 0;
}


void test_thd()
{
    HANDLE hThread = 0;
    DWORD dwThreadId;

    HANDLE* hp = new HANDLE();

    hThread = ::CreateThread(
        NULL,           //默认安全属性
        NULL,           //默认堆栈
        ThreadPorc,     //线程入口方法
        hp,           //默认的线程参数, 可以是 NULL
        0,              //线程立即执行
        &dwThreadId     //线程id
    );

    *hp = hThread;
    std::cout << "thread id:" << dwThreadId << ", hp:"<< hp << ",hThread:"<< hThread  << std::endl;
}












int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    consoleAct();
		Sleep(3000);

    std::cout << " wWinMain mid:"<<hInstance<< std::endl;

    test_thd();
    //    Sleep(1000);
    //test_thd();
    //    Sleep(1000);
    //test_thd();

    //MSG msg;
	//while (GetMessage(&msg, nullptr, 0, 0))
	//{
	//	if (!TranslateAccelerator(msg.hwnd, 0, &msg))
	//	{
	//		TranslateMessage(&msg);
	//		DispatchMessage(&msg);
	//	}
	//}
	
        
        
	while (1) {
		Sleep(10000);
	}
    return 0;
}


int APIENTRY wWinMain1(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    consoleAct(); //在界面初始化(InitInstance) 之前,会影响最小化的图标显示 .faq
	Sleep(3000);
    
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {

        if (!TranslateAccelerator(msg.hwnd, 0, &msg))
		{
            TranslateMessage(&msg);
            DispatchMessage(&msg);
		}
    }
    return (int) msg.wParam;
}





