#include "stdafx.h"
#include "InvoiceViewText.h"
#include "InvoiceNoText.h"
#include <string/IntString.h>
#include "../text/PayWayString.h"
#include "../text/InvoiceTypeString.h"
#include "../uiconstants/PrecisionContstants.h"

std::wstring InvoiceViewText::Text(const InvoiceView *pView,uint32_t memberId)
{
	std::wstring text;
	switch(memberId)
	{
		case InvoiceViewMember::Number:
			text = pView->GetInvoiceNo();
		break;
		case InvoiceViewMember::Date:
			text = pView->GetInvoiceDate().GetDayDate();
		break;
		case InvoiceViewMember::CorrectFor:
			text = pView->GetCorrectFor();
		break;
		case InvoiceViewMember::Hent:
			text = pView->GetInvoiceHent();
		break;
		case InvoiceViewMember::CowQty:
			text = IntString(pView->GetCowQty());
		break;
		case InvoiceViewMember::VatRate:
			{
				Decimal percentVatRate = pView->GetVatRate()*Decimal(100L);
				text = percentVatRate.ToString(1);
			}
		break;
		case InvoiceViewMember::PayWay:
			text = PayWayString(pView->GetPayWay()).ToString();
		break;
		case InvoiceViewMember::CustomNumber:
			text = pView->GetCustomNumber();
		break;
		case InvoiceViewMember::TotalGrossValue:
			text = pView->GetTotalGrossValue().ToString(PRICE_PREC);
		break;
		case InvoiceViewMember::TotalNetValue:
			text = pView->GetTotalNetValue().ToString(PRICE_PREC);
		break;
		case InvoiceViewMember::TotalTaxValue:
			text = pView->GetTotalTaxValue().ToString(PRICE_PREC);
		break;
		case InvoiceViewMember::PaidDate:
			text = pView->GetPaidDate().GetDayDate();
		break;
		case InvoiceViewMember::Extras:
			text = pView->GetExtras();
		break;
		case InvoiceViewMember::PayDueDays:
			text = IntString(pView->GetPayDueDays());
		break;
		case InvoiceViewMember::PayDueDate:
			text = pView->GetPayDueDate().GetDayDate();
		break;
		case InvoiceViewMember::Type:
			text = InvoiceTypeString(pView->GetInvoiceType()).ToString();
		break;
		case InvoiceViewMember::TransactionPlace:
			text = pView->GetTransactionPlace();
		break;
		case InvoiceViewMember::TransactionDate:
			text = pView->GetTransactionDate().GetDayDate();
		break;
		case InvoiceViewMember::TotalGrossAfterDeductions:
			text = pView->GetGrossAfterDeductions().ToString(PRICE_PREC);
	}	
	return text;
}
