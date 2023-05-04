#include "stdafx.h"
#include "RepoPurchaseInvoicesListCtrl.h"
#include "../../../utils/StringLoader.h"
#include "../../../resource.h"
#include "../view/PurchaseInvoiceView.h"

CRepoPurchaseInvoicesListCtrl::CRepoPurchaseInvoicesListCtrl() 
{	
	ListHeaderDescriptor invoiceListHeader;
	invoiceListHeader(PurchaseInvoiceViewMember::CustomNumber, StringLoader(IDS_HEADER_CUSTOMNUMBER))
		(PurchaseInvoiceViewMember::InvoiceDate, StringLoader(IDS_HEADER_INVOICEDATE))
		(PurchaseInvoiceViewMember::InvoiceType, StringLoader(IDS_HEADER_INVOICETYPE))
		(PurchaseInvoiceViewMember::PayWay, 	StringLoader(IDS_HEADER_PAYWAY))
		(PurchaseInvoiceViewMember::Hent, 		StringLoader(IDS_HEADER_INVOICEHENT))
		(PurchaseInvoiceViewMember::CowCount, 	StringLoader(IDS_HEADER_COWQTY))
		(PurchaseInvoiceViewMember::VatRate, 	StringLoader(IDS_HEADER_VATRATE))
		(PurchaseInvoiceViewMember::TotalNet, 	StringLoader(IDS_HEADER_TOTALNET));

	SetHeaderDescriptor(invoiceListHeader);			
}
