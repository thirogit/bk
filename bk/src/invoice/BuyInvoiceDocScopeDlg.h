#ifndef __BUYINVOICEDOCSCOPEDLG_H__
#define __BUYINVOICEDOCSCOPEDLG_H__

#include "BuyInvoiceDlg.h"
#include "../context/transaction/InvoiceDocsTransaction.h"
#include "content\SimpleHentsDocsListContent.h"

class CBuyInvoiceDocScopeDlg : public CBuyInvoiceDlg
{	
public:
	CBuyInvoiceDocScopeDlg(InvoiceDocsTransactionPtr transaction,CWnd* pParent = NULL);   
	virtual ~CBuyInvoiceDocScopeDlg();
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