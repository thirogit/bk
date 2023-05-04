#ifndef __REPOPURCHASECONTENTPAGE_H__
#define __REPOPURCHASECONTENTPAGE_H__

#include "../../ui/LayoutManager/ETSLayout.h"
#include "../../resource.h"
#include <data\repo\IPurchase.h>
#include "../../ctrls/contentlistctrl/ContentListCtrl.h"
#include "content/PurchaseInvoicesListContent.h"
#include "content/PurchaseCowListContent.h"
#include "content/PurchaseInvoiceDeductionsListContent.h"
#include "ctrls/RepoPurchaseInvoicesListCtrl.h"
#include "ctrls/RepoPurchaseCowsListCtrl.h"
#include "ctrls/RepoInvoiceDeductionsListCtrl.h"

class CRepoPurchaseContentPage : public ETSLayoutDialog
{
	DECLARE_DYNAMIC(CRepoPurchaseContentPage)

public:
	CRepoPurchaseContentPage();
	virtual ~CRepoPurchaseContentPage();
	enum { IDD = IDD_REPO_PURCHASE_CONTENT };

	void UpdateFrom(IPurchase* pPurchase);
private:
	IPurchase* m_pPurchase;

	CRepoPurchaseInvoicesListCtrl m_InvoiceList;
	PurchaseInvoicesListContent m_InvoiceListContent;

	CRepoPurchaseCowsListCtrl m_CowList;
	PurchaseCowListContent m_CowListContent;
	
	CRepoInvoiceDeductionsListCtrl m_DeductionsList;
	PurchaseInvoiceDeductionsListContent m_DeductionListContent;

private:
	void UpdateCowListFrom(IPurchaseInvoice* invoice);
	void UpdateDeductionListFrom(IPurchaseInvoice* invoice);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	
	afx_msg void OnInvoiceClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnable(BOOL bEnable);
	
	DECLARE_MESSAGE_MAP()

};

#endif