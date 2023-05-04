#include "stdafx.h"
#include "GetDocInvoice.h"


#include "../IBuyDoc.h"
#include "../ISellDoc.h"


uint32_t GetDocInvoice(const IDoc* pDoc)
{
	switch(pDoc->GetDocType())
	{
		case DocType_Buy:
			return ((IBuyDoc*)pDoc)->GetInvoiceId();
		case DocType_Sell:
			return ((ISellDoc*)pDoc)->GetInvoiceId();
	}
	
	return NULL_ID;
}