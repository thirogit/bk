#include "stdafx.h"
#include "PurchaseInvoiceDeductionViewCreator.h"

PurchaseInvoiceDeductionViewCreator::PurchaseInvoiceDeductionViewCreator()
{
}

PurchaseInvoiceDeductionView* PurchaseInvoiceDeductionViewCreator::CreateView(IPurchaseInvoiceDeduction* pDeduction)
{
	PurchaseInvoiceDeductionView* pView = new PurchaseInvoiceDeductionView(pDeduction->GetId());
	RefreshView(pView, pDeduction);
	return pView;
}

void PurchaseInvoiceDeductionViewCreator::RefreshView(PurchaseInvoiceDeductionView* pView, IPurchaseInvoiceDeduction* pDeduction)
{
	pView->SetCode(pDeduction->GetCode());
	pView->SetReason(pDeduction->GetReason());
	pView->SetFraction(pDeduction->GetFraction());
	pView->SetIsEnabled(pDeduction->GetIsEnabled());	
}

