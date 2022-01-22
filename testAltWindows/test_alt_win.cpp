#include <iostream>

#include "test_alt_win.h"
//https://www.cnblogs.com/liangxiaofeng/p/5066026.html


#include <iomanip>

static const int subMsgMapID = 111;
static HWND _subHwnd = 0;
//static CMainWindowSub* _subWin;
static CMainWindowB* _subWin;

BOOL CMainWindow::ProcessWindowMessage(
    _In_ HWND hWnd,
    _In_ UINT uMsg,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam,
    _Inout_ LRESULT& lResult,
    _In_ DWORD dwMsgMapID)
{
    //消息处理宏块不仅仅可以处理父窗口消息（当 dwMsgMapID = 0，即当前这些消息响应成员函数所在的窗口对象），
    //也可以处理子窗口消息（ dwMsgMapID 非0，此时实际是通过 ALT_MSG_MAP 宏来转移）

    //std::cout << "xxxx" << std::endl;
        std::cout<< std::setiosflags(std::ios::uppercase) << std::hex<< "ProcessWindowMessage message[0x" << uMsg << "]"<< std::endl;

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
				//auto hwndButtonCintinue = ::CreateWindow(TEXT("button"),//必须为：button    
				//	TEXT("按钮"),//按钮上显示的字符    
				//	WS_CHILD | WS_VISIBLE,
				//	100, 240, 100, 50,  //按钮在界面上出现的位置
				//	hWnd, (HMENU)123,  //设置按钮IDIDC_BUTTON_CONTINUE = 131自己定义ID
				//	NULL, NULL);

                 
				//_subWin = new CMainWindowSub(_T("edit"),this,subMsgMapID);
				//_subWin = new CMainWindowSub(_T("button"),this,subMsgMapID);
				_subWin = new CMainWindowB();

                //auto h = _subWin->Create(hWnd, CWindow::rcDefault, _T("sub Window"), WS_OVERLAPPEDWINDOW, WS_EX_CLIENTEDGE, nullptr); //独立窗口
                auto h = _subWin->Create(hWnd, CWindow::rcDefault, _T("sub Window"), WS_CHILDWINDOW, NULL, nullptr); //子窗口
                //ShowWindow(h, SW_SHOW);//一定要有个调用触发显示
                _subWin->SetWindowPos(NULL,0,0,600,600, SWP_SHOWWINDOW);
                //if (h)
                //{
                //    ::SetWindowPos(h,NULL,0,0,600,600, SWP_SHOWWINDOW);
                //}

                //if (_subHwnd  == nullptr)
                //{
                //    auto err = GetLastError();
                //    std::cout<< std::dec<< "create, sub Wnd error:"<< err << std::endl;
                //}
                //else
                //{ // err
                //    //_subWin->Attach(_subHwnd);
                //    //::CallWindowProc(_subWin->WindowProc, _subHwnd, uMsg, wParam, lParam);
                //    //_subWin->SetWindowPos(nullptr, 0, 0, 100, 100, 0);
                //    _subWin->SetWindowTextW(_T("btn"));
                //}

			}
        if (uMsg == WM_LBUTTONDOWN)
        {
        }
        if (uMsg == WM_COMMAND)
        {
            //响应按钮  wParam 为 按钮id
        }
            
//#define END_MSG_MAP() 
			break;
        case subMsgMapID:
            if (_subHwnd)
            {
                //::CallWindowProc(_subHwnd, hWnd, uMsg, wParam, lParam);
                //::CallWindowProc(_subWin->WindowProc, hWnd, uMsg, wParam, lParam);
            }
            if (uMsg == WM_SIZE)
            {
                //if (!wParam)
                //{
                //}
                RECT* p = (RECT*)(lParam);

            }
            
            break;
        default: 
			ATLTRACE(static_cast<int>(ATL::atlTraceWindowing), 0, _T("Invalid message map ID (%i)\n"), dwMsgMapID); \
			ATLASSERT(FALSE); 
			break; 
		} 
    return 0;

}




    LRESULT CMainWindow::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) { // ATL消息处理函数的标准形式
        //PAINTSTRUCT ps;
        //this->BeginPaint(&ps); // 开始绘图
        //// 在这里进行绘图操作
        //this->EndPaint(&ps); // 结束绘图
        // bHandled如果不手动赋值FALSE的话，默认为TRUE
        std::cout << "xxxx1" << std::endl;

        PAINTSTRUCT pt;
        HDC hdc;
        auto hwnd = this->m_hWnd;
        hdc = ::BeginPaint(hwnd, &pt);
        TextOut(hdc, 200, 200, TEXT("3322"), 2);



        wchar_t buf[512];
        auto len = ::GetWindowText(hwnd,buf,512);//len 实际获得的字符长度.  hwnd 也可以是文本控件

        TextOut(hdc, 200, 300, buf, len);
        ::EndPaint(hwnd, &pt);
        return 0;
    }
    LRESULT CMainWindow::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
        //PostQuitMessage(0); // 退出消息循环
        std::cout << "xxxx2" << std::endl;
        return 0;
    }
    LRESULT CMainWindow::OnAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) { // ATL命令处理函数的标准形式
        //CSimpleDialog<IDD_ABOUTBOX> dlg;
        //dlg.DoModal(); // 显示『关于』对话框
        return 0;
    }

    LRESULT CMainWindow::OnExit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
        this->DestroyWindow(); // 点击文件->关闭时，销毁窗口
        return 0;
    }



BOOL CMainWindowB::ProcessWindowMessage(
    _In_ HWND hWnd,
    _In_ UINT uMsg,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam,
    _Inout_ LRESULT& lResult,
    _In_ DWORD dwMsgMapID)
{

        std::cout<< std::setiosflags(std::ios::uppercase) << std::hex<< "b ProcessWindowMessage message[0x" << uMsg << "]"<< std::endl;

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

				//_subWin = new CMainWindowSub(_T("edit"),this,subMsgMapID);

                RECT rc= { 100, 100, 100, 100};
                auto h = m_cc.Create(_T("edit"),this,subMsgMapID,hWnd,rc);
                //auto h =  m_cc.Create(_T("button"),this,subMsgMapID,hWnd,&CWindow::rcDefault);
                //auto h =  m_cc.Create(_T("button"),this,subMsgMapID,hWnd,rc);
                m_cc.SetWindowTextW(_T("mark text"));
                ::SetWindowPos(h,NULL,50,50,100,100, SWP_SHOWWINDOW);//一定要有个调用,设置大小,触发显示

                //::ShowWindow(h, SW_SHOW);
                //if (h)
                //{
                //    ::SetWindowPos(h,NULL,0,0,200,100, SWP_SHOWWINDOW);
                //}
			}
        if (uMsg == WM_COMMAND)
        {
            //响应按钮  wParam 为 按钮id
        }
            
//#define END_MSG_MAP() 
			break;
        
        case subMsgMapID:
            if (uMsg == WM_SIZE)
            {
                //if (!wParam)
                //{
                //}
                RECT* p = (RECT*)(lParam);

            }
            
            break;
        default: 
			ATLTRACE(static_cast<int>(ATL::atlTraceWindowing), 0, _T("Invalid message map ID (%i)\n"), dwMsgMapID); \
			ATLASSERT(FALSE); 
			break; 
		} 
    return 0;

}




