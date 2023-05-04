#include "stdafx.h"
#include "DeductionElementViewCreator.h"
#include <string/TextUtils.h>

DeductionElementViewCreator::DeductionElementViewCreator(SeasonSession* facade) : m_pFacade(facade)
{
}

DeductionElementView* DeductionElementViewCreator::CreateView(const DeductionElement* pDeductionElement)
{
	DeductionElementView* pView = new DeductionElementView(pDeductionElement->GetId());
	RefreshView(pDeductionElement,pView);
	return pView;
}

void DeductionElementViewCreator::RefreshView(const DeductionElement* pDeductionElement,DeductionElementView* pView)
{
	const InvoiceDeduction& deduction = pDeductionElement->GetDeduction();
	const DeductionDetails& details = deduction.GetDetails();
	
	pView->SetAmount(pDeductionElement->GetDeductedAmount());
	pView->SetCode(details.GetCode());
	pView->SetFraction(details.GetFraction());
	pView->SetIsEnabled(deduction.GetIsEnabled());
	pView->SetReason(TextUtils::JoinLines(details.GetReason(), L';'));

}

