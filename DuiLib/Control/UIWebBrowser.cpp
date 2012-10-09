#include "StdAfx.h"
#include "UIWebBrowser.h"
#include <atlconv.h>

DuiLib::CWebBrowserUI::CWebBrowserUI()
	: m_pWebBrowser2(NULL)
	, m_pWebBrowserEventHandler(NULL)
	, m_bAutoNavi(false)
{
	m_clsid=CLSID_WebBrowser;
	m_sHomePage.Empty();
}

bool DuiLib::CWebBrowserUI::DoCreateControl()
{
	if (!CActiveXUI::DoCreateControl())
		return false;
	m_pManager->AddTranslateAccelerator(this);
	SetDispatchHandler(this);
	SetExternalUIHandler(this);
	SetDownloadManager(this);
	GetControl(IID_IWebBrowser2,(LPVOID*)&m_pWebBrowser2);
	if ( m_bAutoNavi && !m_sHomePage.IsEmpty())
	{
		this->Navigate2(m_sHomePage);
	}
	return true;
}

void DuiLib::CWebBrowserUI::ReleaseControl()
{
	m_bCreated=false;
	m_pManager->RemoveTranslateAccelerator(this);
	CActiveXUI::ReleaseControl();
}

DuiLib::CWebBrowserUI::~CWebBrowserUI()
{

}

STDMETHODIMP DuiLib::CWebBrowserUI::GetTypeInfoCount( UINT *iTInfo )
{
	*iTInfo = 0;
	return S_OK;
}

STDMETHODIMP DuiLib::CWebBrowserUI::GetTypeInfo( UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo )
{
return E_NOTIMPL;
}

STDMETHODIMP DuiLib::CWebBrowserUI::GetIDsOfNames( REFIID riid, OLECHAR **rgszNames, UINT cNames, LCID lcid,DISPID *rgDispId )
{
return E_NOTIMPL;
}

STDMETHODIMP DuiLib::CWebBrowserUI::Invoke( DISPID dispIdMember, REFIID riid, LCID lcid,WORD wFlags, DISPPARAMS* pDispParams,VARIANT* pVarResult, EXCEPINFO* pExcepInfo,UINT* puArgErr )
{
	if ((riid != IID_NULL))
		return E_INVALIDARG;

	switch(dispIdMember)
	{
	case DISPID_BEFORENAVIGATE2:
		BeforeNavigate2(
			pDispParams->rgvarg[6].pdispVal,
			pDispParams->rgvarg[5].pvarVal,
			pDispParams->rgvarg[4].pvarVal,
			pDispParams->rgvarg[3].pvarVal,
			pDispParams->rgvarg[2].pvarVal,
			pDispParams->rgvarg[1].pvarVal,
			pDispParams->rgvarg[0].pboolVal);
		break;
	case DISPID_COMMANDSTATECHANGE:
			CommandStateChange(
				pDispParams->rgvarg[1].lVal,
				pDispParams->rgvarg[0].boolVal);
		break;
	case DISPID_NAVIGATECOMPLETE2:
		NavigateComplete2(
			pDispParams->rgvarg[1].pdispVal,
			pDispParams->rgvarg[0].pvarVal);
		break;
	case DISPID_NAVIGATEERROR:
		NavigateError(
			pDispParams->rgvarg[4].pdispVal,
			pDispParams->rgvarg[3].pvarVal,
			pDispParams->rgvarg[2].pvarVal,
			pDispParams->rgvarg[1].pvarVal,
			pDispParams->rgvarg[0].pboolVal);
		break;
	case DISPID_STATUSTEXTCHANGE:
		break;
//  	case DISPID_NEWWINDOW2:
//  		break;
	case DISPID_NEWWINDOW3:
		NewWindow3(
			pDispParams->rgvarg[4].ppdispVal,
			pDispParams->rgvarg[3].pboolVal,
			pDispParams->rgvarg[2].uintVal,
			pDispParams->rgvarg[1].bstrVal,
			pDispParams->rgvarg[0].bstrVal);
		break;
// 	case DISPID_PROPERTYCHANGE:
// 		if (pDispParams->cArgs>0 && pDispParams->rgvarg[0].vt == VT_BSTR) {
// 			TRACE(_T("PropertyChange(%s)\n"), pDispParams->rgvarg[0].bstrVal);
// 		}
// 		break;
	default:
		return E_NOTIMPL;
	}
	return S_OK;
}

STDMETHODIMP DuiLib::CWebBrowserUI::QueryInterface( REFIID riid, LPVOID *ppvObject )
{
	return S_OK;
}

STDMETHODIMP_(ULONG) DuiLib::CWebBrowserUI::AddRef()
{
	InterlockedIncrement(&m_dwRef); 
	return m_dwRef;
}

STDMETHODIMP_(ULONG) DuiLib::CWebBrowserUI::Release()
{
	ULONG ulRefCount = InterlockedDecrement(&m_dwRef);
	return ulRefCount; 
}

void DuiLib::CWebBrowserUI::Navigate2( LPCTSTR lpszUrl )
{
	if (lpszUrl == NULL)
		return;

	if (m_pWebBrowser2)
	{
		VARIANT v;
		v.vt=VT_BSTR;
		v.bstrVal=T2BSTR(lpszUrl);
		HRESULT hr = m_pWebBrowser2->Navigate2(&v, NULL, NULL, NULL, NULL);
	}
}

void DuiLib::CWebBrowserUI::Refresh()
{
	if (m_pWebBrowser2)
	{
		m_pWebBrowser2->Refresh();
	}
}
/// DWebBrowserEvents2
void DuiLib::CWebBrowserUI::BeforeNavigate2( IDispatch *pDisp,VARIANT *&url,VARIANT *&Flags,VARIANT *&TargetFrameName,VARIANT *&PostData,VARIANT *&Headers,VARIANT_BOOL *&Cancel )
{
	if (m_pWebBrowserEventHandler)
	{
		m_pWebBrowserEventHandler->BeforeNavigate2(pDisp,url,Flags,TargetFrameName,PostData,Headers,Cancel);
	}
}

void DuiLib::CWebBrowserUI::NavigateError( IDispatch *pDisp,VARIANT * &url,VARIANT *&TargetFrameName,VARIANT *&StatusCode,VARIANT_BOOL *&Cancel )
{
	if (m_pWebBrowserEventHandler)
	{
		m_pWebBrowserEventHandler->NavigateError(pDisp,url,TargetFrameName,StatusCode,Cancel);
	}
}

void DuiLib::CWebBrowserUI::NavigateComplete2( IDispatch *pDisp,VARIANT *&url )
{
	if (m_pWebBrowserEventHandler)
	{
		m_pWebBrowserEventHandler->NavigateComplete2(pDisp,url);
	}
}

void DuiLib::CWebBrowserUI::ProgressChange( LONG nProgress, LONG nProgressMax )
{
	if (m_pWebBrowserEventHandler)
	{
		m_pWebBrowserEventHandler->ProgressChange(nProgress,nProgressMax);
	}
}

void DuiLib::CWebBrowserUI::NewWindow3( IDispatch **pDisp, VARIANT_BOOL *&Cancel, DWORD dwFlags, BSTR bstrUrlContext, BSTR bstrUrl )
{
	if (m_pWebBrowserEventHandler)
	{
		m_pWebBrowserEventHandler->NewWindow3(pDisp,Cancel,dwFlags,bstrUrlContext,bstrUrl);
	}
}
void DuiLib::CWebBrowserUI::CommandStateChange(long Command,VARIANT_BOOL Enable)
{
	if (m_pWebBrowserEventHandler)
	{
		m_pWebBrowserEventHandler->CommandStateChange(Command,Enable);
	}
}

// IDownloadManager
STDMETHODIMP DuiLib::CWebBrowserUI::Download( /* [in] */ IMoniker *pmk, /* [in] */ IBindCtx *pbc, /* [in] */ DWORD dwBindVerb, /* [in] */ LONG grfBINDF, /* [in] */ BINDINFO *pBindInfo, /* [in] */ LPCOLESTR pszHeaders, /* [in] */ LPCOLESTR pszRedir, /* [in] */ UINT uiCP )
{
	if (m_pWebBrowserEventHandler)
	{
		return m_pWebBrowserEventHandler->Download(pmk,pbc,dwBindVerb,grfBINDF,pBindInfo,pszHeaders,pszRedir,uiCP);
	}
	return S_OK;
}

// IDocHostUIHandler
STDMETHODIMP DuiLib::CWebBrowserUI::ShowContextMenu( DWORD dwID, POINT* pptPosition, IUnknown* pCommandTarget, IDispatch* pDispatchObjectHit )
{
	if (m_pWebBrowserEventHandler)
	{
		return m_pWebBrowserEventHandler->ShowContextMenu(dwID,pptPosition,pCommandTarget,pDispatchObjectHit);
	}
	return S_OK;
}

STDMETHODIMP DuiLib::CWebBrowserUI::GetHostInfo( DOCHOSTUIINFO* pInfo )
{
	if (m_pWebBrowserEventHandler)
	{
		return m_pWebBrowserEventHandler->GetHostInfo(pInfo);
	}
	return S_OK;
}

STDMETHODIMP DuiLib::CWebBrowserUI::ShowUI( DWORD dwID, IOleInPlaceActiveObject* pActiveObject, IOleCommandTarget* pCommandTarget, IOleInPlaceFrame* pFrame, IOleInPlaceUIWindow* pDoc )
{
	if (m_pWebBrowserEventHandler)
	{
		return m_pWebBrowserEventHandler->ShowUI(dwID,pActiveObject,pCommandTarget,pFrame,pDoc);
	}
	return S_OK;
}

STDMETHODIMP DuiLib::CWebBrowserUI::HideUI()
{
	if (m_pWebBrowserEventHandler)
	{
		return m_pWebBrowserEventHandler->HideUI();
	}
	return S_OK;
}

STDMETHODIMP DuiLib::CWebBrowserUI::UpdateUI()
{
	if (m_pWebBrowserEventHandler)
	{
		return m_pWebBrowserEventHandler->UpdateUI();
	}
	return S_OK;
}

STDMETHODIMP DuiLib::CWebBrowserUI::EnableModeless( BOOL fEnable )
{
	if (m_pWebBrowserEventHandler)
	{
		return m_pWebBrowserEventHandler->EnableModeless(fEnable);
	}
	return S_OK;
}

STDMETHODIMP DuiLib::CWebBrowserUI::OnDocWindowActivate( BOOL fActivate )
{
	if (m_pWebBrowserEventHandler)
	{
		return m_pWebBrowserEventHandler->OnDocWindowActivate(fActivate);
	}
	return S_OK;
}

STDMETHODIMP DuiLib::CWebBrowserUI::OnFrameWindowActivate( BOOL fActivate )
{
	if (m_pWebBrowserEventHandler)
	{
		return m_pWebBrowserEventHandler->OnFrameWindowActivate(fActivate);
	}
	return S_OK;
}

STDMETHODIMP DuiLib::CWebBrowserUI::ResizeBorder( LPCRECT prcBorder, IOleInPlaceUIWindow* pUIWindow, BOOL fFrameWindow )
{
	if (m_pWebBrowserEventHandler)
	{
		return m_pWebBrowserEventHandler->ResizeBorder(prcBorder,pUIWindow,fFrameWindow);
	}
	return S_OK;
}

STDMETHODIMP DuiLib::CWebBrowserUI::TranslateAccelerator( LPMSG lpMsg, const GUID* pguidCmdGroup, DWORD nCmdID )
{
	if (m_pWebBrowserEventHandler)
	{
		return m_pWebBrowserEventHandler->TranslateAccelerator(lpMsg,pguidCmdGroup,nCmdID);
	}
	return S_OK;
}

LRESULT DuiLib::CWebBrowserUI::TranslateAccelerator( MSG *pMsg )
{
	if( m_pWebBrowser2 != NULL )
	{
		IOleInPlaceActiveObject *pObj;
		if (FAILED(m_pWebBrowser2->QueryInterface(IID_IOleInPlaceActiveObject, (LPVOID *)&pObj)))
			return S_FALSE;
		return pObj->TranslateAccelerator(pMsg);
	}
	return E_NOTIMPL;
}

STDMETHODIMP DuiLib::CWebBrowserUI::GetOptionKeyPath( LPOLESTR* pchKey, DWORD dwReserved )
{
	if (m_pWebBrowserEventHandler)
	{
		return m_pWebBrowserEventHandler->GetOptionKeyPath(pchKey,dwReserved);
	}
	return S_OK;
}

STDMETHODIMP DuiLib::CWebBrowserUI::GetDropTarget( IDropTarget* pDropTarget, IDropTarget** ppDropTarget )
{
	if (m_pWebBrowserEventHandler)
	{
		return m_pWebBrowserEventHandler->GetDropTarget(pDropTarget,ppDropTarget);
	}
	return S_OK;
}

STDMETHODIMP DuiLib::CWebBrowserUI::GetExternal( IDispatch** ppDispatch )
{
	if (m_pWebBrowserEventHandler)
	{
		return m_pWebBrowserEventHandler->GetExternal(ppDispatch);
	}
	return S_OK;
}

STDMETHODIMP DuiLib::CWebBrowserUI::TranslateUrl( DWORD dwTranslate, OLECHAR* pchURLIn, OLECHAR** ppchURLOut )
{
	if (m_pWebBrowserEventHandler)
	{
		return m_pWebBrowserEventHandler->TranslateUrl(dwTranslate,pchURLIn,ppchURLOut);
	}
	return S_OK;
}

STDMETHODIMP DuiLib::CWebBrowserUI::FilterDataObject( IDataObject* pDO, IDataObject** ppDORet )
{
	if (m_pWebBrowserEventHandler)
	{
		return m_pWebBrowserEventHandler->FilterDataObject(pDO,ppDORet);
	}
	return S_OK;
}

void DuiLib::CWebBrowserUI::SetWebBrowserEventHandler( CWebBrowserEventHandler* pEventHandler )
{
	if ( pEventHandler!=NULL && m_pWebBrowserEventHandler!=pEventHandler)
		m_pWebBrowserEventHandler=pEventHandler;
}

void DuiLib::CWebBrowserUI::Refresh2( int Level )
{
	VARIANT vLevel;
	VariantInit(&vLevel);
	vLevel.vt=VT_I4;
	vLevel.intVal=Level;
	m_pWebBrowser2->Refresh2(&vLevel);
	VariantClear(&vLevel);
}

void DuiLib::CWebBrowserUI::SetAttribute( LPCTSTR pstrName, LPCTSTR pstrValue )
{
	if (_tcscmp(pstrName, _T("homepage")) == 0)
	{
		m_sHomePage = pstrValue;
	}
	else if (_tcscmp(pstrName, _T("autonavi"))==0)
	{
		m_bAutoNavi = (_tcscmp(pstrValue, _T("true")) == 0);
	}
	else
		CActiveXUI::SetAttribute(pstrName, pstrValue);
}

void DuiLib::CWebBrowserUI::NavigateHomePage()
{
	if (!m_sHomePage.IsEmpty())
		this->NavigateUrl(m_sHomePage);
}

void DuiLib::CWebBrowserUI::NavigateUrl( LPCTSTR lpszUrl )
{
	if (m_pWebBrowser2 && lpszUrl)
	{
			m_pWebBrowser2->Navigate((BSTR)SysAllocString(T2BSTR(lpszUrl)),NULL,NULL,NULL,NULL);
	}
}

LPCTSTR DuiLib::CWebBrowserUI::GetClass() const
{
	return _T("WebBrowserUI");
}

LPVOID DuiLib::CWebBrowserUI::GetInterface( LPCTSTR pstrName )
{
	if( _tcscmp(pstrName, _T("WebBrowser")) == 0 ) return static_cast<CWebBrowserUI*>(this);
	return CActiveXUI::GetInterface(pstrName);
}

void DuiLib::CWebBrowserUI::SetHomePage( LPCTSTR lpszUrl )
{
	m_sHomePage.Format(_T("%s"),lpszUrl);
}

LPCTSTR DuiLib::CWebBrowserUI::GetHomePage()
{
	return m_sHomePage;
}

void DuiLib::CWebBrowserUI::SetAutoNavigation( bool bAuto /*= TRUE*/ )
{
	if (m_bAutoNavi==bAuto)	return;

	m_bAutoNavi=bAuto;
}

bool DuiLib::CWebBrowserUI::IsAutoNavigation()
{
	return m_bAutoNavi;
}
