#include "stdafx.h"
#include <exdisp.h>
#include <comdef.h>
#include <ShellAPI.h>
#include "ControlEx.h"
#include "resource.h"
#include <iostream>
#include "talk/base/thread.h"
#include "xmppthread.h"
#include "talk/xmpp/constants.h"
#include "talk/xmpp/xmppclientsettings.h"
#include "talk/xmpp/xmppengine.h"

NOTIFYICONDATA m_tnd;
class CYunStaffFrameWnd : public CWindowWnd, public INotifyUI
{
public:
	CYunStaffFrameWnd() { };
	LPCTSTR GetWindowClassName() const { return _T("UIMainFrame"); };
	UINT GetClassStyle() const { return CS_DBLCLKS; };
	void OnFinalMessage(HWND /*hWnd*/) { delete this; };

	void Init() {
		m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));
		m_pMaxBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("maxbtn")));
		m_pRestoreBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("restorebtn")));
		m_pMinBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("minbtn")));
		m_pLoginBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("login")));
		m_pRemember = static_cast<CCheckBoxUI*>(m_pm.FindControl(_T("remenber")));
		m_pAutoLogin = static_cast<CCheckBoxUI*>(m_pm.FindControl(_T("autologin")));
		m_pUsername = static_cast<CEditUI*>(m_pm.FindControl(_T("username")));
		m_pPassword = static_cast<CEditUI*>(m_pm.FindControl(_T("password")));
	}

	void OnPrepare() {
	}

	void Notify(TNotifyUI& msg)
	{
		if( msg.sType == _T("windowinit") ) OnPrepare();
		else if( msg.sType == _T("click") ) {
			if( msg.pSender == m_pCloseBtn ) {
				SendMessage(WM_DESTROY, SC_MINIMIZE, 0); 
				return; 
			}
			else if( msg.pSender == m_pMinBtn ) { 
				::ShowWindow(*this, SW_HIDE);
				//SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); 
				return;
			}
			else if( msg.pSender == m_pMaxBtn ) { 
				SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); return; }
			else if( msg.pSender == m_pRestoreBtn ) { 
				SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); return; }
			else if (msg.pSender == m_pLoginBtn){
				XmppThread thread;
				thread.Start();

				talk_base::InsecureCryptStringImpl pass;
				LPCTSTR password = m_pPassword->GetText().GetData();
				pass.password() = password;
				buzz::XmppClientSettings xcs;
				LPCTSTR username = m_pUsername->GetText().GetData();
				xcs.set_user(username);
				xcs.set_host("example.com");
				xcs.set_use_tls(buzz::TLS_DISABLED);
				xcs.set_pass(talk_base::CryptString(pass));
				//xcs.set_auth_token(buzz::AUTH_MECHANISM_OAUTH2,
				//                   auth_token.c_str());
				xcs.set_server(talk_base::SocketAddress("example.com", 5222));
				thread.Login(xcs);
				MessageBox(*this,m_pUsername->GetText(),"sdfd",1);
			}
		}
		else if(msg.sType== _T("selectchanged"))
		{
			CDuiString name = msg.pSender->GetName();
			CCheckBoxUI* pControl = static_cast<CCheckBoxUI*>(m_pm.FindControl(name));
			if(name==_T("remenber") && !pControl->IsSelected()) {
				CCheckBoxUI* pAutoLogin = static_cast<CCheckBoxUI*>(m_pm.FindControl(_T("autologin")));
				pAutoLogin->Selected(FALSE);
			}else if(name==_T("autologin") && pControl->IsSelected()){
				CCheckBoxUI* pRemenber = static_cast<CCheckBoxUI*>(m_pm.FindControl(_T("remenber")));
				pRemenber->Selected(TRUE);
			}
		}
	}

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
		styleValue &= ~WS_CAPTION;
		::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

		m_pm.Init(m_hWnd);
		CDialogBuilder builder;
		CDialogBuilderCallbackEx cb;
		CControlUI* pRoot = builder.Create(_T("skin.xml"), (UINT)0,  &cb, &m_pm);
		ASSERT(pRoot && "Failed to parse XML");
		m_pm.AttachDialog(pRoot);
		m_pm.AddNotifier(this);

		Init();
		
		m_tnd.cbSize=sizeof(NOTIFYICONDATA);
		m_tnd.hWnd=*this;
		//tnd.uID=IDR_MAINFRAME;
		m_tnd.uFlags=NIF_MESSAGE|NIF_ICON|NIF_TIP;
		m_tnd.uCallbackMessage=WM_ICONMESSAGE;
		m_tnd.hIcon=LoadIcon((HINSTANCE) GetWindowLongPtr(*this, GWLP_HINSTANCE), MAKEINTRESOURCE(IDI_ICON3));
		strcpy(m_tnd.szTip,"云办公系统");//图标提示为"测试程序"
		Shell_NotifyIcon(NIM_ADD,&m_tnd);//向任务栏添加图标 
		return 0;
	}

	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{	
		Shell_NotifyIcon(NIM_DELETE,&m_tnd);
		::PostQuitMessage(0L);
		bHandled = FALSE;		
		return 0;
	}

	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
        if( ::IsIconic(*this) ) bHandled = FALSE;
        return (wParam == 0) ? TRUE : FALSE;
	}

	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 0;
	}

	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 0;
	}

	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
		::ScreenToClient(*this, &pt);

		RECT rcClient;
		::GetClientRect(*this, &rcClient);

 		if( !::IsZoomed(*this) ) {
 			RECT rcSizeBox = m_pm.GetSizeBox();
 			if( pt.y < rcClient.top + rcSizeBox.top ) {
 				if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
 				if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
 				return HTTOP;
 			}
 			else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) {
 				if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
 				if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
 				return HTBOTTOM;
 			}
 			if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
 			if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
 		}

		RECT rcCaption = m_pm.GetCaptionRect();
		if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
			&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {
				CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
				if( pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 && 
					_tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
					_tcscmp(pControl->GetClass(), _T("TextUI")) != 0 )
					return HTCAPTION;
		}

		return HTCLIENT;
	}

	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
        SIZE szRoundCorner = m_pm.GetRoundCorner();
        if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) {
            CDuiRect rcWnd;
            ::GetWindowRect(*this, &rcWnd);
            rcWnd.Offset(-rcWnd.left, -rcWnd.top);
            rcWnd.right++; rcWnd.bottom++;
            HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
            ::SetWindowRgn(*this, hRgn, TRUE);
            ::DeleteObject(hRgn);
        }

        bHandled = FALSE;
        return 0;
	}

	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		MONITORINFO oMonitor = {};
		oMonitor.cbSize = sizeof(oMonitor);
		::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
		CDuiRect rcWork = oMonitor.rcWork;
		rcWork.Offset(-rcWork.left, -rcWork.top);

		LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
		lpMMI->ptMaxPosition.x	= rcWork.left;
		lpMMI->ptMaxPosition.y	= rcWork.top;
		lpMMI->ptMaxSize.x		= rcWork.right;
		lpMMI->ptMaxSize.y		= rcWork.bottom;

		bHandled = FALSE;
		return 0;
	}

	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
		if( wParam == SC_CLOSE ) {
			Shell_NotifyIcon(NIM_DELETE,&m_tnd);
			::PostQuitMessage(0L);
			bHandled = TRUE;
			return 0;
		}
		BOOL bZoomed = ::IsZoomed(*this);
		LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
		if( ::IsZoomed(*this) != bZoomed ) {
			if( !bZoomed ) {
				CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
				if( pControl ) pControl->SetVisible(false);
				pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
				if( pControl ) pControl->SetVisible(true);
			}
			else {
				CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
				if( pControl ) pControl->SetVisible(true);
				pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
				if( pControl ) pControl->SetVisible(false);
			}
		}
		return lRes;
	}

	LRESULT onIconMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if(lParam==WM_LBUTTONUP)
        {
			::ShowWindow(*this, SW_SHOW);
            // AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOW);
        }
		return TRUE;
	}
	

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;
		BOOL bHandled = TRUE;
		switch( uMsg ) {
		case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
		case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
		case WM_DESTROY:       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
		case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
		case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
		case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
		case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
		case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
		case WM_GETMINMAXINFO: lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
		case WM_SYSCOMMAND:    lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
		case WM_ICONMESSAGE:   lRes = onIconMessage(uMsg, wParam, lParam, bHandled); break;

            
		default:
		bHandled = FALSE;
		}
		if( bHandled ) return lRes;
		if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	}

public:
	CPaintManagerUI m_pm;

private:
	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRestoreBtn;
	CButtonUI* m_pMinBtn;	
	CButtonUI* m_pLoginBtn;
	CCheckBoxUI* m_pRemember;
	CCheckBoxUI* m_pAutoLogin;
	CEditUI* m_pUsername;
	CEditUI* m_pPassword;
	//...
};


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
	std::cout << CPaintManagerUI::GetInstancePath();
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin/yunstaff/"));
	//CPaintManagerUI::SetResourceZip(_T("YunStaffRes.zip"));

	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) return 0;

	CYunStaffFrameWnd* pFrame = new CYunStaffFrameWnd();
	if( pFrame == NULL ) return 0;
	pFrame->Create(NULL, _T("云达办公系统"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 800, 572);
	pFrame->CenterWindow();
	::ShowWindow(*pFrame, SW_SHOW);
	//::SendMessage(*pFrame, WM_SETICON, TRUE, (LPARAM)::LoadIcon((HMODULE)&__ImageBase, MAKEINTRESOURCE(IDI_BIG)));
	//SetIcon(::LoadIcon((HMODULE)&__ImageBase, MAKEINTRESOURCE(IDI_SMALL)), FALSE);

	SendMessage(*pFrame, WM_SETICON, FALSE,  (LPARAM) LoadIcon((HINSTANCE) GetWindowLongPtr(*pFrame, GWLP_HINSTANCE), MAKEINTRESOURCE(IDI_ICON2)));
    SendMessage(*pFrame, WM_SETICON, TRUE, (LPARAM) LoadIcon((HINSTANCE) GetWindowLongPtr(*pFrame, GWLP_HINSTANCE),  MAKEINTRESOURCE(IDI_ICON3)));

	CPaintManagerUI::MessageLoop();

	::CoUninitialize();
	return 0;
}