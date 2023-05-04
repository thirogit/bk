#include "stdafx.h"
#include "PurchaseInvoiceViewCreator.h"
#include "../PurchaseInvoiceCalculator.h"

PurchaseInvoiceViewCreator::PurchaseInvoiceViewCreator()
{
}

PurchaseInvoiceView* PurchaseInvoiceViewCreator::CreateView(IPurchaseInvoice* pPurchaseInvoice)
{
	PurchaseInvoiceView* pView = new PurchaseInvoiceView(pPurchaseInvoice->GetId());
	RefreshView(pView, pPurchaseInvoice);
	return pView;
}

void PurchaseInvoiceViewCreator::RefreshView(PurchaseInvoiceView* pView,IPurchaseInvoice* invoice)
{
	PurchaseInvoiceCalculator calculator(invoice);
	pView->SetVatRate(invoice->GetVatRate());
	pView->SetPayWay(invoice->GetPayWay());

	PurchaseHentView hent(L"");
	m_hentViewCreator.RefreshView(&hent, invoice->GetInvoiceHent());

	pView->SetInvoiceHent(&hent);
	pView->SetInvoiceType(invoice->GetInvoiceType());
	pView->SetCowCount(invoice->GetCowCount());
	pView->SetTotalNet(calculator.GetTotalNet());

	pView->SetCustomNumber(invoice->GetCustomNumber());
	pView->SetInvoiceType(invoice->GetInvoiceType());	
	pView->SetInvoiceDate(invoice->GetInvoiceDate());

	
	
}

