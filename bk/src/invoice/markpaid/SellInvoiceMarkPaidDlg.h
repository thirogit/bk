#ifndef __SELLINVOICEMARKPAIDDLG_H__
#define __SELLINVOICEMARKPAIDDLG_H__

#include "InvoiceMarkPaidDlg.h"
#include "../../runtime/UniqueIds.h"


class CSellInvoiceMarkPaidDlg : public CInvoiceMarkPaidDlg
{	
public:
	CSellInvoiceMarkPaidDlg(const UniqueIds& invoiceIds,CWnd* pParent = NULL);
	virtual ~CSellInvoiceMarkPaidDlg();	
	
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