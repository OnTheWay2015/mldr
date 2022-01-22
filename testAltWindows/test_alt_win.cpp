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
    //��Ϣ�����鲻�������Դ���������Ϣ���� dwMsgMapID = 0������ǰ��Щ��Ϣ��Ӧ��Ա�������ڵĴ��ڶ��󣩣�
    //Ҳ���Դ����Ӵ�����Ϣ�� dwMsgMapID ��0����ʱʵ����ͨ�� ALT_MSG_MAP ����ת�ƣ�

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

        //MESSAGE_HANDLER(WM_PAINT, OnPaint) // if��Ϣ�ַ���֧

			if (uMsg == WM_CREATE)
			{
				//auto hwndButtonCintinue = ::CreateWindow(TEXT("button"),//����Ϊ��button    
				//	TEXT("��ť"),//��ť����ʾ���ַ�    
				//	WS_CHILD | WS_VISIBLE,
				//	100, 240, 100, 50,  //��ť�ڽ����ϳ��ֵ�λ��
				//	hWnd, (HMENU)123,  //���ð�ťIDIDC_BUTTON_CONTINUE = 131�Լ�����ID
				//	NULL, NULL);

                 
				//_subWin = new CMainWindowSub(_T("edit"),this,subMsgMapID);
				//_subWin = new CMainWindowSub(_T("button"),this,subMsgMapID);
				_subWin = new CMainWindowB();

                //auto h = _subWin->Create(hWnd, CWindow::rcDefault, _T("sub Window"), WS_OVERLAPPEDWINDOW, WS_EX_CLIENTEDGE, nullptr); //��������
                auto h = _subWin->Create(hWnd, CWindow::rcDefault, _T("sub Window"), WS_CHILDWINDOW, NULL, nullptr); //�Ӵ���
                //ShowWindow(h, SW_SHOW);//һ��Ҫ�и����ô�����ʾ
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
            //��Ӧ��ť  wParam Ϊ ��ťid
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




    LRESULT CMainWindow::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) { // ATL��Ϣ�������ı�׼��ʽ
        //PAINTSTRUCT ps;
        //this->BeginPaint(&ps); // ��ʼ��ͼ
        //// ��������л�ͼ����
        //this->EndPaint(&ps); // ������ͼ
        // bHandled������ֶ���ֵFALSE�Ļ���Ĭ��ΪTRUE
        std::cout << "xxxx1" << std::endl;

        PAINTSTRUCT pt;
        HDC hdc;
        auto hwnd = this->m_hWnd;
        hdc = ::BeginPaint(hwnd, &pt);
        TextOut(hdc, 200, 200, TEXT("3322"), 2);



        wchar_t buf[512];
        auto len = ::GetWindowText(hwnd,buf,512);//len ʵ�ʻ�õ��ַ�����.  hwnd Ҳ�������ı��ؼ�

        TextOut(hdc, 200, 300, buf, len);
        ::EndPaint(hwnd, &pt);
        return 0;
    }
    LRESULT CMainWindow::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
        //PostQuitMessage(0); // �˳���Ϣѭ��
        std::cout << "xxxx2" << std::endl;
        return 0;
    }
    LRESULT CMainWindow::OnAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) { // ATL��������ı�׼��ʽ
        //CSimpleDialog<IDD_ABOUTBOX> dlg;
        //dlg.DoModal(); // ��ʾ�����ڡ��Ի���
        return 0;
    }

    LRESULT CMainWindow::OnExit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
        this->DestroyWindow(); // ����ļ�->�ر�ʱ�����ٴ���
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

        //MESSAGE_HANDLER(WM_PAINT, OnPaint) // if��Ϣ�ַ���֧

			if (uMsg == WM_CREATE)
			{

				//_subWin = new CMainWindowSub(_T("edit"),this,subMsgMapID);

                RECT rc= { 100, 100, 100, 100};
                auto h = m_cc.Create(_T("edit"),this,subMsgMapID,hWnd,rc);
                //auto h =  m_cc.Create(_T("button"),this,subMsgMapID,hWnd,&CWindow::rcDefault);
                //auto h =  m_cc.Create(_T("button"),this,subMsgMapID,hWnd,rc);
                m_cc.SetWindowTextW(_T("mark text"));
                ::SetWindowPos(h,NULL,50,50,100,100, SWP_SHOWWINDOW);//һ��Ҫ�и�����,���ô�С,������ʾ

                //::ShowWindow(h, SW_SHOW);
                //if (h)
                //{
                //    ::SetWindowPos(h,NULL,0,0,200,100, SWP_SHOWWINDOW);
                //}
			}
        if (uMsg == WM_COMMAND)
        {
            //��Ӧ��ť  wParam Ϊ ��ťid
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




