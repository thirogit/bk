#include "stdafx.h"
#include "BuyInvoiceMarkPaidDlg.h"
#include <boost/foreach.hpp>
#include "../../ui/ExceptionMsgBox.h"

CBuyInvoiceMarkPaidDlg::CBuyInvoiceMarkPaidDlg(const UniqueIds& invoiceIds,CWnd* pParent/* = NULL*/) : CInvoiceMarkPaidDlg(pParent),m_invoiceIds(invoiceIds) 
{
}

CBuyInvoiceMarkPaidDlg::~CBuyInvoiceMarkPaidDlg()
{
}
	
BOOL CBuyInvoiceMarkPaidDlg::OnInitDialog()
{
	CInvoiceMarkPaidDlg::OnInitDialog();

	

	CreateContent();

	return TRUE;
}

void CBuyInvoiceMarkPaidDlg::CreateContent()
{	
	SeasonSession* session = GetSeasonSession();
	BuyInvoiceClientObject* pBuyInvoiceCO;
	
	BOOST_FOREACH(uint32_t invoiceId,m_invoiceIds)
	{
		pBuyInvoiceCO = session->GetBuyInvoice(invoiceId);
		AddInvoice(pBuyInvoiceCO->Object());		
	}
}
	
void CBuyInvoiceMarkPaidDlg::MarkInvoice(uint32_t invoiceId,const DateTime& paidDt)
{
	SeasonSession* session = GetSeasonSession();
	BuyInvoiceClientObject* pBuyInvoiceCO = session->GetBuyInvoice(invoiceId);
	BuyInvoice* pBuyInvoice = pBuyInvoiceCO->Object();
			
	ExceptionMsgBox msgbox;

	msgbox.TryCatchAny(	[this, pBuyInvoice, &paidDt]
						{
							switch (pBuyInvoice->GetInvoiceScope())
							{
							case InvoiceScope_Cows:
								MarkCowsInvoice(pBuyInvoice->GetId(), paidDt);
								break;

							case InvoiceScope_Docs:
								MarkDocsInvoice(pBuyInvoice->GetId(), paidDt);
								break;
							}

							BuyInvoice* pClone = pBuyInvoice->Clone();
							pClone->SetPaidDate(paidDt);
							UpdateInvoice(pClone);
							delete pClone;
						});
		
}

void CBuyInvoiceMarkPaidDlg::MarkCowsInvoice(uint32_t invoiceId,const DateTime& paidDt)
{
	SeasonSession* session = GetSeasonSession();
	InvoiceCowsTransactionPtr transaction;

	try
	{
		transaction = session->EditBuyInvoiceCows(invoiceId);
		InvoiceDetails details = transaction->GetDetails();
		details.SetPaidDate(paidDt);
		transaction->UpdateDetails(details);
		transaction->Commit();
	}
	catch (...)
	{
		if (transaction)
			transaction->Rollback();

		std::rethrow_exception(std::current_exception());
	}
}

void CBuyInvoiceMarkPaidDlg::MarkDocsInvoice(uint32_t invoiceId,const DateTime& paidDt)
{
	SeasonSession* session = GetSeasonSession();
	InvoiceDocsTransactionPtr transaction;
	
	try
	{
		transaction = session->EditBuyInvoiceDocs(invoiceId);
		InvoiceDetails details = transaction->GetDetails();
		details.SetPaidDate(paidDt);
		transaction->UpdateDetails(details);
		transaction->Commit();
	}
	catch (...)
	{
		if (transaction)
			transaction->Rollback();

		std::rethrow_exception(std::current_exception());
	}
}