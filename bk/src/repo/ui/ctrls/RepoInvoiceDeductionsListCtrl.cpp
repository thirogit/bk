#include "stdafx.h"
#include "RepoInvoiceDeductionsListCtrl.h"
#include "../view/PurchaseInvoiceDeductionView.h"
#include "../../../resource.h"
#include "../../../utils/StringLoader.h"

BEGIN_MESSAGE_MAP(CRepoInvoiceDeductionsListCtrl, CContentListCtrl)
	
END_MESSAGE_MAP()

CRepoInvoiceDeductionsListCtrl::CRepoInvoiceDeductionsListCtrl() 
{	
	ListHeaderDescriptor header;
	header(PurchaseInvoiceDeductionViewMember::Code, StringLoader(IDS_HEADER_DEDUCTIONCODE))
		(PurchaseInvoiceDeductionViewMember::Reason, StringLoader(IDS_HEADER_DEDUCTIONREASON))
		(PurchaseInvoiceDeductionViewMember::Fraction, StringLoader(IDS_HEADER_DEDUCTIONFRACTION))
		(PurchaseInvoiceDeductionViewMember::Enabled, StringLoader(IDS_HEADER_DEDUCTIONENABLED));
		

	SetHeaderDescriptor(header);
}
