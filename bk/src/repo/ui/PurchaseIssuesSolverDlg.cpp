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
#include "../../hent/HentSearchDlg.h"
#include "../../content/AgentsComboContent.h"
#include "../../content/ClassesComboContent.h"
#include "../../content/HerdsComboContent.h"
#include "../../content/StocksComboContent.h"
#include "../../ui/comboselectdlg/ComboSelectDlg.h"
#include "../../context/transaction/TransactionException.h"
#include "InputCowNoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CPurchaseIssuesSolverDlg::SolveSingleInvoker::SolveSingleInvoker(SolutionType type, IPurchaseDownloadIssue* issue, DownloadPurchaseTransactionPtr transaction) : 
m_transaction(transaction), SolveInvoker(type, issue)
{
}

void CPurchaseIssuesSolverDlg::SolveSingleInvoker::Invoke(const SolutionParameters& parameters)
{
	m_transaction->SolveIssue(m_issue->GetId(), parameters);
}

CPurchaseIssuesSolverDlg::SolveCategoryInvoker::SolveCategoryInvoker(SolutionType type, IPurchaseDownloadIssue* issue, DownloadPurchaseTransactionPtr transaction) : 
m_transaction(transaction), SolveInvoker(type, issue)
{
}

void CPurchaseIssuesSolverDlg::SolveCategoryInvoker::Invoke(const SolutionParameters& parameters)
{
	m_transaction->SolveIssueCategory(m_issue->GetCategory(), parameters);
}



//-------------------------------------------------------------------------------------------------

IMPLEMENT_DYNAMIC(CPurchaseIssuesSolverDlg, CLazyDlg<CSessionAwareDialog>)

CPurchaseIssuesSolverDlg::CPurchaseIssuesSolverDlg(DownloadPurchaseTransactionPtr transaction,CWnd* pParent /*=NULL*/) : CLazyDlg<CSessionAwareDialog>(CPurchaseIssuesSolverDlg::IDD, pParent)
{
	m_transaction = transaction;
}

CPurchaseIssuesSolverDlg::~CPurchaseIssuesSolverDlg()
{
}

void CPurchaseIssuesSolverDlg::DoDataExchange(CDataExchange* pDX)
{
	CLazyDlg<CSessionAwareDialog>::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INVOICES, m_InvoiceList);
	DDX_Control(pDX, IDC_COWS, m_CowList);
	DDX_Control(pDX, IDC_DEDUCTIONS, m_DeductionsList);
	DDX_Control(pDX, IDC_ISSUES, m_IssuesListCtrl);
}

BEGIN_MESSAGE_MAP(CPurchaseIssuesSolverDlg, CLazyDlg<CSessionAwareDialog>)

	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_INVOICES, OnInvoiceClick)

END_MESSAGE_MAP()


BOOL CPurchaseIssuesSolverDlg::OnInitDialog()
{
	CLazyDlg<CSessionAwareDialog>::OnInitDialog();

	m_InvoiceList.SetContent(&m_InvoiceListContent);
	m_CowList.SetContent(&m_CowListContent);
	m_DeductionsList.SetContent(&m_DeductionListContent);

	m_IssuesListCtrl.SetOnRightClick(std::bind(&CPurchaseIssuesSolverDlg::OnIssuesListRClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	return TRUE;
}

void CPurchaseIssuesSolverDlg::OnIssuesListRClick(int rowIndex, uint32_t colId, const CellDescriptor& cell)
{
	
	if (rowIndex < 0) return;

	MenuBuilder menu;
	IssueRow* row =  m_IssuesListContent.GetIssueRowAt(rowIndex);
	if (row)
	{

		ItemId id = row->Content()->GetIssueId();
		IPurchaseDownloadIssue* issue = m_transaction->GetIssue(id);

		uint32_t solutions = issue->GetSolutions();

		if (solutions & SolutionType_Add)
		{
			menu.add(StringLoader(IDS_SOLUTION_ADD), std::bind(&CPurchaseIssuesSolverDlg::SolveByAdding, this, issue));			

			if (issue->GetCategory().GetDomain() == PurchaseDownloadIssueDomain_Hent)
			{
				menu.add(StringLoader(IDS_SOLUTION_ADDALLHENTS), std::bind(&CPurchaseIssuesSolverDlg::SolveByAddingAllHents, this));
			}
		}


		if (solutions & SolutionType_Clear)
		{
			menu.add(StringLoader(IDS_SOLUTION_CLEAR), std::bind(&CPurchaseIssuesSolverDlg::SolveByClearing, this, issue));
		}

		if (solutions & SolutionType_Replace)
		{
			menu.add(StringLoader(IDS_SOLUTION_REPLACE), std::bind(&CPurchaseIssuesSolverDlg::SolveByReplacingSingle, this, issue));
			menu.add(StringLoader(IDS_SOLUTION_REPLACEALL), std::bind(&CPurchaseIssuesSolverDlg::SolveByReplacingAll, this, issue));
		}

		menu.track(this);
	}

}

void CPurchaseIssuesSolverDlg::RefreshIssuesList()
{
	m_IssuesListCtrl.SetContent(NULL);
	m_IssuesListContent.Clear();
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


void CPurchaseIssuesSolverDlg::UpdateCowListFrom(IPurchaseInvoice* invoice)
{
	m_CowList.SetContent(NULL);
	m_CowListContent.Clear();

	if (invoice != NULL)
	{
		PtrEnumerator<IPurchaseCow> enumCows = invoice->EnumCows();
		while (enumCows.hasNext())
		{
			m_CowListContent.AddRow(*enumCows);
			enumCows.advance();
		}

		m_CowList.SetContent(&m_CowListContent);
	}
}

void CPurchaseIssuesSolverDlg::UpdateDeductionListFrom(IPurchaseInvoice* invoice)
{
	m_DeductionsList.SetContent(NULL);
	m_DeductionListContent.Clear();

	if (invoice != NULL)
	{
		PtrEnumerator<IPurchaseInvoiceDeduction> enumDeducions = invoice->EnumDeductions();
		while (enumDeducions.hasNext())
		{
			m_DeductionListContent.AddRow(*enumDeducions);
			enumDeducions.advance();
		}
		m_DeductionsList.SetContent(&m_DeductionListContent);
	}
}


void CPurchaseIssuesSolverDlg::OnInvoiceClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE plvItemActive = (LPNMITEMACTIVATE)pNMHDR;

	if (plvItemActive->iItem >= 0)
	{
		PurchaseInvoiceRow* invoiceRow = m_InvoiceListContent.GetInvoiceRowAt(plvItemActive->iItem);
		PurchaseInvoiceView* invoiceView = invoiceRow->Content();
		m_CowList.EnableWindow(TRUE);
		m_DeductionsList.EnableWindow(TRUE);

		const IPurchase* purchase = m_transaction->GetPurchase();
		IPurchaseInvoice* invoice = purchase->GetInvoice(invoiceView->GetInvoiceId());
		UpdateCowListFrom(invoice);
		UpdateDeductionListFrom(invoice);
	}
	*pResult = 0;
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

	if (purchase->GetInvoiceCount() == 1)
	{
		IPurchaseInvoice* onlyInvoice = purchase->GetInvoiceAt(0);
		UpdateCowListFrom(onlyInvoice);
		UpdateDeductionListFrom(onlyInvoice);
	}
	else
	{
		m_CowList.EnableWindow(FALSE);
		m_DeductionsList.EnableWindow(FALSE);
	}
	
	RefreshIssuesList();
	
}

void CPurchaseIssuesSolverDlg::OnDestroy()
{
	//delete m_pRepoSession;
	//DeepDelete(m_PurchaseMap);
	//m_PurchaseListContent.Clear();	
}

void CPurchaseIssuesSolverDlg::OnCancel()
{	
	m_transaction->Rollback();
	EndDialog(IDCANCEL);
}

void CPurchaseIssuesSolverDlg::OnClose()
{
	OnCancel();
}

void CPurchaseIssuesSolverDlg::SolveByReplacingSingle(IPurchaseDownloadIssue* issue)
{
	SolveSingleInvoker invoker(SolutionType_Replace, issue, m_transaction);
	SolveByReplacing(issue->GetCategory().GetDomain(),&invoker);
}

void CPurchaseIssuesSolverDlg::SolveByReplacingAll(IPurchaseDownloadIssue* issue)
{
	SolveCategoryInvoker invoker(SolutionType_Replace, issue, m_transaction);
	SolveByReplacing(issue->GetCategory().GetDomain(), &invoker);
}

void CPurchaseIssuesSolverDlg::SolveHentIssueByReplacing(SolveInvoker* invoker)
{
	uint32_t hentId = SelectHent();
	if (hentId != NULL_ID)
	{
		HentClientObject* hentCO = GetSeasonSession()->GetHent(hentId);
		if (hentCO)
		{
			IHent* hent = hentCO->Object();
			PurchaseHent purchaseHent(L"");
			purchaseHent.CopyFrom(hent);
			invoker->Solve(purchaseHent);
			RefreshIssuesList();
		}			
	}	
}

void CPurchaseIssuesSolverDlg::SolveHentIssueByAdding(IPurchaseDownloadIssue* issue)
{
	m_transaction->SolveIssue(issue->GetId(), SolutionParameters::make(SolutionType_Add));
	RefreshIssuesList();
}

void CPurchaseIssuesSolverDlg::SolveByAddingAllHents()
{

	try
	{
		CWaitDialog waitDlg(this, StringLoader(IDS_WAITADDING));

		waitDlg.WaitForTask([this]()
		{
			bool bThereWasAHentIssue = false;
			do 
			{
				bThereWasAHentIssue = false;
				for (int i = 0, count = m_transaction->GetIssueCount(); i < count; i++)
				{
					IPurchaseDownloadIssue* issue = m_transaction->GetIssueAt(i);
					PurchaseDownloadIssueCategory category = issue->GetCategory();
					if (category.GetDomain() == PurchaseDownloadIssueDomain_Hent)
					{
						m_transaction->SolveIssue(issue->GetId(), SolutionParameters::make(SolutionType_Add));
						bThereWasAHentIssue = true;
						break;
					}
				}
			} while (bThereWasAHentIssue);
		});

		
	}
	catch (TransactionException& e)
	{
		AfxMessageBox(e.wwhat());
	}
		
	RefreshIssuesList();
}

void CPurchaseIssuesSolverDlg::SolveByClearing(IPurchaseDownloadIssue* issue)
{
	m_transaction->SolveIssue(issue->GetId(), SolutionParameters::make(SolutionType_Clear));
	RefreshIssuesList();
}

void CPurchaseIssuesSolverDlg::SolveByAdding(IPurchaseDownloadIssue* issue)
{
	PurchaseDownloadIssueCategory category = issue->GetCategory();
	switch (category.GetDomain())
	{
	case PurchaseDownloadIssueDomain_Hent:	
		SolveHentIssueByAdding(issue);
		break;
	}

	RefreshIssuesList();
}

void CPurchaseIssuesSolverDlg::SolveByReplacing(PurchaseDownloadIssueDomain domain,SolveInvoker* invoker)
{	
	switch (domain)
	{
	case PurchaseDownloadIssueDomain_Herd:
		SolveHerdIssue(invoker);
		break;
	case PurchaseDownloadIssueDomain_Agent:
		SolveAgentIssue(invoker);
		break;
	case PurchaseDownloadIssueDomain_Hent:
		SolveHentIssueByReplacing(invoker);
		break;
	case PurchaseDownloadIssueDomain_Class:
		SolveClassIssue(invoker);
		break;	
	case PurchaseDownloadIssueDomain_Stock:
		SolveStockIssue(invoker);
		break;	
	case PurchaseDownloadIssueDomain_Cow:
		SolveCowIssue(invoker);
		break;
	}
	RefreshIssuesList();	
}



void CPurchaseIssuesSolverDlg::SolveHerdIssue(SolveInvoker* invoker)
{
	uint32_t herdId = SelectHerd();
	if (herdId != NULL_ID)
	{
		IHerd* herd = GetSeasonSession()->GetHerd(herdId);
		if (herd != NULL)
		{
			int herdIndex = herd->GetHerdIndex();
			invoker->Solve<int>(herdIndex);
		}
	}
}

void CPurchaseIssuesSolverDlg::SolveAgentIssue(SolveInvoker* invoker)
{
	uint32_t agentId = SelectAgent();
	if (agentId != NULL_ID)
	{
		IAgent* agent = GetSeasonSession()->GetAgent(agentId);
		if (agent != NULL)
		{
			invoker->Solve(agent->GetAgentCode());
		}
	}
}

void CPurchaseIssuesSolverDlg::SolveClassIssue(SolveInvoker* invoker)
{
	uint32_t classId = SelectClass();
	if (classId != NULL_ID)
	{
		IClass* cowclass = GetSeasonSession()->GetClass(classId);
		if (cowclass != NULL)
		{
			invoker->Solve(cowclass->GetClassCode());			
		}
	}

}

void CPurchaseIssuesSolverDlg::SolveStockIssue(SolveInvoker* invoker)
{
	uint32_t stockId = SelectStock();
	if (stockId != NULL_ID)
	{
		IStock* stock = GetSeasonSession()->GetStock(stockId);
		if (stock != NULL)
		{
			invoker->Solve(stock->GetStockCode());
		}
	}
}

uint32_t CPurchaseIssuesSolverDlg::SelectHerd()
{
	HerdsComboContent herdsContent;
	herdsContent.CreateContent(GetSeasonSession());

	CComboSelectDlg herdSelectDlg(this);
	herdSelectDlg.SetCaption(StringLoader(IDS_ASKTOSELECTHERD));
	return herdSelectDlg.Select(herdsContent, NULL_ID);
}

uint32_t CPurchaseIssuesSolverDlg::SelectAgent()
{
	AgentsComboContent agentsContent;
	agentsContent.CreateContent(GetSeasonSession());

	CComboSelectDlg agentSelectDlg(this);
	agentSelectDlg.SetCaption(StringLoader(IDS_ASKTOSELECTAGENT));
	return agentSelectDlg.Select(agentsContent,NULL_ID);
}

uint32_t CPurchaseIssuesSolverDlg::SelectHent()
{
	HentSearchDlg searchDlg(this);
	return searchDlg.FindHent();
}

uint32_t CPurchaseIssuesSolverDlg::SelectClass()
{
	ClassesComboContent classesContent;
	classesContent.CreateContent(GetSeasonSession());

	CComboSelectDlg classSelectDlg(this);
	classSelectDlg.SetCaption(StringLoader(IDS_ASKTOSELECTCLASS));
	return classSelectDlg.Select(classesContent, NULL_ID);
}

uint32_t CPurchaseIssuesSolverDlg::SelectStock()
{
	StocksComboContent stocksContent;
	stocksContent.CreateContent(GetSeasonSession());

	CComboSelectDlg stockSelectDlg(this);
	stockSelectDlg.SetCaption(StringLoader(IDS_ASKTOSELECTSTOCK));
	uint32_t selectedStockId = stockSelectDlg.Select(stocksContent, NULL_ID);
	stocksContent.DestroyContent();
	return selectedStockId;
}

void CPurchaseIssuesSolverDlg::OnOK()
{
	try
	{
		CWaitDialog waitDlg(this, StringLoader(IDS_WAITSAVING));

		waitDlg.WaitForTask([this]()
		{
			m_transaction->Commit();
		});
				
		EndDialog(IDOK);
	}
	catch (TransactionException& e)
	{
		AfxMessageBox(e.wwhat());
	}
}

void CPurchaseIssuesSolverDlg::SolveCowIssue(SolveInvoker* invoker)
{
	CowNo cowNo = AskForCowNo();
	if (!cowNo.IsNull())
	{
		invoker->Solve(cowNo);
	}
}

CowNo CPurchaseIssuesSolverDlg::AskForCowNo()
{
	CInputCowNoDlg dlg(this);
	return dlg.ask();
}