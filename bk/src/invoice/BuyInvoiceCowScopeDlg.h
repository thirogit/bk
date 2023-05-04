#ifndef __BUYINVOICECOWSCOPEDLG_H__
#define __BUYINVOICECOWSCOPEDLG_H__

#include "BuyInvoiceDlg.h"
#include "../context/transaction/InvoiceCowsTransaction.h"
#include "content\SimpleInDocsListContent.h"

class CBuyInvoiceCowScopeDlg : public CBuyInvoiceDlg
{	
public:
	CBuyInvoiceCowScopeDlg(InvoiceCowsTransactionPtr transaction,CWnd* pParent = NULL);   
	virtual ~CBuyInvoiceCowScopeDlg();
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