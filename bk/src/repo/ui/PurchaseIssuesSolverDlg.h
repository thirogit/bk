#ifndef __PURCHASEISSUESSOLVERDLG_H__
#define __PURCHASEISSUESSOLVERDLG_H__

#include "../../ui/lazydlg/LazyDlg.h"
#include "../../SessionAwareDialog.h"
#include "../../context/transaction/DownloadPurchaseTransaction.h"
#include "../../resource.h"
#include "../../ctrls/contentlistctrl/ContentListCtrl.h"
#include "content/PurchaseInvoicesListContent.h"
#include "content/PurchaseCowListContent.h"
#include "ctrls/RepoPurchaseInvoicesListCtrl.h"
#include "ctrls/RepoPurchaseCowsListCtrl.h"
#include "ctrls\PurchaseIssuesListCtrl.h"
#include "content\PurchaseIssuesListContent.h"
#include "ctrls/RepoInvoiceDeductionsListCtrl.h"
#include "content/PurchaseInvoiceDeductionsListContent.h"

class CPurchaseIssuesSolverDlg : public CLazyDlg<CSessionAwareDialog>
{
	DECLARE_DYNAMIC(CPurchaseIssuesSolverDlg)


	struct tag_Id{};

	struct RowKeyExtractor
	{
		typedef std::wstring result_type;
		result_type operator()(PurchaseInvoiceRow* pRow) const
		{
			return pRow->Content()->GetInvoiceId();
		}
	};

	typedef boost::multi_index_container
		<
		PurchaseInvoiceRow*,
		boost::multi_index::indexed_by<
		boost::multi_index::random_access<>,
		boost::multi_index::ordered_unique < boost::multi_index::tag<tag_Id>, RowKeyExtractor >
		>
		> RowIndex;


	class SolveInvoker
	{
	public:
		SolveInvoker(SolutionType type, IPurchaseDownloadIssue* issue) : m_issue(issue), m_type(type)
		{
		}

		template<typename ARG, typename std::enable_if<std::is_pod<ARG>::value, int>::type = 0> void Solve(ARG solutionValue) 
		{
			Invoke(SolutionParameters::make<ARG>(m_type, solutionValue));
		}

		template<typename ARG, typename std::enable_if<!std::is_pod<ARG>::value, int>::type = 0> void Solve(ARG& solutionValue)
		{			
			Invoke(SolutionParameters::make<ARG>(m_type, solutionValue));
		}
	protected:
		virtual void Invoke(const SolutionParameters& right) = 0;

	protected:
		SolutionType m_type;
		IPurchaseDownloadIssue* m_issue;
	};

	class SolveSingleInvoker : public SolveInvoker
	{
	public:
		SolveSingleInvoker(SolutionType type, IPurchaseDownloadIssue* issue,DownloadPurchaseTransactionPtr transaction);
		virtual void Invoke(const SolutionParameters& right);
	private:
		DownloadPurchaseTransactionPtr m_transaction;
	};

	class SolveCategoryInvoker : public SolveInvoker
	{
	public:
		SolveCategoryInvoker(SolutionType type, IPurchaseDownloadIssue* issue,DownloadPurchaseTransactionPtr transaction);
		virtual void Invoke(const SolutionParameters& right);
	private:
		DownloadPurchaseTransactionPtr m_transaction;
	};

public:
	CPurchaseIssuesSolverDlg(DownloadPurchaseTransactionPtr transaction,CWnd* pParent = NULL);
	virtual ~CPurchaseIssuesSolverDlg();	
	enum { IDD = IDD_REPO_PURCHASE_ISSUES };

private:

	DownloadPurchaseTransactionPtr m_transaction;

	CRepoPurchaseInvoicesListCtrl m_InvoiceList;
	PurchaseInvoicesListContent m_InvoiceListContent;

	CRepoPurchaseCowsListCtrl m_CowList;
	PurchaseCowListContent m_CowListContent;

	CRepoInvoiceDeductionsListCtrl m_DeductionsList;
	PurchaseInvoiceDeductionsListContent m_DeductionListContent;

	CPurchaseIssuesListCtrl m_IssuesListCtrl;
	PurchaseIssuesListContent m_IssuesListContent;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	virtual void OnAfterDlgShow();
	virtual void OnCancel();
	virtual void OnOK();


	void UpdateCowListFrom(IPurchaseInvoice* invoice);
	void UpdateDeductionListFrom(IPurchaseInvoice* invoice);
	void OnIssuesListRClick(int row, uint32_t colId, const CellDescriptor& cell);
	
	void SolveByClearing(IPurchaseDownloadIssue* issue);
	void SolveByAdding(IPurchaseDownloadIssue* issue);
	void SolveByAddingAllHents();
	void SolveByReplacingSingle(IPurchaseDownloadIssue* issue);
	void SolveByReplacingAll(IPurchaseDownloadIssue* issue);

	void SolveByReplacing(PurchaseDownloadIssueDomain domain, SolveInvoker* invoker);
	
	void SolveHentIssueByReplacing(SolveInvoker* invoker);
	void SolveHentIssueByAdding(IPurchaseDownloadIssue* issue);
	void SolveHerdIssue(SolveInvoker* invoker);
	void SolveAgentIssue(SolveInvoker* invoker);
	void SolveClassIssue(SolveInvoker* invoker);
	void SolveStockIssue(SolveInvoker* invoker);
	void SolveCowIssue(SolveInvoker* invoker);
	void RefreshIssuesList();
	uint32_t SelectHerd();
	uint32_t SelectAgent();
	uint32_t SelectHent();
	uint32_t SelectClass();
	uint32_t SelectStock();
	CowNo AskForCowNo();
			
	afx_msg void OnInvoiceClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()

public:
	
};

#endif 