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
	CContainedWindow m_cc;  //һ��ֻ�ܰ�һ���ӿؼ�
	CContainedWindow m_cc1;
};



// TODO: ʵ�ִ�����CMainWindow
class CMainWindow : public CWindowImpl<CMainWindow> { // �����ڣ�����CWindowImplģ��
public:
    //BEGIN_MSG_MAP(CMainWindow) // ���ú�ʵ��ProcessWindowMessage���������Էַ���Ϣ
    //    //COMMAND_ID_HANDLER(IDM_ABOUT, OnAbout) // if����ַ���֧
    //    //COMMAND_ID_HANDLER(IDM_EXIT, OnExit) // if����ַ���֧
    //    MESSAGE_HANDLER(WM_PAINT, OnPaint) // if��Ϣ�ַ���֧
    //    MESSAGE_HANDLER(WM_DESTROY, OnDestroy) // if��Ϣ�ַ���֧
    //END_MSG_MAP()

    BOOL ProcessWindowMessage(
        _In_ HWND hWnd,
        _In_ UINT uMsg,
        _In_ WPARAM wParam,
        _In_ LPARAM lParam,
        _Inout_ LRESULT& lResult,
        _In_ DWORD dwMsgMapID) ;
    LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) ; // ATL��Ϣ�������ı�׼��ʽ
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) ;
    LRESULT OnAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) ; // ATL��������ı�׼��ʽ
    LRESULT OnExit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) ;

};


