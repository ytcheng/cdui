#include "StdAfx.h"
#include "UITreeView.h"

#pragma warning( disable: 4251 )
namespace DuiLib
{
	//************************************
	// Method:    CTreeNodeUI
	// FullName:  CTreeNodeUI::CTreeNodeUI
	// Access:    public 
	// Returns:   
	// Qualifier:
	// Parameter: CTreeNodeUI * _ParentNode
	// Note:	  
	//************************************
	CTreeNodeUI::CTreeNodeUI( CTreeNodeUI* _ParentNode ) : CListContainerElementUI()
	{
		pTreeView		= NULL;
		m_iTreeLavel	= 0;
		m_bIsVisable	= true;
		m_bIsHasChild	= false;
		m_bIsCheckBox	= false;
		pParentTreeNode	= NULL;

		pHoriz			= new CHorizontalLayoutUI();
		pFolderButton	= new CCheckBoxUI();
		pDottedLine		= new CLabelUI();
		pCheckBox		= new CCheckBoxUI();
		pItemButton		= new COptionUI();

		this->SetFixedHeight(18);
		pFolderButton->SetFixedWidth(GetFixedHeight());
		pDottedLine->SetFixedWidth(2);
		pCheckBox->SetFixedWidth(GetFixedHeight());
		pItemButton->SetAttribute(_T("align"),_T("left"));

		pDottedLine->SetVisible(false);
		pCheckBox->SetVisible(false);
		pItemButton->SetMouseEnabled(false);

		if(_ParentNode)
		{
			if (_tcsicmp(_ParentNode->GetClass(), _T("TreeNodeUI")) != 0)
				return;

			pDottedLine->SetVisible(_ParentNode->IsVisible());
			pDottedLine->SetFixedWidth(_ParentNode->GetDottedLine()->GetFixedWidth()+16);
			this->SetParentNode(_ParentNode);
		}

		pHoriz->Add(pDottedLine);
		pHoriz->Add(pFolderButton);
		pHoriz->Add(pCheckBox);
		pHoriz->Add(pItemButton);
		Add(pHoriz);
	}

	//************************************
	// Method:    ~CTreeNodeUI
	// FullName:  CTreeNodeUI::~CTreeNodeUI
	// Access:    public 
	// Returns:   
	// Qualifier:
	// Parameter: void
	// Note:	  
	//************************************
	CTreeNodeUI::~CTreeNodeUI( void )
	{
	}

	//************************************
	// Method:    GetClass
	// FullName:  CTreeNodeUI::GetClass
	// Access:    public 
	// Returns:   LPCTSTR
	// Qualifier: const
	// Note:	  
	//************************************
	LPCTSTR CTreeNodeUI::GetClass() const
	{
		return _T("TreeNodeUI");
	}

	//************************************
	// Method:    GetInterface
	// FullName:  CTreeNodeUI::GetInterface
	// Access:    public 
	// Returns:   LPVOID
	// Qualifier:
	// Parameter: LPCTSTR pstrName
	// Note:	  
	//************************************
	LPVOID CTreeNodeUI::GetInterface( LPCTSTR pstrName )
	{
		if( _tcscmp(pstrName, _T("TreeNode")) == 0 )
			return static_cast<CTreeNodeUI*>(this);
		return CListContainerElementUI::GetInterface(pstrName);
	}

	//************************************
	// Method:    Invalidate
	// FullName:  CTreeNodeUI::Invalidate
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Note:	  
	//************************************
	void CTreeNodeUI::Invalidate()
	{
		if( !IsVisible() )
			return;

		if( GetParent() ) {
			CContainerUI* pParentContainer = static_cast<CContainerUI*>(GetParent()->GetInterface(_T("Container")));
			if( pParentContainer ) {
				RECT rc = pParentContainer->GetPos();
				RECT rcInset = pParentContainer->GetInset();
				rc.left += rcInset.left;
				rc.top += rcInset.top;
				rc.right -= rcInset.right;
				rc.bottom -= rcInset.bottom;
				CScrollBarUI* pVerticalScrollBar = pParentContainer->GetVerticalScrollBar();
				if( pVerticalScrollBar && pVerticalScrollBar->IsVisible() ) rc.right -= pVerticalScrollBar->GetFixedWidth();
				CScrollBarUI* pHorizontalScrollBar = pParentContainer->GetHorizontalScrollBar();
				if( pHorizontalScrollBar && pHorizontalScrollBar->IsVisible() ) rc.bottom -= pHorizontalScrollBar->GetFixedHeight();

				RECT invalidateRc = m_rcItem;
				if( !::IntersectRect(&invalidateRc, &m_rcItem, &rc) ) 
					return;

				CControlUI* pParent = GetParent();
				RECT rcTemp;
				RECT rcParent;
				while( pParent = pParent->GetParent() )
				{
					rcTemp = invalidateRc;
					rcParent = pParent->GetPos();
					if( !::IntersectRect(&invalidateRc, &rcTemp, &rcParent) ) 
						return;
				}

				if( m_pManager != NULL ) m_pManager->Invalidate(invalidateRc);
			}
			else {
				CContainerUI::Invalidate();
			}
		}
		else {
			CContainerUI::Invalidate();
		}
	}

	//************************************
	// Method:    Add
	// FullName:  CTreeNodeUI::Add
	// Access:    public 
	// Returns:   bool
	// Qualifier:
	// Parameter: CControlUI * _pTreeNodeUI
	// Note:	  
	//************************************
	bool CTreeNodeUI::Add( CControlUI* _pTreeNodeUI )
	{
		if (_tcsicmp(_pTreeNodeUI->GetClass(), _T("TreeNodeUI")) == 0)
			return AddChildNode((CTreeNodeUI*)_pTreeNodeUI);

		return CListContainerElementUI::Add(_pTreeNodeUI);
	}
	//************************************
	// Method:    AddAt
	// FullName:  CTreeNodeUI::AddAt
	// Access:    public 
	// Returns:   bool
	// Qualifier:
	// Parameter: CControlUI * pControl
	// Parameter: int iIndex
	// Note:	  暂时屏蔽，后续完善
	//************************************
	bool CTreeNodeUI::AddAt( CControlUI* pControl, int iIndex )
	{
		return false;
	}
	//************************************
	// Method:    Remove
	// FullName:  CTreeNodeUI::Remove
	// Access:    public 
	// Returns:   bool
	// Qualifier:
	// Parameter: CControlUI * pControl
	// Note:	  
	//************************************
	bool CTreeNodeUI::Remove( CControlUI* pControl )
	{
		return RemoveAt((CTreeNodeUI*)pControl);
	}

	//************************************
	// Method:    PutVisible
	// FullName:  CTreeNodeUI::SetVisibleTag
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: bool _IsVisible
	// Note:	  
	//************************************
	void CTreeNodeUI::SetVisibleTag( bool _IsVisible )
	{
		m_bIsVisable = _IsVisible;
	}
	//************************************
	// Method:    GetVisible
	// FullName:  CTreeNodeUI::GetVisibleTag
	// Access:    public 
	// Returns:   bool
	// Qualifier:
	// Note:	  
	//************************************
	bool CTreeNodeUI::GetVisibleTag()
	{
		return m_bIsVisable;
	}

	//************************************
	// Method:    SetItemText
	// FullName:  CTreeNodeUI::SetItemText
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: LPCTSTR pstrValue
	// Note:	  
	//************************************
	void CTreeNodeUI::SetItemText( LPCTSTR pstrValue )
	{
		pItemButton->SetText(pstrValue);
	}

	//************************************
	// Method:    GetItemText
	// FullName:  CTreeNodeUI::GetItemText
	// Access:    public 
	// Returns:   CDuiString
	// Qualifier:
	// Note:	  
	//************************************
	CDuiString CTreeNodeUI::GetItemText()
	{
		return pItemButton->GetText();
	}

	//************************************
	// Method:    CheckBoxSelected
	// FullName:  CTreeNodeUI::CheckBoxSelected
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: bool _Selected
	// Note:	  
	//************************************
	void CTreeNodeUI::CheckBoxSelected( bool _Selected )
	{
		pCheckBox->Selected(_Selected);
	}

	//************************************
	// Method:    IsCheckBoxSelected
	// FullName:  CTreeNodeUI::IsCheckBoxSelected
	// Access:    public 
	// Returns:   bool
	// Qualifier: const
	// Note:	  
	//************************************
	bool CTreeNodeUI::IsCheckBoxSelected() const
	{
		return pCheckBox->IsSelected();
	}

	//************************************
	// Method:    IsHasChild
	// FullName:  CTreeNodeUI::IsHasChild
	// Access:    public 
	// Returns:   bool
	// Qualifier: const
	// Note:	  
	//************************************
	bool CTreeNodeUI::IsHasChild() const
	{
		return m_bIsHasChild;
	}

	//************************************
	// Method:    GetTreeLevel
	// FullName:  CTreeNodeUI::GetTreeLevel
	// Access:    public 
	// Returns:   long
	// Qualifier: const
	// Note:	  
	//************************************
	long CTreeNodeUI::GetTreeLevel() const
	{
		return m_iTreeLavel;
	}

	//************************************
	// Method:    AddChildNode
	// FullName:  CTreeNodeUI::AddChildNode
	// Access:    public 
	// Returns:   bool
	// Qualifier:
	// Parameter: CTreeNodeUI * _pTreeNodeUI
	// Note:	  
	//************************************
	bool CTreeNodeUI::AddChildNode( CTreeNodeUI* _pTreeNodeUI )
	{
		if (!_pTreeNodeUI)
			return false;

		if (_tcsicmp(_pTreeNodeUI->GetClass(), _T("TreeNodeUI")) != 0)
			return false;

		_pTreeNodeUI->GetDottedLine()->SetVisible(true);
		_pTreeNodeUI->GetDottedLine()->SetFixedWidth(pDottedLine->GetFixedWidth()+16);
		_pTreeNodeUI->SetParentNode(this);
		_pTreeNodeUI->GetItemButton()->SetGroup(pItemButton->GetGroup());
		mTreeNodes.Add(_pTreeNodeUI);

		m_bIsHasChild = true;
		return true;
	}

	//************************************
	// Method:    RemoveAt
	// FullName:  CTreeNodeUI::RemoveAt
	// Access:    public 
	// Returns:   bool
	// Qualifier:
	// Parameter: CTreeNodeUI * _pTreeNodeUI
	// Note:	  
	//************************************
	bool CTreeNodeUI::RemoveAt( CTreeNodeUI* _pTreeNodeUI )
	{
		int nIndex = mTreeNodes.Find(_pTreeNodeUI);
		CTreeNodeUI* pNode = static_cast<CTreeNodeUI*>(mTreeNodes.GetAt(nIndex));
		if(pNode && pNode == _pTreeNodeUI)
		{
			while(pNode->IsHasChild())
				RemoveAt(static_cast<CTreeNodeUI*>(pNode->mTreeNodes.GetAt(0)));
			mTreeNodes.Remove(nIndex);
			return true;
		}
		return false;
	}

	//************************************
	// Method:    SetParentNode
	// FullName:  CTreeNodeUI::SetParentNode
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: CTreeNodeUI * _pParentTreeNode
	// Note:	  
	//************************************
	void CTreeNodeUI::SetParentNode( CTreeNodeUI* _pParentTreeNode )
	{
		pParentTreeNode = _pParentTreeNode;
	}

	//************************************
	// Method:    GetParentNode
	// FullName:  CTreeNodeUI::GetParentNode
	// Access:    public 
	// Returns:   CTreeNodeUI*
	// Qualifier:
	// Note:	  
	//************************************
	CTreeNodeUI* CTreeNodeUI::GetParentNode()
	{
		return pParentTreeNode;
	}

	//************************************
	// Method:    GetCountChild
	// FullName:  CTreeNodeUI::GetCountChild
	// Access:    public 
	// Returns:   long
	// Qualifier:
	// Note:	  
	//************************************
	long CTreeNodeUI::GetCountChild()
	{
		return mTreeNodes.GetSize();
	}

	//************************************
	// Method:    SetTreeView
	// FullName:  CTreeNodeUI::SetTreeView
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: CTreeViewUI * _CTreeViewUI
	// Note:	  
	//************************************
	void CTreeNodeUI::SetTreeView( CTreeViewUI* _CTreeViewUI )
	{
		pTreeView = _CTreeViewUI;
	}
	//************************************
	// Method:    GetTreeView
	// FullName:  CTreeNodeUI::GetTreeView
	// Access:    public 
	// Returns:   CTreeViewUI*
	// Qualifier:
	// Note:	  
	//************************************
	CTreeViewUI* CTreeNodeUI::GetTreeView()
	{
		return pTreeView;
	}

	//************************************
	// Method:    SetAttribute
	// FullName:  CTreeNodeUI::SetAttribute
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: LPCTSTR pstrName
	// Parameter: LPCTSTR pstrValue
	// Note:	  
	//************************************
	void CTreeNodeUI::SetAttribute( LPCTSTR pstrName, LPCTSTR pstrValue )
	{
		if(_tcscmp(pstrName, _T("text")) == 0 )
			pItemButton->SetText(pstrValue);
		else if(_tcscmp(pstrName, _T("horizattr")) == 0 )
			pHoriz->ApplyAttributeList(pstrValue);
		else if(_tcscmp(pstrName, _T("dotlineattr")) == 0 )
			pDottedLine->ApplyAttributeList(pstrValue);
		else if(_tcscmp(pstrName, _T("folderattr")) == 0 )
			pFolderButton->ApplyAttributeList(pstrValue);
		else if(_tcscmp(pstrName, _T("checkboxattr")) == 0 )
			pCheckBox->ApplyAttributeList(pstrValue);
		else if(_tcscmp(pstrName, _T("itemattr")) == 0 )
			pItemButton->ApplyAttributeList(pstrValue);
		else CListContainerElementUI::SetAttribute(pstrName,pstrValue);
	}

	CStdPtrArray CTreeNodeUI::GetTreeNodes()
	{
		return mTreeNodes;
	}

	CTreeNodeUI* CTreeNodeUI::GetChildNode( int _nIndex )
	{
		return static_cast<CTreeNodeUI*>(mTreeNodes.GetAt(_nIndex));
	}

	//************************************
	// Method:    SetVisibleFolderBtn
	// FullName:  CTreeNodeUI::SetVisibleFolderBtn
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: bool _IsVisibled
	// Node:	  
	//************************************
	void CTreeNodeUI::SetVisibleFolderBtn( bool _IsVisibled )
	{
		pFolderButton->SetVisible(_IsVisibled);
	}
	//************************************
	// Method:    GetVisibleFolderBtn
	// FullName:  CTreeNodeUI::GetVisibleFolderBtn
	// Access:    public 
	// Returns:   bool
	// Qualifier:
	// Node:	  
	//************************************
	bool CTreeNodeUI::GetVisibleFolderBtn()
	{
		return pFolderButton->IsVisible();
	}
	//************************************
	// Method:    SetVisibleCheckBtn
	// FullName:  CTreeNodeUI::SetVisibleCheckBtn
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: bool _IsVisibled
	// Node:	  
	//************************************
	void CTreeNodeUI::SetVisibleCheckBtn( bool _IsVisibled )
	{
		pCheckBox->SetVisible(_IsVisibled);
	}
	//************************************
	// Method:    GetVisibleCheckBtn
	// FullName:  CTreeNodeUI::GetVisibleCheckBtn
	// Access:    public 
	// Returns:   bool
	// Qualifier:
	// Node:	  
	//************************************
	bool CTreeNodeUI::GetVisibleCheckBtn()
	{
		return pCheckBox->IsVisible();
	}

	/*****************************************************************************/
	/*****************************************************************************/
	/*****************************************************************************/

	//************************************
	// Method:    CTreeViewUI
	// FullName:  CTreeViewUI::CTreeViewUI
	// Access:    public 
	// Returns:   
	// Qualifier:
	// Parameter: void
	// Note:	  
	//************************************
	CTreeViewUI::CTreeViewUI( void ) : m_bVisibleFolderBtn(true),m_bVisibleCheckBtn(false)
	{
		this->GetHeader()->SetVisible(false);
	}

	//************************************
	// Method:    ~CTreeViewUI
	// FullName:  CTreeViewUI::~CTreeViewUI
	// Access:    public 
	// Returns:   
	// Qualifier:
	// Parameter: void
	// Note:	  
	//************************************
	CTreeViewUI::~CTreeViewUI( void )
	{
	}

	//************************************
	// Method:    GetClass
	// FullName:  UiLib::CTreeViewUI::GetClass
	// Access:    virtual public 
	// Returns:   LPCTSTR
	// Qualifier: const
	//************************************
	LPCTSTR CTreeViewUI::GetClass() const
	{
		return _T("TreeViewUI");
	}

	//************************************
	// Method:    GetInterface
	// FullName:  UiLib::CTreeViewUI::GetInterface
	// Access:    virtual public 
	// Returns:   LPVOID
	// Qualifier:
	// Parameter: LPCTSTR pstrName
	//************************************
	LPVOID CTreeViewUI::GetInterface( LPCTSTR pstrName )
	{
		if( _tcscmp(pstrName, _T("TreeView")) == 0 ) return static_cast<CTreeViewUI*>(this);
		return CListUI::GetInterface(pstrName);
	}

	//************************************
	// Method:    Add
	// FullName:  CTreeViewUI::Add
	// Access:    virtual public 
	// Returns:   bool
	// Qualifier:
	// Parameter: CTreeNodeUI * pControl
	// Note:	  
	//************************************
	bool CTreeViewUI::Add( CTreeNodeUI* pControl )
	{
		if (!pControl)
			return false;

		if (_tcsicmp(pControl->GetClass(), _T("TreeNodeUI")) != 0)
			return false;

		pControl->GetFolderButton()->OnNotify += MakeDelegate(this,&CTreeViewUI::OnFolderChanged);
		pControl->GetCheckBox()->OnNotify += MakeDelegate(this,&CTreeViewUI::OnCheckBoxChanged);

		pControl->SetVisibleFolderBtn(m_bVisibleFolderBtn);
		pControl->SetVisibleCheckBtn(m_bVisibleCheckBtn);

		CListUI::Add(pControl);

		if(pControl->GetCountChild() > 0)
		{
			int nCount = pControl->GetCountChild();
			for(int nIndex = 0;nIndex < nCount;nIndex++)
			{
				CTreeNodeUI* pNode = pControl->GetChildNode(nIndex);
				if(pNode)
					Add(pNode);
			}
		}

		pControl->SetTreeView(this);
		return true;
	}

	//************************************
	// Method:    AddAt
	// FullName:  CTreeViewUI::AddAt
	// Access:    virtual public 
	// Returns:   long
	// Qualifier:
	// Parameter: CTreeNodeUI * pControl
	// Parameter: int iIndex
	// Note:	  
	//************************************
	long CTreeViewUI::AddAt( CTreeNodeUI* pControl, int iIndex )
	{
		if (!pControl)
			return -1;

		if (_tcsicmp(pControl->GetClass(), _T("TreeNodeUI")) != 0)
			return -1;

		CTreeNodeUI* pParent = static_cast<CTreeNodeUI*>(GetItemAt(iIndex));
		if(!pParent)
			return -1;

		CListUI::AddAt(pControl,iIndex);

		pControl->SetVisibleFolderBtn(m_bVisibleFolderBtn);
		pControl->SetVisibleCheckBtn(m_bVisibleCheckBtn);

		if(pControl->GetCountChild() > 0)
		{
			int nCount = pControl->GetCountChild();
			for(int nIndex = 0;nIndex < nCount;nIndex++)
			{
				CTreeNodeUI* pNode = pControl->GetChildNode(nIndex);
				if(pNode)
					return AddAt(pNode,iIndex+1);
			}
		}
		else
			return iIndex+1;

		return -1;
	}

	//************************************
	// Method:    Remove
	// FullName:  CTreeViewUI::Remove
	// Access:    virtual public 
	// Returns:   bool
	// Qualifier:
	// Parameter: CTreeNodeUI * pControl
	// Note:	  
	//************************************
	bool CTreeViewUI::Remove( CTreeNodeUI* pControl )
	{
		if(pControl->GetCountChild() > 0)
		{
			int nCount = pControl->GetCountChild();
			for(int nIndex = 0;nIndex < nCount;nIndex++)
			{
				CTreeNodeUI* pNode = pControl->GetChildNode(nIndex);
				if(pNode)
					Remove(pNode);
			}
		}
		CListUI::Remove(pControl);
		return true;
	}

	//************************************
	// Method:    RemoveAt
	// FullName:  CTreeViewUI::RemoveAt
	// Access:    virtual public 
	// Returns:   bool
	// Qualifier:
	// Parameter: int iIndex
	// Note:	  
	//************************************
	bool CTreeViewUI::RemoveAt( int iIndex )
	{
		CTreeNodeUI* pItem = (CTreeNodeUI*)GetItemAt(iIndex);
		if(pItem->GetCountChild())
			Remove(pItem);
		return true;
	}

	//************************************
	// Method:    RemoveAll
	// FullName:  CTreeViewUI::RemoveAll
	// Access:    virtual public 
	// Returns:   void
	// Qualifier:
	// Note:	  
	//************************************
	void CTreeViewUI::RemoveAll()
	{
		CListUI::RemoveAll();
	}

	//************************************
	// Method:    Notify
	// FullName:  CTreeViewUI::Notify
	// Access:    virtual public 
	// Returns:   void
	// Qualifier:
	// Parameter: TNotifyUI & msg
	// Note:	  
	//************************************
	void CTreeViewUI::Notify( TNotifyUI& msg )
	{
	}

	//************************************
	// Method:    OnChanged
	// FullName:  CTreeViewUI::OnCheckBoxChanged
	// Access:    virtual public 
	// Returns:   bool
	// Qualifier:
	// Parameter: void * param
	// Note:	  
	//************************************
	bool CTreeViewUI::OnCheckBoxChanged( void* param )
	{
		TNotifyUI* pMsg = (TNotifyUI*)param;
		if(pMsg->sType == _T("selectchanged"))
		{
			CCheckBoxUI* pCheckBox = (CCheckBoxUI*)pMsg->pSender;
			CTreeNodeUI* pItem = (CTreeNodeUI*)pCheckBox->GetParent()->GetParent();
			SetItemCheckBox(pCheckBox->GetCheck(),pItem);
		}
		return true;
	}

	//************************************
	// Method:    OnFolderChanged
	// FullName:  CTreeViewUI::OnFolderChanged
	// Access:    virtual public 
	// Returns:   bool
	// Qualifier:
	// Parameter: void * param
	// Note:	  
	//************************************
	bool CTreeViewUI::OnFolderChanged( void* param )
	{
		TNotifyUI* pMsg = (TNotifyUI*)param;
		if(pMsg->sType == _T("selectchanged"))
		{
			CCheckBoxUI* pFolder = (CCheckBoxUI*)pMsg->pSender;
			CTreeNodeUI* pItem = (CTreeNodeUI*)pFolder->GetParent()->GetParent();
			pItem->SetVisibleTag(!pFolder->GetCheck());
			SetItemExpand(!pFolder->GetCheck(),pItem);
		}
		return true;
	}

	//************************************
	// Method:    SetItemCheckBox
	// FullName:  CTreeViewUI::SetItemCheckBox
	// Access:    virtual public 
	// Returns:   bool
	// Qualifier:
	// Parameter: bool _Selected
	// Parameter: CTreeNodeUI * _TreeNode
	// Note:	  
	//************************************
	bool CTreeViewUI::SetItemCheckBox( bool _Selected,CTreeNodeUI* _TreeNode /*= NULL*/ )
	{
		if(_TreeNode)
		{
			if(_TreeNode->GetCountChild() > 0)
			{
				int nCount = _TreeNode->GetCountChild();
				for(int nIndex = 0;nIndex < nCount;nIndex++)
				{
					CTreeNodeUI* pItem = _TreeNode->GetChildNode(nIndex);
					pItem->GetCheckBox()->Selected(_Selected);
					if(pItem->GetCountChild())
						SetItemCheckBox(_Selected,pItem);
				}
			}
			return true;
		}
		else
		{
			int nIndex = 0;
			int nCount = GetCount();
			while(nIndex < nCount)
			{
				CTreeNodeUI* pItem = (CTreeNodeUI*)GetItemAt(nIndex);
				pItem->GetCheckBox()->Selected(_Selected);
				if(pItem->GetCountChild())
					SetItemCheckBox(_Selected,pItem);

				nIndex++;
			}
			return true;
		}
		return false;
	}

	//************************************
	// Method:    SetExpand
	// FullName:  CTreeViewUI::SetItemExpand
	// Access:    virtual public 
	// Returns:   void
	// Qualifier:
	// Parameter: bool _Expanded
	// Parameter: CTreeNodeUI * _TreeNode
	// Note:	  
	//************************************
	void CTreeViewUI::SetItemExpand( bool _Expanded,CTreeNodeUI* _TreeNode /*= NULL*/ )
	{
		if(_TreeNode)
		{
			if(_TreeNode->GetCountChild() > 0)
			{
				int nCount = _TreeNode->GetCountChild();
				for(int nIndex = 0;nIndex < nCount;nIndex++)
				{
					CTreeNodeUI* pItem = _TreeNode->GetChildNode(nIndex);
					pItem->SetVisible(_Expanded);

					if(pItem->GetCountChild() && !pItem->GetFolderButton()->IsSelected())
						SetItemExpand(_Expanded,pItem);
				}
			}
		}
		else
		{
			int nIndex = 0;
			int nCount = GetCount();
			while(nIndex < nCount)
			{
				CTreeNodeUI* pItem = (CTreeNodeUI*)GetItemAt(nIndex);

				pItem->SetVisible(_Expanded);

				if(pItem->GetCountChild() && !pItem->GetFolderButton()->IsSelected())
					SetItemExpand(_Expanded,pItem);

				nIndex++;
			}
		}
	}

	//************************************
	// Method:    SetVisibleFolderBtn
	// FullName:  CTreeViewUI::SetVisibleFolderBtn
	// Access:    virtual public 
	// Returns:   void
	// Qualifier:
	// Parameter: bool _IsVisibled
	// Node:	  
	//************************************
	void CTreeViewUI::SetVisibleFolderBtn( bool _IsVisibled )
	{
		m_bVisibleFolderBtn = _IsVisibled;
		int nCount = this->GetCount();
		for(int nIndex = 0;nIndex < nCount;nIndex++)
		{
			CTreeNodeUI* pItem = static_cast<CTreeNodeUI*>(this->GetItemAt(nIndex));
			pItem->GetFolderButton()->SetVisible(m_bVisibleFolderBtn);
		}
	}
	//************************************
	// Method:    GetVisibleFolderBtn
	// FullName:  CTreeViewUI::GetVisibleFolderBtn
	// Access:    virtual public 
	// Returns:   bool
	// Qualifier:
	// Node:	  
	//************************************
	bool CTreeViewUI::GetVisibleFolderBtn()
	{
		return m_bVisibleFolderBtn;
	}
	//************************************
	// Method:    SetVisibleCheckBtn
	// FullName:  CTreeViewUI::SetVisibleCheckBtn
	// Access:    virtual public 
	// Returns:   void
	// Qualifier:
	// Parameter: bool _IsVisibled
	// Node:	  
	//************************************
	void CTreeViewUI::SetVisibleCheckBtn( bool _IsVisibled )
	{
		m_bVisibleCheckBtn = _IsVisibled;
		int nCount = this->GetCount();
		for(int nIndex = 0;nIndex < nCount;nIndex++)
		{
			CTreeNodeUI* pItem = static_cast<CTreeNodeUI*>(this->GetItemAt(nIndex));
			pItem->GetCheckBox()->SetVisible(m_bVisibleCheckBtn);
		}
	}
	//************************************
	// Method:    GetVisibleCheckBtn
	// FullName:  CTreeViewUI::GetVisibleCheckBtn
	// Access:    virtual public 
	// Returns:   bool
	// Qualifier:
	// Node:	  
	//************************************
	bool CTreeViewUI::GetVisibleCheckBtn()
	{
		return m_bVisibleCheckBtn;
	}

	//************************************
	// Method:    SetAttribute
	// FullName:  CTreeViewUI::SetAttribute
	// Access:    virtual public 
	// Returns:   void
	// Qualifier:
	// Parameter: LPCTSTR pstrName
	// Parameter: LPCTSTR pstrValue
	// Note:	  
	//************************************
	void CTreeViewUI::SetAttribute( LPCTSTR pstrName, LPCTSTR pstrValue )
	{
		if(_tcscmp(pstrName,_T("visiblefolderbtn")) == 0)
			SetVisibleFolderBtn(_tcscmp(pstrValue,_T("true")) == 0);
		else if(_tcscmp(pstrName,_T("visiblecheckbtn")) == 0)
			SetVisibleCheckBtn(_tcscmp(pstrValue,_T("true")) == 0);
		else CListUI::SetAttribute(pstrName,pstrValue);
	}
}