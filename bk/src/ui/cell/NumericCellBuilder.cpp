#include "stdafx.h"
#include "NumericCellBuilder.h"
#include <string/TextUtils.h>

#define IDC_EDITCELL 9004

BEGIN_MESSAGE_MAP(NumericCellBuilder::CNumericCell, CNumericEdit)
  
    ON_WM_KILLFOCUS()
    ON_WM_NCDESTROY()
    ON_WM_CHAR()
    ON_WM_CREATE()
    ON_WM_GETDLGCODE()
	
END_MESSAGE_MAP()


NumericCellBuilder::CNumericCell::CNumericCell(int precision) : CNumericEdit(precision)
{
}

void NumericCellBuilder::CNumericCell::SendText()
{
	std::wstring text = GetText();
	if (m_txtreceiver)
		m_txtreceiver(text);

	if (m_decreceiver)
	{
		NullDecimal decimal;
		if (!text.empty())
			decimal = Decimal::FromString(text);

		m_decreceiver(decimal);
	}

	if (m_fltreceiver)
	{
		NullDouble d;
		if (!text.empty())
			d = TextUtils::AlphaToFloat(text);

		m_fltreceiver(d);
	}

	
	
}

BOOL NumericCellBuilder::CNumericCell::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN)
    {
	    if (pMsg->wParam == VK_RETURN || /*pMsg->wParam == VK_DELETE || */
			pMsg->wParam == VK_ESCAPE || /*pMsg->wParam == VK_TAB || 
			pMsg->wParam == VK_UP || pMsg->wParam == VK_DOWN ||*/ GetKeyState (VK_CONTROL))
			{
				::TranslateMessage (pMsg);
				::DispatchMessage (pMsg);
				return TRUE;		    	// DO NOT process further
			}
    }

    return CNumericEdit::PreTranslateMessage (pMsg);
}


void NumericCellBuilder::CNumericCell::OnKillFocus(CWnd* pNewWnd)
{
    CNumericEdit::OnKillFocus(pNewWnd);
    DestroyWindow();
}


void NumericCellBuilder::CNumericCell::OnNcDestroy()
{
    CNumericEdit::OnNcDestroy();
    delete this;
}

void NumericCellBuilder::CNumericCell::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    //BOOL Shift = GetKeyState (VK_SHIFT) < 0;
    switch (nChar)
    {
		case VK_RETURN :
			SendText();
		case VK_ESCAPE :
			GetParent()->SetFocus();
			return;
   }
    CNumericEdit::OnChar (nChar, nRepCnt, nFlags);
}

int NumericCellBuilder::CNumericCell::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CNumericEdit::OnCreate (lpCreateStruct) == -1)
		return -1;

    // Set the proper font
	CFont* Font = GetParent()->GetFont();
    SetFont (Font);

    //SetWindowText (InitText);
    
	SetFocus();
    
    return 0;
}

UINT NumericCellBuilder::CNumericCell::OnGetDlgCode()
{
    return CNumericEdit::OnGetDlgCode() | DLGC_WANTARROWS | DLGC_WANTTAB;
}
int NumericCellBuilder::CNumericCell::Create(const RECT &rect, CWnd *pParentWnd, UINT nID)
{
	return CNumericEdit::Create(ES_LEFT | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,rect,pParentWnd,nID);
}

//-------------------------------------------------------------------------------------------------

NumericCellBuilder::NumericCellBuilder() : m_precision(2)
{
}


NumericCellBuilder& NumericCellBuilder::precision(int precision)
{
	m_precision = max(precision,0);
	return *this;
}

NumericCellBuilder& NumericCellBuilder::value(const NullDecimal& decvalue)
{
	m_setter = std::bind(&NumericCellBuilder::set_decimal, this, decvalue, std::placeholders::_1);
	return *this;
}

NumericCellBuilder& NumericCellBuilder::value(const NullDouble& dvalue)
{
	m_setter = std::bind(&NumericCellBuilder::set_double, this, dvalue, std::placeholders::_1);
	return *this;
}

NumericCellBuilder& NumericCellBuilder::receiver(NumberTextReceiver& recver)
{
	m_txtrecver = recver;
	
	return *this;
}

NumericCellBuilder& NumericCellBuilder::receiver(DecimalReceiver& recver)
{
	m_decrecver = recver;	
	return *this;
}

NumericCellBuilder& NumericCellBuilder::receiver(FloatReceiver& recver)
{
	m_floatrecver = recver;
	return *this;
}

std::wstring NumericCellBuilder::set_decimal(const NullDecimal& value, int precision)
{
	return value.ToString(DecimalFormat(precision, L'.'));
}

std::wstring NumericCellBuilder::set_double(const NullDouble& value, int precision)
{
	return value.ToString(FloatFormat(precision,L'.'));
}


void NumericCellBuilder::show(CWnd* pParent,const CRect& rect)
{
	CNumericCell *pEdit = new CNumericCell(m_precision);
	pEdit->m_decreceiver = m_decrecver;
	pEdit->m_txtreceiver = m_txtrecver;
	pEdit->m_fltreceiver = m_floatrecver;
	
	pEdit->Create (rect,pParent, IDC_EDITCELL);		
	if (m_setter)
		pEdit->SetText(m_setter(m_precision));

	pEdit->SetSel (0, -1);
}