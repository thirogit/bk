#ifndef __SELLINVOICECOWSCOPEDLG_H__
#define __SELLINVOICECOWSCOPEDLG_H__

#include "SellInvoiceDlg.h"
#include "../context/transaction/InvoiceCowsTransaction.h"
#include "content\SimpleInDocsListContent.h"

class CSellInvoiceCowScopeDlg : public CSellInvoiceDlg
{	
public:
	CSellInvoiceCowScopeDlg(InvoiceCowsTransactionPtr transaction,CWnd* pParent = NULL);   
	virtual ~CSellInvoiceCowScopeDlg();
protected:
	virtual void SetupControls();
	virtual void AddItem();
	virtual void DelItem();
	virtual void SaveInvoice();
	virtual std::wstring GetCaption();
private:
	InvoiceCowsTransactionPtr m_transaction;
	SimpleInDocsListContent* m_pDocsListContent;
	
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP();
};

#endif