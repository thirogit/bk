#include "stdafx.h"
#include "FindHentDlg.h"
#include "../hents/HentsTab.h"
#include <arrays/DeepDelete.h>

#include "../../content/predicate/HentHentTypePredicate.h"
#include "../../content/predicate/HentFiscalNoPredicate.h"
#include "../../content/predicate/HentPersonalNoPredicate.h"
#include "../../content/predicate/HentHentNoPredicate.h"
#include "../../content/predicate/HentAliasPredicate.h"
#include "../../content/predicate/HentNamePredicate.h"
#include "../../content/predicate/HentCityPredicate.h"
#include "../../content/predicate/HentStreetPredicate.h"
#include "../../content/predicate/HentPOBoxPredicate.h"
#include "../../content/predicate/HentZipPredicate.h"
#include "../../content/predicate/AndPredicate.h"

IMPLEMENT_DYNCREATE(FindHentDlg, FindDlg)

BEGIN_MESSAGE_MAP(FindHentDlg, CDialog)
	ON_MESSAGE(WM_FIND, OnFind)
	ON_BN_CLICKED(IDC_CLEARHENTTYPE, OnClearHentType)
	
END_MESSAGE_MAP()


FindHentDlg::FindHentDlg() :
m_fiscalNo(L"[0-9]{0,10}"),
m_personalNo(L"[0-9]{0,11}"),
m_hentNo(L"^[a-zA-Z]{0,2}[0-9]{0,12}")
{
}

void FindHentDlg::Create(FindContainerDlg* container)
{
	m_container = container;
	m_pParentWnd = m_container;
	CDialog::Create(FindHentDlg::IDD, m_container);
}

void FindHentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BYHENTTYPE, m_henttype);
	DDX_Control(pDX, IDC_BYFISCALNO, m_fiscalNo);
	DDX_Control(pDX, IDC_BYPERSONALNO, m_personalNo);	
	DDX_Control(pDX, IDC_BYHENTNO, m_hentNo);
	DDX_Control(pDX, IDC_CLEARHENTTYPE, m_clearHentType);

	DDX_StdText(pDX, IDC_BYALIAS, m_alias);
	DDX_StdText(pDX, IDC_BYNAME, m_name);
	DDX_StdText(pDX, IDC_BYCITY, m_city);
	DDX_StdText(pDX, IDC_BYSTREET, m_street);
	DDX_StdText(pDX, IDC_BYPOBOX, m_pobox);
	DDX_StdText(pDX, IDC_BYZIP, m_zip);
	

}
BOOL FindHentDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_henttype.ReloadCombo();
	m_clearHentType.SetImage(IDPNG_TRASHSMALL);
	return TRUE ;
}


void FindHentDlg::OnOK()
{
	UpdateData();

	
	NMFINDRQ notification;
	notification.code = FN_FINDREQUESTED;
	notification.hwndFrom = GetSafeHwnd();
	notification.idFrom = GetDlgCtrlID();

	std::vector<Predicate<HentView>*> predicates;

	HentType henttype = m_henttype.GetSelectedHentType();
	if (henttype != HentType_None)
	{
		predicates.push_back(new HentHentTypePredicate(henttype));
	}

	std::wstring fiscalNo = m_fiscalNo.GetText();
	if (!fiscalNo.empty())
	{
		predicates.push_back(new HentFiscalNoPredicate(fiscalNo));
	}

	std::wstring personalNo = m_personalNo.GetText();
	if (!personalNo.empty())
	{
		predicates.push_back(new HentPersonalNoPredicate(personalNo));
	}

	std::wstring hentNo = m_hentNo.GetText();
	if (!hentNo.empty())
	{
		predicates.push_back(new HentHentNoPredicate(hentNo));
	}

	if (!m_alias.empty())
	{
		predicates.push_back(new HentAliasPredicate(m_alias));
	}

	if (!m_name.empty())
	{
		predicates.push_back(new HentNamePredicate(m_name));
	}

	if (!m_city.empty())
	{
		predicates.push_back(new HentCityPredicate(m_city));
	}

	if (!m_street.empty())
	{
		predicates.push_back(new HentStreetPredicate(m_street));
	}

	if (!m_pobox.empty())
	{
		predicates.push_back(new HentPOBoxPredicate(m_pobox));
	}

	if (!m_zip.empty())
	{
		predicates.push_back(new HentZipPredicate(m_zip));
	}

	if (!predicates.empty())
	{
		AndPredicate<HentView> predicate(predicates);
		
		notification.predicte = &predicate;

		m_container->SendMessage(WM_NOTIFY, notification.idFrom, (LPARAM)&notification);
		DeepDelete(predicates);
	}		
}

void FindHentDlg::OnCancel()
{
	
}

void FindHentDlg::OnClearHentType()
{
	m_henttype.ReloadCombo();
}

LRESULT FindHentDlg::OnFind(WPARAM wParam, LPARAM lParam)
{
	OnOK();
	return 0;
}

