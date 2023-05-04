#include "stdafx.h"
#include "DocumentInputUtils.h"
#include "../DocumentInput.h"



namespace DocumentInputUtils
{
	
	uint32_t GetDocumentInputMaskForInvoice(const IInvoice* invoice)
	{
		return GetDocumentInputMaskForInvoice(InvoiceKind(invoice->GetInvoiceDirection(), invoice->GetInvoiceType()));
	}

	uint32_t GetDocumentInputMaskForInvoice(const InvoiceKind& kind)
	{
		switch (kind.GetDirection())
		{
		case InvoiceDirection_Buy:
			return GetDocumentInputMaskForBuyInvoice(kind.GetInvoiceType());
		case InvoiceDirection_Sell:
			return GetDocumentInputMaskForSellInvoice(kind.GetInvoiceType());
		}

		return DocumentInput_None;
	}


	uint32_t GetDocumentInputMaskForBuyInvoice(InvoiceType type)
	{
		switch (type)
		{
		case InvoiceType_Regular:
			return (DocumentInput_BuyInvoiceRegular);
		case InvoiceType_Lump:
			return (DocumentInput_BuyInvoiceLump);
		}

		return DocumentInput_None;
	}

	uint32_t GetDocumentInputMaskForSellInvoice(InvoiceType type)
	{
		switch (type)
		{
		case InvoiceType_Regular:
			return (DocumentInput_SellInvoiceRegular);
		case InvoiceType_Lump:
			return (DocumentInput_SellInvoiceLump);
		}

		return DocumentInput_None;
	}
};
