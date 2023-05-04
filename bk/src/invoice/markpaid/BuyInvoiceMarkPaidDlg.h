#ifndef __BUYINVOICEMARKPAIDDLG_H__
#define __BUYINVOICEMARKPAIDDLG_H__

#include "InvoiceMarkPaidDlg.h"
#include "../../runtime/UniqueIds.h"


class CBuyInvoiceMarkPaidDlg : public CInvoiceMarkPaidDlg
{	
public:
	CBuyInvoiceMarkPaidDlg(const UniqueIds& invoiceIds,CWnd* pParent = NULL);
	virtual ~CBuyInvoiceMarkPaidDlg();	
	
protected:
	virtual BOOL OnInitDialog();

	void CreateContent();

	virtual void MarkInvoice(uint32_t invoiceId,const DateTime& paidDt);

	virtual void MarkCowsInvoice(uint32_t invoiceId,const DateTime& paidDt);
	virtual void MarkDocsInvoice(uint32_t invoiceId,const DateTime& paidDt);

private:
	UniqueIds m_invoiceIds;
};

#endif