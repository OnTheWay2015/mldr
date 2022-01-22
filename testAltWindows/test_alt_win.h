#pragma once

#include <atlbase.h>
#include <atlwin.h>


class CMainWindowB : public CWindowImpl<CMainWindowB> 
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
	CContainedWindow m_cc;  //一个只能绑定一个子控件
	CContainedWindow m_cc1;
};



// TODO: 实现窗口类CMainWindow
class CMainWindow : public CWindowImpl<CMainWindow> { // 主窗口，基于CWindowImpl模板
public:
    //BEGIN_MSG_MAP(CMainWindow) // 利用宏实现ProcessWindowMessage函数，用以分发消息
    //    //COMMAND_ID_HANDLER(IDM_ABOUT, OnAbout) // if命令分发分支
    //    //COMMAND_ID_HANDLER(IDM_EXIT, OnExit) // if命令分发分支
    //    MESSAGE_HANDLER(WM_PAINT, OnPaint) // if消息分发分支
    //    MESSAGE_HANDLER(WM_DESTROY, OnDestroy) // if消息分发分支
    //END_MSG_MAP()

    BOOL ProcessWindowMessage(
        _In_ HWND hWnd,
        _In_ UINT uMsg,
        _In_ WPARAM wParam,
        _In_ LPARAM lParam,
        _Inout_ LRESULT& lResult,
        _In_ DWORD dwMsgMapID) ;
    LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) ; // ATL消息处理函数的标准形式
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) ;
    LRESULT OnAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) ; // ATL命令处理函数的标准形式
    LRESULT OnExit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) ;

};


