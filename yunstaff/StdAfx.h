
#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#pragma once

#define WIN32_LEAN_AND_MEAN	
#define _CRT_SECURE_NO_DEPRECATE

#include <windows.h>
#include <objbase.h>
#include <zmouse.h>

#include "..\DuiLib\UIlib.h"
#include "talk/xmpp/constants.h"
using namespace DuiLib;
using namespace buzz;
#include "ControlEx.h"
#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "..\\lib\\DuiLib_ud.lib")
#   else
#       pragma comment(lib, "..\\lib\\DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "..\\lib\\DuiLib_u.lib")
#   else
#       pragma comment(lib, "..\\lib\\DuiLib.lib")
#   endif
#endif
#pragma comment(lib, "expat.lib")
#pragma comment(lib, "libjingle.lib")
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"Secur32.lib")
#pragma comment(lib,"Crypt32.lib")
#pragma comment(lib,"Iphlpapi.lib")
#pragma comment(lib,"Winmm.lib")
#pragma comment(lib,"Strmiids.lib")
#define WM_ICONMESSAGE (WM_USER+100)

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
