#include "stdafx.h"
#include "VATCombo.h"
#include <string/TextUtils.h>

BEGIN_MESSAGE_MAP(CVATCombo::CComboEnterEdit,CEnterEdit)	
	ON_WM_CHAR()	
END_MESSAGE_MAP()

CVATCombo::CComboEnterEdit::CComboEnterEdit(const TCHAR *pszRegEx) 
{
	SetRegEx(pszRegEx);
}

void CVATCombo::CComboEnterEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(nChar == VK_TAB)
	{
		CWnd *pNextWnd = GetParent()->GetNextWindow();
		if(pNextWnd)
			pNextWnd->SetFocus();
	}
	CEnterEdit::OnChar(nChar,nRepCnt,nFlags);
}

BEGIN_MESSAGE_MAP(CVATCombo,CComboBox)
	ON_WM_CREATE()
	ON_CONTROL_REFLECT_EX(CBN_SELCHANGE,OnSelChange)
	ON_NOTIFY(EN_EDITCOMPLETE,IDC_COMBOENTEREDIT,OnEditComplete)
END_MESSAGE_MAP()


CVATCombo::CVATCombo() : m_enteredit(L"[0-9]{1,2}([,.][0-9]{0,2})?")
{
}


void CVATCombo::Init()
{
	double VATRates[] = 
	{
		0.0,
		3.0,
		5.0,
		7.0,
		8.0,
		9.0		
	};
	

	ResetContent();
	for(int i = 0;i < (sizeof(VATRates)/sizeof(double));i++)
	{
		AddString(TextUtils::FormatA(L"%.2f",VATRates[i]).c_str());
	}

}

NullDecimal CVATCombo::GetVATRate()
{
	std::wstring sVAT = m_enteredit.GetText();
	
	if(sVAT.empty())
		return NullDecimal();

	return NullDecimal(Decimal::FromString(sVAT)/Decimal(100L));
}

void CVATCombo::SetVATRate(const NullDecimal& VAT)
{
	std::wstring sVAT;
	if(!VAT.IsNull())
	{
		Decimal VAT100 = VAT.ToDecimal(Decimal(0L))*Decimal(100L);
		sVAT = VAT100.ToString(2);
	}

	m_enteredit.SetCommitedText(sVAT.c_str());
}
void CVATCombo::PreSubclassWindow()
{
	m_enteredit.SubclassDlgItem(IDC_COMBOENTEREDIT, this);
	CComboBox::PreSubclassWindow();
	Init();
}

int CVATCombo::OnCreate(LPCREATESTRUCT lpCreateStuct)
{
	if(CComboBox::OnCreate(lpCreateStuct) == -1) return -1;

	m_enteredit.SubclassDlgItem(IDC_COMBOENTEREDIT, this);
	Init();
	return 0;
}

BOOL CVATCombo::OnSelChange()
{
	CString s;
	GetLBText(GetCurSel(),s);
	SetWindowText(s);
	return FALSE;
}
void CVATCombo::OnEditComplete(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMHDR hdr;
	hdr.code = EN_EDITCOMPLETE;
	hdr.hwndFrom = GetSafeHwnd();
	hdr.idFrom = GetDlgCtrlID();
	GetParent()->SendMessage(WM_NOTIFY,hdr.idFrom,(LPARAM)&hdr);
	*pResult = 0;
}

bool CVATCombo::IsUncommited()
{
	return m_enteredit.IsUncommited();
}

