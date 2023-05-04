#ifndef __SELLINVOICEDOCSCOPEDLG_H__
#define __SELLINVOICEDOCSCOPEDLG_H__

#include "SellInvoiceDlg.h"
#include "../context/transaction/InvoiceDocsTransaction.h"

#include "content\SimpleHentsDocsListContent.h"

class CSellInvoiceDocScopeDlg : public CSellInvoiceDlg
{	
public:
	CSellInvoiceDocScopeDlg(InvoiceDocsTransactionPtr transaction,CWnd* pParent = NULL);   
	virtual ~CSellInvoiceDocScopeDlg();
protected:
	virtual void SetupControls();
	virtual void AddItem();
	virtual void DelItem();
	virtual void SaveInvoice();
	void RefreshDocsList();
	virtual std::wstring GetCaption();
private:
	InvoiceDocsTransactionPtr m_transaction;
	SimpleHentsDocsListContent* m_pDocsListContent;
	
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP();
};

#endif