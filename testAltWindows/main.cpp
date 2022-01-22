// testWindows.cpp : ����Ӧ�ó������ڵ㡣
//

#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <vector>

#include "test_alt_win.h"
#define MAX_LOADSTRING 100

CRITICAL_SECTION g_cri;   //�ٽ�
std::vector<HWND> g_wnds;


// ȫ�ֱ���: 
HINSTANCE g_hInst;                                // ��ǰʵ��
HWND g_hWnd;                                    // �������Զ��崰�ھ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������

char *consoleStr = 0;
HWND g_hdlg = 0;
HWND g_hdlg_create = 0;


// �˴���ģ���а����ĺ�����ǰ������: 
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

#define WM_EXIT_CMD  (WM_USER + 10000) 
#define WM_SHOW_MAIN (WM_USER + 10001) 

//---------------
//����,���������ڣ�ֻ��ʾ���½�ͼ��  
    #define WM_TRAY (WM_USER + 100) 
    #define APP_NAME    TEXT("���̳���")  
    #define APP_TIP     TEXT("Win32 API ʵ��ϵͳ���̳���") 
    
    NOTIFYICONDATA nid;     //��������  
    HMENU hMenu;            //���̲˵�  
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







ATOM MyRegisterClass(HINSTANCE hInstance)
{
    
    WNDCLASSEXW wcex;
   
    wcex.cbSize = sizeof(WNDCLASSEX);//WNDCLASSEX �Ĵ�С�����ǿ�����sizeof��WNDCLASSEX�������׼ȷ��ֵ��

    wcex.style          = CS_HREDRAW | CS_VREDRAW; //����������������Ĵ��ھ��еķ���������á�or�����������Ѽ�������һ��
    wcex.lpfnWndProc    = WndProc; // WindowProc  ���ڴ�������ָ�롣
    wcex.cbClsExtra     = 0; //ָ�������ڴ�����ṹ��ĸ����ֽ�����
    wcex.cbWndExtra     = 0; //ָ�������ڴ���ʵ���ĸ����ֽ��������һ��Ӧ�ó�������Դ����CLASSαָ��ע��һ���Ի�����ʱ�������������Ա��� DLGWINDOWEXTRA��
    wcex.hInstance      = hInstance;//��ģ���ʵ�������
    //wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTWINDOWS)); //ͼ��ľ����
    wcex.hIcon          = 0;
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW); //���ľ����
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1); //������ˢ�ľ����
    //wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TESTWINDOWS); //ָ��˵���ָ�롣
    wcex.lpszMenuName = 0;
    wcex.lpszClassName  = szWindowClass; //ָ�������Ƶ�ָ�롣
    //wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL)); //�ʹ����������Сͼ�ꡣ�����ֵΪNULL�����hIcon�е�ͼ��ת���ɴ�С���ʵ�Сͼ�ꡣ
    wcex.hIconSm = 0;

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{

	::InitializeCriticalSection(&g_cri);

   g_hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   auto xx = _T("xxx");// ����������
   memset(szWindowClass, 0,MAX_LOADSTRING);
   memcpy(szWindowClass, xx,6);
   MyRegisterClass(hInstance);
   
   //HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
   //   CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr); // szWindowClass Ϊǰ��ע��Ĵ�����

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
    if (message != WM_GETICON)
    {//.faq  Ϊʲôһֱ�� WM_GETICON ��Ϣ
        std::cout << std::hex<< "WndProc message v["  << message << "]"<< std::endl;
    }

    switch (message )
    {
    case WM_CREATE:
    {
        auto wndidx = 1;
        int x = 50 * wndidx;
        int y = 50 * wndidx;
        ::SetWindowPos(hWnd, nullptr,x,y,50,100, SWP_NOSIZE);
            //�ı�һ���Ӵ���,����ʽ���ڻ򶥲㴰�ڵĳߴ磬λ�ú�Z���Ӵ��ڣ�����ʽ���ڣ�
            //�����㴰�ڸ�����������Ļ�ϳ��ֵ�˳������,���㴰�����õļ�����ߣ����ұ�����ΪZ��ĵ�һ������
        ::ShowWindow(hWnd, true);
    }
        break;
    default:
            DefWindowProc(hWnd, message, wParam, lParam); // ��������Ĭ�ϴ���
    }

    return 1;
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    consoleAct(); //�ڽ����ʼ��(InitInstance) ֮ǰ,��Ӱ����С����ͼ����ʾ .faq
	Sleep(1000);

    // ��������
    //CMainWindow wnd;


    //CMainWindowB wnd;
    CMainWindow wnd;
    wnd.Create(NULL, CWindow::rcDefault, _T("My Window"), WS_OVERLAPPEDWINDOW, WS_EX_CLIENTEDGE, nullptr);



    // ��ʾ�����´���
    wnd.ShowWindow(nCmdShow);
    wnd.UpdateWindow();



    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        
        if (msg.message == WM_CREATE)
        {
        
            std::cout<< std::hex<< "on create, hWnd[0x" << msg.hwnd<< "]"<< std::endl;
        
        }
        if (!TranslateAccelerator(msg.hwnd, 0, &msg))
		{
            TranslateMessage(&msg);
            DispatchMessage(&msg);
		}
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

    consoleAct(); //�ڽ����ʼ��(InitInstance) ֮ǰ,��Ӱ����С����ͼ����ʾ .faq
	Sleep(3000);
    
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr); // szWindowClass Ϊǰ��ע��Ĵ�����

   if (!hWnd)
   {
      return FALSE;
   }

   g_hWnd = hWnd;
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





