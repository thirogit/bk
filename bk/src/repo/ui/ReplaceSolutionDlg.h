#ifndef __PURCHASEISSUESSOLVERDLG_H__
#define __PURCHASEISSUESSOLVERDLG_H__

#include "../../ui/lazydlg/LazyDlg.h"
#include "../../context/transaction/DownloadPurchaseTransaction.h"
#include "../../resource.h"
#include "../../ctrls/contentlistctrl/ContentListCtrl.h"
#include "content/PurchaseInvoicesListContent.h"
#include "content/PurchaseCowListContent.h"
#include "ctrls/RepoPurchaseInvoicesListCtrl.h"
#include "ctrls/RepoPurchaseCowsListCtrl.h"
#include "ctrls\PurchaseIssuesListCtrl.h"
#include "content\PurchaseIssuesListContent.h"

class CPurchaseIssuesSolverDlg : public CLazyDlg<CDialog>
{
	DECLARE_DYNAMIC(CPurchaseIssuesSolverDlg)


	struct tag_Id{};

	struct RowKeyExtractor
	{
		typedef uint32_t result_type;
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

	CContentListCtrl m_DeductionsList;

	CPurchaseIssuesListCtrl m_IssuesListCtrl;
	PurchaseIssuesListContent m_IssuesListContent;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	virtual void OnAfterDlgShow();
	virtual void OnCancel();

	void OnIssuesListRClick(int row, uint32_t colId, const CellDescriptor& cell);
	void SolveByAdding(IPurchaseDownloadIssue* issue);
	void SolveByClearing(IPurchaseDownloadIssue* issue);
	void SolveByReplacing(IPurchaseDownloadIssue* issue);

	uint32_t SelectHerd();
	uint32_t SelectAgent();
	uint32_t SelectHent();
	uint32_t SelectClass();
	uint32_t SelectStock();
			
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()

public:
	
};

#endif 