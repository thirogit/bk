#include "stdafx.h"
#include "PurchaseIssuesSolverDlg.h"
#include "../../ui/waitdlg/WaitDialog.h"
#include "../../utils/StringLoader.h"
#include "../RepoException.h"
#include <arrays/DeepDelete.h>
#include <boost/foreach.hpp>
#include <boost/range/adaptors.hpp>
#include "view/DownloadIssueViewCreator.h"
#include "../../ui/menu/MenuBuilder.h"
#include <data\runtime\SolutionType.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(CPurchaseIssuesSolverDlg, CLazyDlg<CDialog>)

CPurchaseIssuesSolverDlg::CPurchaseIssuesSolverDlg(DownloadPurchaseTransactionPtr transaction,CWnd* pParent /*=NULL*/) : CLazyDlg<CDialog>(CPurchaseIssuesSolverDlg::IDD, pParent)
{
	m_transaction = transaction;
}

CPurchaseIssuesSolverDlg::~CPurchaseIssuesSolverDlg()
{
}

void CPurchaseIssuesSolverDlg::DoDataExchange(CDataExchange* pDX)
{
	CLazyDlg<CDialog>::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INVOICES, m_InvoiceList);
	DDX_Control(pDX, IDC_COWS, m_CowList);
	DDX_Control(pDX, IDC_DEDUCTIONS, m_DeductionsList);
	DDX_Control(pDX, IDC_ISSUES, m_IssuesListCtrl);
}

BEGIN_MESSAGE_MAP(CPurchaseIssuesSolverDlg, CLazyDlg<CDialog>)

	ON_WM_DESTROY()
	ON_WM_CLOSE()

END_MESSAGE_MAP()


BOOL CPurchaseIssuesSolverDlg::OnInitDialog()
{
	CLazyDlg<CDialog>::OnInitDialog();

	m_InvoiceList.SetContent(&m_InvoiceListContent);
	m_CowList.SetContent(&m_CowListContent);

	m_IssuesListCtrl.SetOnRightClick(std::bind(CPurchaseIssuesSolverDlg::OnIssuesListRClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	return TRUE;
}

void CPurchaseIssuesSolverDlg::OnIssuesListRClick(int rowIndex, uint32_t colId, const CellDescriptor& cell)
{
	MenuBuilder menu;

	IssueRow* row =  m_IssuesListContent.GetIssueRowAt(rowIndex);
	ItemId id = row->Content()->GetIssueId();
	IPurchaseDownloadIssue* issue = m_transaction->GetIssue(id);

	uint32_t solutions = issue->GetSolutions();

	if (solutions & SolutionType_Add)
	{
		menu.add(StringLoader(IDS_SOLUTION_ADD), std::bind(&CPurchaseIssuesSolverDlg::SolveByAdding, this, id));
	}
	
	if (solutions & SolutionType_Clear)
	{
		menu.add(StringLoader(IDS_SOLUTION_CLEAR), std::bind(&CPurchaseIssuesSolverDlg::SolveByClearing, this, id));
	}

	if (solutions & SolutionType_Replace)
	{
		menu.add(StringLoader(IDS_SOLUTION_REPLACE), std::bind(&CPurchaseIssuesSolverDlg::SolveByReplacing, this, id));
	}

	menu.track(this);


}


void  CPurchaseIssuesSolverDlg::OnAfterDlgShow()
{
	const IPurchase* purchase = m_transaction->GetPurchase();

	PtrEnumerator<IPurchaseInvoice> enumInvoices = purchase->EnumInvoices();
	while (enumInvoices.hasNext())
	{
		m_InvoiceListContent.AddRow(*enumInvoices);
		enumInvoices.advance();
	}
	

	m_IssuesListCtrl.SetContent(NULL);
	DownloadIssueViewCreator viewCreator;
	IPurchaseDownloadIssue* issue;
	count_t issueCount = m_transaction->GetIssueCount();
	for (int i = 0; i < issueCount; i++)
	{
		issue = m_transaction->GetIssueAt(i);
		m_IssuesListContent.AddRow(issue);
	}
	m_IssuesListCtrl.SetContent(&m_IssuesListContent);
	
}

void CPurchaseIssuesSolverDlg::OnDestroy()
{
	//delete m_pRepoSession;
	//DeepDelete(m_PurchaseMap);
	//m_PurchaseListContent.Clear();	
}

void CPurchaseIssuesSolverDlg::OnCancel()
{	
	//DoDisconnect();
	EndDialog(IDCANCEL);
}

void CPurchaseIssuesSolverDlg::OnClose()
{
	OnCancel();
}

void CPurchaseIssuesSolverDlg::SolveByAdding(IPurchaseDownloadIssue* issue)
{
	if (issue->GetDomain() == PurchaseDownloadIssueDomain_Hent)
	{

	}
}

void CPurchaseIssuesSolverDlg::SolveByClearing(IPurchaseDownloadIssue* issue)
{
	m_transaction->SolveIssue(issue->GetId(), SolutionType_Clear);
}

void CPurchaseIssuesSolverDlg::SolveByReplacing(IPurchaseDownloadIssue* issue)
{

	switch (issue->GetDomain())
	{
	case PurchaseDownloadIssueDomain_Herd:
	case PurchaseDownloadIssueDomain_Agent:
	case PurchaseDownloadIssueDomain_Hent:
	case PurchaseDownloadIssueDomain_Class:
	case PurchaseDownloadIssueDomain_Stock:
			
	}
	
}

uint32_t CPurchaseIssuesSolverDlg::SelectHerd();
uint32_t CPurchaseIssuesSolverDlg::SelectAgent();
uint32_t CPurchaseIssuesSolverDlg::SelectHent();
uint32_t CPurchaseIssuesSolverDlg::SelectClass();
uint32_t CPurchaseIssuesSolverDlg::SelectStock();