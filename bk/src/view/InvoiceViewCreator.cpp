#include "stdafx.h"
#include "InvoiceViewCreator.h"
#include <utils/SafeGet.h>
#include "../text/InvoiceNoText.h"
#include "../invoice/calculator/InvoiceCalculator.h"

InvoiceViewCreator::InvoiceViewCreator(SeasonSession* facade) : m_pFacade(facade)
{
}

InvoiceView* InvoiceViewCreator::CreateView(IInvoice* pInvoice)
{
	InvoiceView* pView = new InvoiceView(pInvoice->GetId());
	RefreshView(pView,pInvoice);	
	return pView;
}

void InvoiceViewCreator::RefreshView(InvoiceView* pView,IInvoice* pInvoice)
{
	pView->SetInvoiceNo((InvoiceNoText(pInvoice).ToString()));

	//IInvoice* pCorrectFor = NULL;
	//uint32_t correctForId = pInvoice->GetCorrectFor();
	//if(correctForId != NULL_ID)
	//	m_pFacade->get
	//pView->SetCorrectFor(const std::string& correctForNo);
	
	pView->SetInvoiceType(pInvoice->GetInvoiceType());
	pView->SetExtras(pInvoice->GetExtras());
	pView->SetInvoiceDate(pInvoice->GetInvoiceDate());
	pView->SetPaidDate(pInvoice->GetPaidDate());

	InvoiceCalculator::CalculationResult result = InvoiceCalculator::Calculate(pInvoice);
	
	pView->SetTotalGrossValue(result.GetGrossValue());
	pView->SetTotalNetValue(result.GetNetValue());
	pView->SetTotalTaxValue(result.GetTaxValue());
	pView->SetDeductedAmount(result.GetDeductedAmount());
	pView->SetGrossAfterDeductions(result.GetGrossAfterDeductionsValue());
	pView->SetVatRate(pInvoice->GetVATRate());
	pView->SetPayWay(pInvoice->GetPayWay());
	pView->SetCustomNumber(pInvoice->GetCustomNumber());
	pView->SetTransactionPlace(pInvoice->GetTransactionPlace());
	pView->SetTransactionDate(pInvoice->GetTransactionDate());
	pView->SetPayDueDays(pInvoice->GetPayDueDays());

	pView->SetCowQty(pInvoice->GetCowQty());
		
	pView->SetInvoiceHent(pInvoice->GetInvoiceHent()->GetName());

	int payDueDays = pInvoice->GetPayDueDays();	
	pView->SetPayDueDate(pInvoice->GetInvoiceDate().AddDays(payDueDays > 0 ? (payDueDays - 1) : payDueDays));

	pView->SetTotalWeight(result.GetWeight());

}

