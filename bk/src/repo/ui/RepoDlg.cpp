#include "stdafx.h"
#include "RepoDlg.h"
#include "../../ui/waitdlg/WaitDialog.h"
#include "../../utils/StringLoader.h"
#include "../RepoException.h"
#include <arrays/DeepDelete.h>
#include <boost/foreach.hpp>
#include <boost/range/adaptors.hpp>
#include "PurchaseIssuesSolverDlg.h"
#include "../../ui/ExceptionMsgBox.h"
#include "../../ui/askfordatedlg/AskForDateDlg.h"
#include <string\TextUtils.h>
#include "../../ctrls/tipwnd/TimerTipWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static const Color _red(255, 201, 14);
static const int _hide_tooltip_after_ms = 2000;
static const int _tooltip_fontheight = 45;

IMPLEMENT_DYNAMIC(CRepoDlg, CLazyDlg<CSessionAwareDialog>)

CRepoDlg::CRepoDlg(CWnd* pParent /*=NULL*/) : CLazyDlg<CSessionAwareDialog>(CRepoDlg::IDD, pParent)
{
	m_pRepoSession = NULL;
	m_selectedPurchaseId = L"";
}

CRepoDlg::~CRepoDlg()
{
}

void CRepoDlg::DoDataExchange(CDataExchange* pDX)
{
	CLazyDlg<CSessionAwareDialog>::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PURACHELIST, m_PurchaseList);
	DDX_Control(pDX, IDC_PURCHASETABS, m_PurchaseTabs);
	DDX_Control(pDX, IDC_REFRESH, m_RefreshBtn);
	DDX_Control(pDX, IDC_SWITCHTOCURMONTH, m_SwitchToNowMonth);
	DDX_Control(pDX, IDC_DOWNLOAD, m_DownloadPurchaseBtn);

	DDX_Control(pDX, IDC_PREVMONTH, m_PrevMonth);
	DDX_Control(pDX, IDC_NEXTMONTH, m_NextMonth);

}

BEGIN_MESSAGE_MAP(CRepoDlg, CLazyDlg<CSessionAwareDialog>)

	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_SWITCHTOCURMONTH, OnSwitchToCurrentMonth)
	ON_BN_CLICKED(IDC_REFRESH, OnRefresh)
	ON_BN_CLICKED(IDC_PREVMONTH, OnPrevMonth)
	ON_BN_CLICKED(IDC_NEXTMONTH, OnNextMonth)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_PURACHELIST, OnPurchaseClick)
	ON_NOTIFY(LVN_KEYDOWN, IDC_PURACHELIST, OnPurchaseListKeyDn)
	ON_BN_CLICKED(IDC_DOWNLOAD, OnDownload)
	
END_MESSAGE_MAP()


BOOL CRepoDlg::OnInitDialog()
{
	CLazyDlg<CSessionAwareDialog>::OnInitDialog();
	m_selectedPurchaseId = L"";
	DateTime now = DateTime::now();
	now.SetYearMonthDay(now.GetYear(), now.GetMonth(), 15);
	m_ViewedMonth = now;

	SeasonSession* session = GetSeasonSession();
	m_pRepoSession = new RepoSession(session);

	m_PurchaseList.SetContent(&m_PurchaseListContent);

	UpdateCurrentMonthBtnCaption();

	m_PurchaseHentsPage.Create(CRepoPurchaseHentsPage::IDD, &m_PurchaseTabs);
	m_PurchaseContentPage.Create(CRepoPurchaseContentPage::IDD, &m_PurchaseTabs);
	m_PurchaseSummaryPage.Create(CRepoPurchaseSummaryPage::IDD, &m_PurchaseTabs);

	m_PurchaseTabs.AddTab(&m_PurchaseContentPage, StringLoader(IDS_PURCHASE_CONTENT_TABTEXT).c_str());
	m_PurchaseTabs.AddTab(&m_PurchaseHentsPage, StringLoader(IDS_PURCHASE_HENTS_TABTEXT).c_str());
	m_PurchaseTabs.AddTab(&m_PurchaseSummaryPage, StringLoader(IDS_PURCHASE_SUMMARY_TABTEXT).c_str());

	m_DownloadPurchaseBtn.SetImage(IDPNG_DOWNLOAD_BW16X16);

	MenuBuilder downloadMenu;
	downloadMenu.add(StringLoader(IDS_DOWNLOADWITHDT), std::bind(&CRepoDlg::OnDownloadWithDate, this));
	m_DownloadPurchaseBtn.SetMenu(downloadMenu);

	m_RefreshBtn.SetImage(IDPNG_REFRESH_BW16X16);
	m_SwitchToNowMonth.SetImage(IDPNG_WATCH_BW16X16);
	
	m_PrevMonth.SetImage(IDPNG_LEFTARROW_16X16BW);
	m_NextMonth.SetImage(IDPNG_RIGHTARROW_16X16BW);

	m_Msg.Create(this);
	m_Msg.SetShowCenteredInParent();

	return TRUE;
}

void CRepoDlg::OnDownloadWithDate()
{
	CAskForDateDlg askForDt(this);
	askForDt.SetWindowCaption(StringLoader(IDS_ASKENTERDOCDATE));
	askForDt.SetLabel(StringLoader(IDS_DOCDATELBL));
	DateTime docDt = askForDt.AskForDate();
	if (!docDt.IsNull())
		DownloadPurchase(docDt);
}

void CRepoDlg::UpdateCurrentMonthBtnCaption()
{
	const int months[] = {
		IDS_MONTH_JANUARY,
		IDS_MONTH_FEBRUARY,
		IDS_MONTH_MARCH,
		IDS_MONTH_APRIL,
		IDS_MONTH_MAY,
		IDS_MONTH_JUNE,
		IDS_MONTH_JULY,
		IDS_MONTH_AUGUST,
		IDS_MONTH_SEPTEMBER,
		IDS_MONTH_OCTOBER,
		IDS_MONTH_NOVEMBER,
		IDS_MONTH_DECEMBER
	};

	CWnd* btn = GetDlgItem(IDC_CURRENTMONTH);
	if (!m_ViewedMonth.IsNull())
	{		
		struct tm tm1;
		m_ViewedMonth.SetTmStruct(tm1);
		btn->SetWindowText(TextUtils::Format(L"%s %d", StringLoader(months[tm1.tm_mon]).c_str(), tm1.tm_year + 1900).c_str());

	}
	else
	{
		btn->SetWindowText(L"");
	}
}

void  CRepoDlg::OnAfterDlgShow()
{
	CWaitDialog waitDlg(this, StringLoader(IDS_REPO_CONNECTING));

	boost::function<void()> connectFn = boost::bind(&RepoSession::Open, m_pRepoSession);
	try
	{
		waitDlg.WaitFor(connectFn);
		WaitAndQuery();
	}
	catch (RepoException& e)
	{
		DisableAllControls();
		AfxMessageBox(e.wwhat());
	}
}

void CRepoDlg::DisableAllControls()
{
	const UINT controlIds[] = {

		IDC_PURACHELIST,
		IDC_PURCHASETABS,
		IDC_SWITCHTOCURMONTH,
		IDC_REFRESH,
		IDC_PREVMONTH,
		IDC_NEXTMONTH,
		IDC_DOWNLOAD,
		IDC_CURRENTMONTH,
		0
	};

	const UINT* controlId = controlIds;
	while (*controlId != 0)
	{
		GetDlgItem(*controlId)->EnableWindow(FALSE);
		controlId++;
	}

	m_PurchaseHentsPage.EnableWindow(FALSE);
	m_PurchaseContentPage.EnableWindow(FALSE);
	m_PurchaseSummaryPage.EnableWindow(FALSE);

}

void CRepoDlg::OnDestroy()
{
	delete m_pRepoSession;
	DeepDelete(m_PurchaseMap);
	m_PurchaseListContent.Clear();	
}

void CRepoDlg::WaitAndQuery()
{
	CWaitDialog waitDlg(this,StringLoader(IDS_QUERINGPURCHASES));
	try
	{
		waitDlg.WaitForTask(boost::bind(&CRepoDlg::Query, this));
	}
	catch (RepoException& e)
	{
		ExceptionMsgBox().Show(e);
	}
}

void CRepoDlg::Query()
{
	DateTime monthBegin = m_ViewedMonth.ToMonthBegin();
	DateTime monthEnd = m_ViewedMonth.ToMonthEnd();

	DeepDelete(m_PurchaseMap);
	m_PurchaseMap.clear();
	m_PurchaseListContent.Clear();

	m_PurchaseContentPage.UpdateFrom(NULL);
	m_PurchaseSummaryPage.UpdateFrom(NULL);
	m_PurchaseHentsPage.UpdateFrom(NULL);

	PurchaseHeaders purchaseHeaders = m_pRepoSession->QueryPurchases(monthBegin, monthEnd, NULL);
	m_PurchaseListContent.AddRows(purchaseHeaders);
}

void CRepoDlg::OnCancel()
{	
	DoDisconnect();
	EndDialog(IDCANCEL);
}

void CRepoDlg::OnPrevMonth()
{
	m_ViewedMonth = m_ViewedMonth.AddMonth(-1);
	UpdateCurrentMonthBtnCaption();
	WaitAndQuery();
}

void CRepoDlg::OnNextMonth()
{
	m_ViewedMonth = m_ViewedMonth.AddMonth(1);
	UpdateCurrentMonthBtnCaption();
	WaitAndQuery();
}

void CRepoDlg::OnSwitchToCurrentMonth()
{
	m_ViewedMonth = DateTime::now();
	UpdateCurrentMonthBtnCaption();
	WaitAndQuery();
}

void CRepoDlg::OnRefresh()
{
	WaitAndQuery();
	m_selectedPurchaseId = L"";
}

void CRepoDlg::OnClose()
{
	OnCancel();
}

void CRepoDlg::DoDisconnect()
{

	CWaitDialog waitDlg(this, StringLoader(IDS_REPO_DISCONNECTING));

	boost::function<void()> disconnectFn = boost::bind(&RepoSession::Close, m_pRepoSession);
	try
	{
		waitDlg.WaitFor(disconnectFn);
	}
	catch (...)
	{		
	}
}


void CRepoDlg::WaitAndFetchPurchase(const std::wstring& purchaseId)
{
	CWaitDialog waitDlg(this, StringLoader(IDS_DOWNLOADINGPURCHASE));
	try
	{
		waitDlg.WaitForTask(boost::bind(&CRepoDlg::FetchPurchase, this, purchaseId));
	}
	catch (RepoException& e)
	{
		ExceptionMsgBox().Show(e);
	}
}

void CRepoDlg::FetchPurchase(const std::wstring& purchaseId)
{
	boost::optional<Purchase> purchase = m_pRepoSession->GetPurchase(purchaseId);
	m_PurchaseMap[purchase->GetId()] = new Purchase(purchase.get_ptr());
}

void CRepoDlg::OnPurchaseSelected(const std::wstring& purchaseId)
{
	auto it = m_PurchaseMap.find(purchaseId);
	Purchase* purchase = NULL;
	if (it != m_PurchaseMap.end())
	{
		purchase = it->second;
	}
	else
	{
		WaitAndFetchPurchase(purchaseId);
		it = m_PurchaseMap.find(purchaseId);
		if (it != m_PurchaseMap.end())
		{
			purchase = it->second;
		}
	}
	
	m_PurchaseContentPage.UpdateFrom(purchase);
	m_PurchaseSummaryPage.UpdateFrom(purchase);
	m_PurchaseHentsPage.UpdateFrom(purchase);

}

void CRepoDlg::OnPurchaseUnselected(const std::wstring& purchaseId)
{
	
}

void CRepoDlg::OnPurchaseClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE plvItemActive = (LPNMITEMACTIVATE)pNMHDR;

	if (plvItemActive->iItem >= 0)
	{
		if (!m_selectedPurchaseId.empty())
			OnPurchaseUnselected(m_selectedPurchaseId);

		PurchaseHeaderRow* row = m_PurchaseListContent.GetPurchaseRowAt(plvItemActive->iItem);
		m_selectedPurchaseId = row->Content()->GetPurchaseId();
		OnPurchaseSelected(m_selectedPurchaseId);
	}
	*pResult = 0;
}

void CRepoDlg::OnPurchaseListKeyDn(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	// TODO: Add your control notification handler
	//       code here

	*pResult = 0;
}

void CRepoDlg::OnDownload()
{		
	DownloadPurchase(DateTime());
}


void CRepoDlg::DownloadPurchase(DateTime dt)
{
	int selectedIndex = m_PurchaseList.GetCurSel();
	if (selectedIndex >= 0)
	{
		PurchaseHeaderRow* row = m_PurchaseListContent.GetPurchaseRowAt(selectedIndex);
		DownloadPurchaseTransactionPtr transaction =
			GetSeasonSession()->OpenDownloadPurchaseTransaction(row->Content()->GetPurchaseId());

		transaction->SetDocDate(dt);
		if (transaction->GetIssueCount() > 0)
		{
			CPurchaseIssuesSolverDlg issuesSolvedDlg(transaction, this);
			if (issuesSolvedDlg.DoModal() == IDOK)
			{
				row->Content()->SetDownloaded(true);
				m_PurchaseList.Invalidate();
				ShowPurchaseDownloadedMsg();
			}
		}
		else
		{			
			transaction->Commit();
			row->Content()->SetDownloaded(true);
			m_PurchaseList.Invalidate();
			ShowPurchaseDownloadedMsg();
		}
	}
}

void CRepoDlg::ShowPurchaseDownloadedMsg()
{
	m_Msg.SetText(StringLoader(IDS_PRUCHASEDOWNLOADED));
	m_Msg.SetStyle(TipWndStyle().inner(_red).outer(_red).textsize(_tooltip_fontheight));
	m_Msg.Show(_hide_tooltip_after_ms);
	MessageBeep(MB_ICONERROR);
}