#include "stdafx.h"
#include "SellInvoiceMarkPaidDlg.h"
#include <boost/foreach.hpp>
#include "../../ui/ExceptionMsgBox.h"

CSellInvoiceMarkPaidDlg::CSellInvoiceMarkPaidDlg(const UniqueIds& invoiceIds,CWnd* pParent/* = NULL*/) : CInvoiceMarkPaidDlg(pParent),m_invoiceIds(invoiceIds) 
{
}

CSellInvoiceMarkPaidDlg::~CSellInvoiceMarkPaidDlg()
{
}
	
BOOL CSellInvoiceMarkPaidDlg::OnInitDialog()
{
	CInvoiceMarkPaidDlg::OnInitDialog();
	CreateContent();
	return TRUE;
}

void CSellInvoiceMarkPaidDlg::CreateContent()
{	
	SeasonSession* session = GetSeasonSession();
	SellInvoiceClientObject* pInvoiceCO;
	
	BOOST_FOREACH(uint32_t invoiceId,m_invoiceIds)
	{
		pInvoiceCO = session->GetSellInvoice(invoiceId);
		AddInvoice(pInvoiceCO->Object());		
	}
}
	
void CSellInvoiceMarkPaidDlg::MarkInvoice(uint32_t invoiceId,const DateTime& paidDt)
{
	SeasonSession* session = GetSeasonSession();
	SellInvoiceClientObject* pSellInvoiceCO = session->GetSellInvoice(invoiceId);
	SellInvoice* pSellInvoice = pSellInvoiceCO->Object();
			
	ExceptionMsgBox msgbox;

	msgbox.TryCatchAny(	[this, pSellInvoice, &paidDt]
						{
							switch (pSellInvoice->GetInvoiceScope())
							{
							case InvoiceScope_Cows:
								MarkCowsInvoice(pSellInvoice->GetId(), paidDt);
								break;

							case InvoiceScope_Docs:
								MarkDocsInvoice(pSellInvoice->GetId(), paidDt);
								break;
							}

							SellInvoice* pClone = pSellInvoice->Clone();
							pClone->SetPaidDate(paidDt);
							UpdateInvoice(pClone);
							delete pClone;
						});
		
}

void CSellInvoiceMarkPaidDlg::MarkCowsInvoice(uint32_t invoiceId,const DateTime& paidDt)
{
	SeasonSession* session = GetSeasonSession();
	InvoiceCowsTransactionPtr transaction;

	try
	{
		transaction = session->EditSellInvoiceCows(invoiceId);
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

void CSellInvoiceMarkPaidDlg::MarkDocsInvoice(uint32_t invoiceId,const DateTime& paidDt)
{
	SeasonSession* session = GetSeasonSession();
	InvoiceDocsTransactionPtr transaction;
	
	try
	{
		transaction = session->EditSellInvoiceDocs(invoiceId);
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