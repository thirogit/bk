#ifndef __DOCUMENTINPUTUTILS_H__
#define __DOCUMENTINPUTUTILS_H__

#include <stdint.h>
#include <data\InvoiceType.h>
#include <data\IInvoice.h>
#include <data\InvoiceKind.h>

namespace DocumentInputUtils
{
	uint32_t GetDocumentInputMaskForBuyInvoice(InvoiceType type);
	uint32_t GetDocumentInputMaskForSellInvoice(InvoiceType type);
	uint32_t GetDocumentInputMaskForInvoice(const InvoiceKind& kind);
	uint32_t GetDocumentInputMaskForInvoice(const IInvoice* invoice);
}

#endif