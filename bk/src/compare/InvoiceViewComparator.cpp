#include "stdafx.h"
#include "InvoiceViewComparator.h"
#include <compare/TypesCompare.h>
#include <assert.h>

using namespace TypesCompare;

COMPARE_RESULT InvoiceViewComparator::CompareBy(uint32_t dataMember,const InvoiceView* DataLeft,const InvoiceView* DataRight) const
{
	const InvoiceView *left = DataLeft,*right = DataRight;
		
	switch(dataMember)
	{
		default:
			ASSERT(0);		
			
		case InvoiceViewMember::Number:
			return CompareLocaleString(left->GetInvoiceNo(), right->GetInvoiceNo());
		case InvoiceViewMember::Date:
			return CompareDateTime(left->GetInvoiceDate(), right->GetInvoiceDate());
		case InvoiceViewMember::Hent:
			return CompareLocaleString(left->GetInvoiceHent(), right->GetInvoiceHent());
		case InvoiceViewMember::CowQty:
			return CompareInt(left->GetCowQty(), right->GetCowQty());			
		case InvoiceViewMember::VatRate:
			return CompareType<Decimal>(left->GetVatRate(), right->GetVatRate());		
		case InvoiceViewMember::PayWay:
			return CompareType<PayWay>(left->GetPayWay(), right->GetPayWay());		
		case InvoiceViewMember::CustomNumber:
			return CompareLocaleString(left->GetCustomNumber(), right->GetCustomNumber());
		case InvoiceViewMember::TotalGrossValue:
			return CompareType<Decimal>(left->GetTotalGrossValue(), right->GetTotalGrossValue());
		case InvoiceViewMember::DeductedAmount:
			return CompareType<Decimal>(left->GetDeductedAmount(), right->GetDeductedAmount());
		case InvoiceViewMember::TotalGrossAfterDeductions:
			return CompareType<Decimal>(left->GetGrossAfterDeductions(), right->GetGrossAfterDeductions());
		case InvoiceViewMember::TotalNetValue:
			return CompareType<Decimal>(left->GetTotalNetValue(), right->GetTotalNetValue());
		case InvoiceViewMember::TotalTaxValue:
			return CompareType<Decimal>(left->GetTotalTaxValue(), right->GetTotalTaxValue());
		case InvoiceViewMember::PaidDate:
			return CompareDateTime(left->GetPaidDate(), right->GetPaidDate());
		case InvoiceViewMember::Extras:
			return CompareLocaleString(left->GetExtras(), right->GetExtras());
		case InvoiceViewMember::PayDueDays:
			return CompareInt(left->GetPayDueDays(), right->GetPayDueDays());
		case InvoiceViewMember::PayDueDate:
			return CompareDateTime(left->GetPayDueDate(), right->GetPayDueDate());
		case InvoiceViewMember::Type:
			return CompareType<InvoiceType>(left->GetInvoiceType(), right->GetInvoiceType());
		case InvoiceViewMember::TransactionPlace:
			return CompareLocaleString(left->GetTransactionPlace(), right->GetTransactionPlace());
		case InvoiceViewMember::TransactionDate:
			return CompareDateTime(left->GetTransactionDate(), right->GetTransactionDate());
		case InvoiceViewMember::TotalWeight:
			return CompareType<Decimal>(left->GetTotalWeight(), right->GetTotalWeight());	
	}

	ASSERT(0);
	return COMPARE_LESS;
}
