#include "stdafx.h"
#include "InvoiceListHeader.h"
#include "../view/InvoiceView.h"
#include "../utils/StringLoader.h"
#include "../resource.h"

InvoiceListHeader::InvoiceListHeader()
{
	operator()	(	InvoiceViewMember::Number,					StringLoader(IDS_HEADER_INVOICENO))
				(	InvoiceViewMember::Date,					StringLoader(IDS_HEADER_INVOICEDATE))					
				(	InvoiceViewMember::Hent,					StringLoader(IDS_HEADER_INVOICEHENT))
				(	InvoiceViewMember::CowQty,					StringLoader(IDS_HEADER_COWQTY))
				(	InvoiceViewMember::VatRate,					StringLoader(IDS_HEADER_VATRATE))
				(	InvoiceViewMember::PayWay,					StringLoader(IDS_HEADER_PAYWAY))
				(	InvoiceViewMember::CustomNumber,				StringLoader(IDS_HEADER_CUSTOMNUMBER))
				(	InvoiceViewMember::TotalGrossValue,				StringLoader(IDS_HEADER_TOTALGROSS))
				(	InvoiceViewMember::TotalGrossAfterDeductions,	StringLoader(IDS_HEADER_TOTALGROSSAFTERDEDUCTIONS))
				(	InvoiceViewMember::TotalNetValue,				StringLoader(IDS_HEADER_TOTALNET))
				(	InvoiceViewMember::TotalTaxValue,				StringLoader(IDS_HEADER_TOTALTAX))
				(	InvoiceViewMember::PaidDate,					StringLoader(IDS_HEADER_PAID))
				(	InvoiceViewMember::Extras,						StringLoader(IDS_HEADER_EXTRAS))
				(	InvoiceViewMember::PayDueDays,					StringLoader(IDS_HEADER_PAYDUEDAYS))
				(	InvoiceViewMember::PayDueDate,					StringLoader(IDS_HEADER_PAYDUEDATE))
				(	InvoiceViewMember::Type,						StringLoader(IDS_HEADER_INVOICETYPE))
				(	InvoiceViewMember::TransactionPlace,			StringLoader(IDS_HEADER_TRANSACTIONPLACE))
				(	InvoiceViewMember::TransactionDate,			StringLoader(IDS_HEADER_TRANSACTIONDATE))
				//(	InvoiceViewMember::CorrectFor,		StringLoader(IDS_HEADER_CORRECTFORNO))
				;	

}
