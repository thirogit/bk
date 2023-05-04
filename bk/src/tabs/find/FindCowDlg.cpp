#include "stdafx.h"
#include "FindCowDlg.h"
#include "FindContainerDlg.h"
#include "../../content/predicate/CowPresenceCowNoPredicate.h"
#include "../../content/predicate/CowPresenceWeightRangePredicate.h"
#include "../../content/predicate/AndPredicate.h"
#include "../../content/predicate/CowPresenceAgeRangePredicate.h"
#include "../../content/StocksComboContent.h"
#include "../../content/ClassesComboContent.h"
#include "../../content/predicate/CowPresenceOwnStockPredicate.h"
#include "../../content/predicate/CowPresenceOwnClassPredicate.h"
#include "../../content/predicate/CowPresencePassportNoPredicate.h"
#include "../../content/predicate/CowPresenceBirthDatePredicate.h"
#include "../../content/predicate/CowPresenceCowSexPredicate.h"
#include "../../content/predicate/CowPresenceHerdEntryDatePredicate.h"
#include "../../content/predicate/CowPresenceHerdExitDatePredicate.h"
#include "../../content/predicate/CowPresenceBoughtFromPredicate.h"
#include "../../content/predicate/CowPresenceSoldToPredicate.h"
#include "../../hent/HentSearchDlg.h"
#include "../../AppServices.h"

IMPLEMENT_DYNCREATE(FindCowDlg, FindDlg)

BEGIN_MESSAGE_MAP(FindCowDlg, FindDlg)

	ON_BN_CLICKED(IDC_BIRTHENABLE, OnBirthDtEnable)
	ON_BN_CLICKED(IDC_BUYFROMENABLE, OnBoughtFromEnable)

	ON_BN_CLICKED(IDC_ENTRYDTENABLE, OnHerdEntryDtEnable)
	ON_BN_CLICKED(IDC_EXITDTENABLE, OnHerdExitDtEnable)

	ON_BN_CLICKED(IDC_BUYFROMENABLE, OnBoughtFromEnable)
	ON_BN_CLICKED(IDC_SOLDTOENABLE, OnSoldToEnable)

	ON_MESSAGE(WM_FIND, OnFind)
	ON_BN_CLICKED(IDC_CLEARSTOCK, OnClearStock)
	ON_BN_CLICKED(IDC_CLEARCLASS, OnClearClass)
	ON_BN_CLICKED(IDC_CLEARCOWSEX, OnClearCowSex)

	ON_BN_CLICKED(IDC_BYBOUGHTFROM, OnBoughtFromHent)
	ON_BN_CLICKED(IDC_BYSOLDTO, OnSoldToHent)
	
	ON_EN_KILLFOCUS(IDC_BYCOWNO,OnByCowNoKillFocus)
	ON_EN_SETFOCUS(IDC_BYCOWNO,OnByCowNoSetFocus)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


FindCowDlg::FindCowDlg() : m_CowNo(L"^[a-zA-Z]{0,2}[0-9]{0,12}"), m_PassportNo(L"^[a-zA-Z]{0,2}[0-9]{0,8}"), m_WeightSpan(3), 
		m_ByBirthDt(FALSE), m_ByBoughtFromEnabled(FALSE), m_BySoldToEnabled(FALSE), m_ByEntryDtEnabled(FALSE), m_ByExitDtEnabled(FALSE)
{
}

void FindCowDlg::Create(FindContainerDlg* container)
{
	m_container = container;
	m_pParentWnd = m_container;
	CDialog::Create(FindCowDlg::IDD, m_container);
}

void FindCowDlg::DoDataExchange(CDataExchange* pDX)
{
	FindDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BIRTHDATESPAN, m_BirthDtSpan);
	DDX_Control(pDX, IDC_BYCOWNO, m_CowNo);
	DDX_Control(pDX, IDC_BYPASSNO, m_PassportNo);
	DDX_Check(pDX, IDC_BIRTHENABLE, m_ByBirthDt);
	DDX_Control(pDX, IDC_BYWEIGHT, m_WeightSpan);
	DDX_Control(pDX, IDC_SCANNERSTATUS, m_scannerIndicator);
	DDX_Control(pDX, IDC_AGESPAN, m_AgeSpan);
	DDX_Control(pDX, IDC_BYSTOCK, m_stocksCB);
	DDX_Control(pDX, IDC_BYCLASS, m_classesCB);
	DDX_Control(pDX, IDC_BYSEX, m_cowSexCB);
	DDX_Control(pDX, IDC_CLEARSTOCK, m_clearStockBtn);
	DDX_Control(pDX, IDC_CLEARCLASS, m_clearClassBtn);
	DDX_Control(pDX, IDC_CLEARCOWSEX, m_clearSexBtn);

	DDX_Control(pDX, IDC_BYBOUGHTFROM, m_BoughtFrom);
	DDX_Control(pDX, IDC_BYSOLDTO, m_SoldTo);

	DDX_Check(pDX, IDC_BUYFROMENABLE, m_ByBoughtFromEnabled);
	
	DDX_Check(pDX, IDC_SOLDTOENABLE, m_BySoldToEnabled);

	DDX_Control(pDX, IDC_ENTRYDATESPAN, m_EntryDtSpan);
	DDX_Control(pDX, IDC_EXITDATESPAN, m_ExitDtSpan);


	DDX_Check(pDX, IDC_ENTRYDTENABLE, m_ByEntryDtEnabled);
	DDX_Check(pDX, IDC_EXITDTENABLE, m_ByExitDtEnabled);

}
BOOL FindCowDlg::OnInitDialog()
{
	FindDlg::OnInitDialog();

	DWORD a = m_BoughtFrom.GetExStyle();
	m_BoughtFrom.ModifyStyleEx(0, HBS_EX_NOEDITHENT);
	DWORD b = m_BoughtFrom.GetExStyle();
	m_SoldTo.ModifyStyleEx(0, HBS_EX_NOEDITHENT);

	m_ByBoughtFromEnabled = FALSE;
	m_BySoldToEnabled = FALSE;
	m_ByEntryDtEnabled = FALSE;
	m_ByExitDtEnabled = FALSE;
	m_ByBirthDt = FALSE;
	UpdateData(FALSE);
	OnBirthDtEnable();
	OnHerdEntryDtEnable();
	OnHerdExitDtEnable();
	OnBoughtFromEnable();
	OnSoldToEnable();


	SeasonSession* session = GetSeasonSession();

	StocksComboContent stocksContent;
	stocksContent.CreateContent(session);
	m_stocksCB.SetContent(stocksContent);

	ClassesComboContent classesContent;
	classesContent.CreateContent(session);
	m_classesCB.SetContent(classesContent);

	m_cowSexCB.InitCombo();
	m_cowSexCB.SetCurSel(-1);
	
	m_clearStockBtn.SetImage(IDPNG_TRASHSMALL);
	m_clearClassBtn.SetImage(IDPNG_TRASHSMALL);
	m_clearSexBtn.SetImage(IDPNG_TRASHSMALL);


	m_scannerIndicator.SetStatus(AppServices::GetBCScannerService()->GetConnectionStatus());
	BarcodeScannerService* bcService = AppServices::GetBCScannerService();
	bcService->AddListener(this);


	return TRUE ;
}

void FindCowDlg::OnBoughtFromHent()
{
	OnHent(m_BoughtFrom);
}

void FindCowDlg::OnSoldToHent()
{
	OnHent(m_SoldTo);
}

void FindCowDlg::OnHent(CHentBtn& btn)
{
	HentSearchDlg hentSearchDlg(this);
	uint32_t hentId = hentSearchDlg.FindHent();
	if (hentId != NULL_ID)
	{
		btn.SetHent(hentId);
	}
}

void FindCowDlg::OnOK()
{
	std::vector<Predicate<CowPresenceView>*> predicates;

	UpdateData();
	
	NMFINDRQ notification;
	notification.code = FN_FINDREQUESTED;
	notification.hwndFrom = GetSafeHwnd();
	notification.idFrom = GetDlgCtrlID();
	
	std::wstring cowNoSearch = m_CowNo.GetText();
	if (!cowNoSearch.empty())
	{
		predicates.push_back(new CowPresenceCowNoPredicate(cowNoSearch));
	}

	DecimalRange weightRange = m_WeightSpan.GetRange();
	if (!weightRange.GetLow().IsNull() || !weightRange.GetHi().IsNull())
	{
		predicates.push_back(new CowPresenceWeightRangePredicate(weightRange));
	}

	IntRange ageRange = m_AgeSpan.GetRange();
	CalendarUnit unit = m_AgeSpan.GetUnit();


	if (unit != CalendarUnit_None && (!ageRange.GetHi().IsNull() || !ageRange.GetLow().IsNull()))
	{
		predicates.push_back(new CowPresenceAgeRangePredicate(ageRange, unit));
	}
	
	uint32_t stockId = m_stocksCB.GetSelectedItemId();
	if (stockId != NULL_ID)
	{
		predicates.push_back(new CowPresenceOwnStockPredicate(stockId));
	}

	uint32_t cowClassId = m_classesCB.GetSelectedItemId();
	if (cowClassId != NULL_ID)
	{
		predicates.push_back(new CowPresenceOwnClassPredicate(cowClassId));
	}

	std::wstring passportNo = m_PassportNo.GetText();
	if (!passportNo.empty())
	{		                         
		predicates.push_back(new CowPresencePassportNoPredicate(passportNo));
	}

	if (m_ByBirthDt)
	{
		DateRange birthDtRange = m_BirthDtSpan.GetRange();
		predicates.push_back(new CowPresenceBirthDatePredicate(birthDtRange));
	}

	if (m_cowSexCB.GetCurSel() != -1)
	{
		predicates.push_back(new CowPresenceCowSexPredicate(m_cowSexCB.GetSex()));
	}

	if (m_ByEntryDtEnabled)
	{
		predicates.push_back(new CowPresenceHerdEntryDatePredicate(m_EntryDtSpan.GetRange()));
	}

	if (m_ByExitDtEnabled)
	{
		predicates.push_back(new CowPresenceHerdExitDatePredicate(m_ExitDtSpan.GetRange()));
	}

	if (m_ByBoughtFromEnabled)
	{
		predicates.push_back(new CowPresenceBoughtFromPredicate(m_BoughtFrom.GetHent()));
	}

	if (m_BySoldToEnabled)
	{
		predicates.push_back(new CowPresenceSoldToPredicate(m_SoldTo.GetHent()));
	}
	

	if (!predicates.empty())
	{
		AndPredicate<CowPresenceView> predicate(predicates);
		notification.predicte = &predicate;
		
		m_container->SendMessage(WM_NOTIFY, notification.idFrom, (LPARAM)&notification);
		DeepDelete(predicates);
	}
}

LRESULT FindCowDlg::OnFind(WPARAM wParam, LPARAM lParam)
{
	OnOK();
	return 0;
}

void FindCowDlg::OnCancel()
{
	
}

void FindCowDlg::OnBirthDtEnable()
{
	UpdateData();
	m_BirthDtSpan.EnableWindow(m_ByBirthDt);
}



void FindCowDlg::OnHerdEntryDtEnable()
{
	UpdateData();
	m_EntryDtSpan.EnableWindow(m_ByEntryDtEnabled);
}

void FindCowDlg::OnHerdExitDtEnable()
{
	UpdateData();
	m_ExitDtSpan.EnableWindow(m_ByExitDtEnabled);
}


void FindCowDlg::OnBoughtFromEnable()
{
	UpdateData();
	m_BoughtFrom.EnableWindow(m_ByBoughtFromEnabled);
}

void FindCowDlg::OnSoldToEnable()
{
	UpdateData();
	m_SoldTo.EnableWindow(m_BySoldToEnabled);
}

void FindCowDlg::OnClearStock()
{
	m_stocksCB.SetCurSel(-1);
}

void FindCowDlg::OnClearClass()
{
	m_classesCB.SetCurSel(-1);
}

void FindCowDlg::OnClearCowSex()
{
	m_cowSexCB.SetCurSel(-1);
}

void FindCowDlg::OnByCowNoKillFocus()
{
	m_scannerIndicator.EnableWindow(FALSE);
		
}

void FindCowDlg::OnDestroy()
{
	FindDlg::OnDestroy();
	BarcodeScannerService* bcService = AppServices::GetBCScannerService();
	bcService->RemoveListener(this);
}

void FindCowDlg::OnByCowNoSetFocus()
{
	m_scannerIndicator.EnableWindow(TRUE);	
	
}

void FindCowDlg::OnBarcode(const std::wstring& sBarcode)
{

	CWnd* focusedWnd = GetFocus();
	if(focusedWnd)
	{
		if (focusedWnd->m_hWnd != m_CowNo.m_hWnd)
			return;
	}

	m_CowNo.SetText(sBarcode);
	m_CowNo.SetSel(0, -1);
	OnOK();
}

void FindCowDlg::OnScannerConnected()
{
	m_scannerIndicator.SetStatus(ScannerConnectionStatus_Connected);
}

void FindCowDlg::OnScannerDisconnected()
{
	m_scannerIndicator.SetStatus(ScannerConnectionStatus_Disconnected);
}

