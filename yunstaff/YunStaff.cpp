#include "stdafx.h"
#include <exdisp.h>
#include <comdef.h>
#include "resource.h"
#include <iostream>
#include "talk/base/thread.h"
#include "talk/xmpp/constants.h"
#include "talk/xmpp/xmppclientsettings.h"
#include "talk/xmpp/xmppengine.h"
#include "MainFrameWnd.h"




int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
	std::cout << CPaintManagerUI::GetInstancePath();
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin/yunstaff/"));
	//CPaintManagerUI::SetResourceZip(_T("YunStaffRes.zip"));

	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) return 0;

	MainFrameWnd* mainFrame = new MainFrameWnd(SW_SHOW);
	if( mainFrame == NULL ) return 0;

	CPaintManagerUI::MessageLoop();

	::CoUninitialize();
	return 0;
}