#include "stdafx.h"
#include "PurchaseInvoiceDeductionViewText.h"
#include "../../../text/YesNoText.h"

std::wstring PurchaseInvoiceDeductionViewText::Text(const PurchaseInvoiceDeductionView *pView, uint32_t memberId)
{
	std::wstring text;
	switch(memberId)
	{
		
	case PurchaseInvoiceDeductionViewMember::Code:
		text = pView->GetCode();
		break;
	case PurchaseInvoiceDeductionViewMember::Reason:
		text = pView->GetReason();
		break;
	case PurchaseInvoiceDeductionViewMember::Fraction:
		text = (pView->GetFraction() * Decimal(100L)).ToString(4) + L"%";
		break;
	case PurchaseInvoiceDeductionViewMember::Enabled:
		text = YesNoText(pView->GetIsEnabled()).ToString();
		break;		
	}	
	return text;
}
