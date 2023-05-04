#include "stdafx.h"
#include "PurchaseInvoiceViewText.h"
#include <string/IntString.h>
#include "../../../text/PayWayString.h"
#include "../../../text/InvoiceTypeString.h"
#include "../../../uiconstants/PrecisionContstants.h"

std::wstring PurchaseInvoiceViewText::Text(const PurchaseInvoiceView *pView, uint32_t memberId)
{
	std::wstring text;
	switch(memberId)
	{
		
		case PurchaseInvoiceViewMember::InvoiceDate:
			text = pView->GetInvoiceDate().GetDayDate();
		break;		
		case PurchaseInvoiceViewMember::Hent:
			text = pView->GetInvoiceHent()->GetName();
		break;
		case PurchaseInvoiceViewMember::CowCount:
			text = IntString(pView->GetCowCount());
		break;
		case PurchaseInvoiceViewMember::VatRate:
			{
				Decimal percentVatRate = pView->GetVatRate()*Decimal(100L);
				text = percentVatRate.ToString(1);
			}
		break;
		case PurchaseInvoiceViewMember::PayWay:
			text = PayWayString(pView->GetPayWay()).ToString();
		break;
		case PurchaseInvoiceViewMember::CustomNumber:
			text = pView->GetCustomNumber();
		break;
	
		case PurchaseInvoiceViewMember::TotalNet:
			text = pView->GetTotalNet().ToString(PRICE_PREC);
		break;
		case PurchaseInvoiceViewMember::InvoiceType:
			text = InvoiceTypeString(pView->GetInvoiceType()).ToString();
		break;		
	}	
	return text;
}
