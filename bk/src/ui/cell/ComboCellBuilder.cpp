#include "stdafx.h"
#include "ComboCellBuilder.h"
#include <data\IIdable.h>

#define CB_DROPDOWNHEIGHT 100
#define IDC_STOCKCOMBOCELL 9002

BEGIN_MESSAGE_MAP(ComboCellBuilder::CComboCell, CBaseCombo)
	
	ON_WM_CREATE()
	ON_WM_KILLFOCUS()
	ON_WM_CHAR()
	ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCloseup)
	ON_WM_NCDESTROY()
	ON_WM_GETDLGCODE()
	
END_MESSAGE_MAP()


int ComboCellBuilder::CComboCell::Create(const RECT &rect,CWnd *pParentWnd,UINT nID)
{
	DWORD dwStyle =  WS_VSCROLL| WS_VISIBLE | CBS_DROPDOWNLIST |  WS_CHILD | WS_BORDER |  ES_AUTOHSCROLL | ES_AUTOVSCROLL |  CBS_DISABLENOSCROLL;
	
	CRect dropRect(rect);
	dropRect.bottom += CB_DROPDOWNHEIGHT;
	return CBaseCombo::Create(dwStyle, dropRect,pParentWnd, nID);	
}


void ComboCellBuilder::CComboCell::SetSelectReceiver(SelectReceiver& selRecvr)
{
	m_selrecver = selRecvr;
}

BOOL ComboCellBuilder::CComboCell::PreTranslateMessage(MSG* pMsg)
{
	// If the message if for "Enter" or "Esc"
	// Do not process
	if (pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			// DO NOT process further
			return TRUE;				
		}
	}
	
	return CBaseCombo::PreTranslateMessage(pMsg);
}

void ComboCellBuilder::CComboCell::OnKillFocus(CWnd* pNewWnd)
{
	CBaseCombo::OnKillFocus(pNewWnd);
	DestroyWindow();	
}

void ComboCellBuilder::CComboCell::OnNcDestroy()
{
	CBaseCombo::OnNcDestroy();    
    delete this;
}

void ComboCellBuilder::CComboCell::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// If the key is "Esc" set focus back to the list control
	switch(nChar)
	{
	case VK_RETURN:
			SendItem();
			GetParent()->SetFocus();
	return ;
	case VK_ESCAPE:
			GetParent()->SetFocus();
			m_bESC = TRUE;
	return;
	}
	
	CBaseCombo::OnChar(nChar, nRepCnt, nFlags);
}

void ComboCellBuilder::CComboCell::OnCloseup() 
{
	// Set the focus to the parent list control
	if(!m_bESC) SendItem();
	GetParent()->SetFocus();
}


int ComboCellBuilder::CComboCell::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBaseCombo::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}
	m_bESC = FALSE;
	
	// Set the proper font
	CFont* pFont = GetParent()->GetFont();
	SetFont(pFont);	

	SetFocus();	
	return 0;
}

UINT ComboCellBuilder::CComboCell::OnGetDlgCode()
{
	  return CBaseCombo::OnGetDlgCode() | DLGC_WANTARROWS | DLGC_WANTTAB;
}

void ComboCellBuilder::CComboCell::SendItem()
{
	if(m_selrecver)
		m_selrecver(GetSelectedItemId());
}

ComboCellBuilder::ComboCellBuilder() : m_selectedItemId(NULL_ID)
{
}

	
ComboCellBuilder& ComboCellBuilder::add(uint32_t itemId,const std::wstring& sItemText)
{
	m_content.AddItem(itemId,sItemText);
	return *this;
}

ComboCellBuilder& ComboCellBuilder::content(const ComboContent& comboContent)
{
	m_content.CopyFrom(comboContent);
	return *this;
}

ComboCellBuilder& ComboCellBuilder::select(uint32_t itemId)
{
	m_selectedItemId = itemId;
	return *this;
}

ComboCellBuilder& ComboCellBuilder::receiver(SelectReceiver& recver)
{
	m_recver = recver;
	return *this;
}

void ComboCellBuilder::show(CWnd* pParent,const CRect& rect)
{
	CComboCell *pCombo = new CComboCell();	
	pCombo->SetSelectReceiver(m_recver);
	pCombo->Create (rect,pParent, IDC_STOCKCOMBOCELL);		
	pCombo->SetContent(m_content);
	pCombo->Select(m_selectedItemId);

}


