#include <iostream>
#include <windows.h>

#include <atlbase.h>
#include <atlwin.h>




int main(int argc, char* argv[]) 
{

  auto hi = GetModuleHandle(NULL);
	std::cout << "test_exe main act! hins:" <<  hi << std::endl;

	for (int i = 0; i < argc;i++) {
		std::cout << "argc["<<i<<"]:" <<  argv[i] << std::endl;
	}
  return 0;

}





class CMainWindowBB : public CWindowImpl<CMainWindowBB> 
{
public:
	BOOL ProcessWindowMessage(
		_In_ HWND hWnd,
		_In_ UINT uMsg,
		_In_ WPARAM wParam,
		_In_ LPARAM lParam,
		_Inout_ LRESULT& lResult,
		_In_ DWORD dwMsgMapID);

	private:
	CContainedWindow m_cc;
	CContainedWindow m_cc1;
};



BOOL CMainWindowBB::ProcessWindowMessage(
    _In_ HWND hWnd,
    _In_ UINT uMsg,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam,
    _Inout_ LRESULT& lResult,
    _In_ DWORD dwMsgMapID)
{

        std::cout<< std::hex<< "b ProcessWindowMessage message[0x" << uMsg << "]"<< std::endl;

 //BEGIN_MSG_MAP(CMainWindow)
    BOOL bHandled = TRUE; 
        (hWnd); 
        (uMsg); 
        (wParam); 
        (lParam); 
        (lResult); 
        (bHandled); 
        switch (dwMsgMapID) 
        { 
        case 0:

        //MESSAGE_HANDLER(WM_PAINT, OnPaint) // if消息分发分支

			if (uMsg == WM_CREATE)
			{


                {
                RECT rc= { 100, 100, 100, 100};
                //m_cc.Create(_T("edit"),this,123,hWnd,rc);
                auto h = m_cc.Create(_T("button"),this,123,hWnd,rc);
                m_cc.SetWindowTextW(_T("a1"));
                
                ::SetWindowPos(h,NULL,50,50,100,100, SWP_SHOWWINDOW);
               
                //::ShowWindow(h, SWP_SHOWWINDOW);
                //m_cc.Create(_T("edit"),this,123,hWnd,&CWindow::rcDefault);
                }


			}
        if (uMsg == WM_COMMAND)
        {
            //响应按钮  wParam 为 按钮id
        }
            
//#define END_MSG_MAP() 
			break;
        
        case 123:
            if (uMsg == WM_SIZE)
            {
                //if (!wParam)
                //{
                //}
                RECT* p = (RECT*)(lParam);

            }
            
            //if (uMsg == WM_PAINT)
            //{
            //    ::SetWindowPos(hWnd,NULL,0,0,100,100, SWP_SHOWWINDOW);
            //}
            break;
        default: 
			ATLTRACE(static_cast<int>(ATL::atlTraceWindowing), 0, _T("Invalid message map ID (%i)\n"), dwMsgMapID); \
			ATLASSERT(FALSE); 
			break; 
		} 
    return 0;

}



class CMainWindoww : public CWindowImpl<CMainWindoww> 
{
public:
	BOOL ProcessWindowMessage(
		_In_ HWND hWnd,
		_In_ UINT uMsg,
		_In_ WPARAM wParam,
		_In_ LPARAM lParam,
		_Inout_ LRESULT& lResult,
		_In_ DWORD dwMsgMapID);

};

CMainWindowBB* _subwnd;

BOOL CMainWindoww::ProcessWindowMessage(
    _In_ HWND hWnd,
    _In_ UINT uMsg,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam,
    _Inout_ LRESULT& lResult,
    _In_ DWORD dwMsgMapID)
{

        std::cout<< std::hex<< "ProcessWindowMessage message[0x" << uMsg << "]"<< std::endl;

 //BEGIN_MSG_MAP(CMainWindow)
    BOOL bHandled = TRUE; 
        (hWnd); 
        (uMsg); 
        (wParam); 
        (lParam); 
        (lResult); 
        (bHandled); 
        switch (dwMsgMapID) 
        { 
        case 0:

        //MESSAGE_HANDLER(WM_PAINT, OnPaint) // if消息分发分支

			if (uMsg == WM_CREATE)
			{
                _subwnd = new CMainWindowBB();
                auto h = _subwnd->Create(hWnd, CWindow::rcDefault, _T("sub Window"), WS_CHILDWINDOW, NULL, nullptr);
                
                //auto h = _subwnd->Create(hWnd, CWindow::rcDefault, _T("sub Window"), WS_OVERLAPPEDWINDOW, WS_EX_CLIENTEDGE, nullptr);
                //::ShowWindow(h, SW_SHOW);

                _subwnd->SetWindowPos(NULL,0,0,600,600, SWP_SHOWWINDOW);
                //if (h)
                //{
                //    ::SetWindowPos(h,NULL,0,0,600,600, SWP_SHOWWINDOW);
                //}

			}
        if (uMsg == WM_COMMAND)
        {
            //响应按钮  wParam 为 按钮id
        }
            
//#define END_MSG_MAP() 
			break;
        
        case 123:
            std::cout << "mark" << std::endl;
            if (uMsg == WM_SIZE)
            {
                //if (!wParam)
                //{
                //}
                RECT* p = (RECT*)(lParam);

            }
            
            //if (uMsg == WM_PAINT)
            //{
            //    ::SetWindowPos(hWnd,NULL,0,0,100,100, SWP_SHOWWINDOW);
            //}
            break;
        default: 
			ATLTRACE(static_cast<int>(ATL::atlTraceWindowing), 0, _T("Invalid message map ID (%i)\n"), dwMsgMapID); \
			ATLASSERT(FALSE); 
			break; 
		} 
    return 0;

}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{

	Sleep(1000);

    // 创建窗口
    //CMainWindow wnd;


    CMainWindoww wnd;
    //CMainWindowBB wnd;
    wnd.Create(NULL, CWindow::rcDefault, _T("My Window"), WS_OVERLAPPEDWINDOW, WS_EX_CLIENTEDGE, nullptr);


    // 显示并更新窗口
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
