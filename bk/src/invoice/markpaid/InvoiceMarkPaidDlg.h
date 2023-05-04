#ifndef __INVOICEMARKPAIDDLG_H__
#define __INVOICEMARKPAIDDLG_H__

#include "../../resource.h"
#include <afxwin.h>
#include "../../ctrls/contentlistctrl/ContentListCtrl.h"
#include "../../SessionAwareDialog.h"
#include "../../content/SimpleInvoicesListContent.h"
#include "../../view/InvoiceView.h"

class CInvoiceMarkPaidDlg : public CSessionAwareDialog
{	
public:
	enum { IDD = IDD_MARKPAID };	
	
	CInvoiceMarkPaidDlg(CWnd* pParent = NULL);
	virtual ~CInvoiceMarkPaidDlg();	
protected:
	virtual void MarkInvoice(uint32_t invoiceId,const DateTime& paidDt) = 0;
	void MarkSelectedInvoices(const DateTime& paidDt);
	
	void AddInvoice(IInvoice* pInvoice);
	void RemoveInvoice(uint32_t invoiceId);
	const InvoiceView* GetInvoice(uint32_t invoiceId) const;
	void UpdateInvoice(IInvoice* pInvoice);
	
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	
protected:
	SimpleInvoicesListContent* m_pContent;	
	CContentListCtrl m_invoiceList;		
private:
	
	afx_msg void OnMarkPaidNow();
	afx_msg void OnMarkPaidDate();
	afx_msg void OnMarkPaidNo();

	afx_msg void OnSelectAll();
	afx_msg void OnInvertSelection();
	afx_msg void  OnSelectNone();

	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP();
};

#endif