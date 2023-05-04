#include "stdafx.h"
#include "BuyInvoicesListContent.h"
#include "../view/BuyInvoiceViewCreator.h"
#include "../uiconstants/ColorConstants.h"

void BuyInvoicesListContent::CreateContent()
{
	PtrEnumerator<BuyInvoiceClientObject> invoiceEnum = m_pSession->EnumBuyInvoices();

	BuyInvoiceClientObject* invoiceCO;
	BuyInvoice* invoice;
	InvoiceView* view;
	InvoiceRow* row;

	BuyInvoiceViewCreator viewCreator(m_pSession);
	while(invoiceEnum.hasNext())
	{
		invoiceCO = *invoiceEnum;
		invoice = invoiceCO->Object();

		if (IsValidForThisContent(invoice))
		{
			view = viewCreator.CreateView(invoice);
			row = new InvoiceRow(view);

			if (invoiceCO->IsLocked())
				row->SetRowColor(Color(LOCKED_ROW_COLOR));

			AddRow(row);
		}
		invoiceEnum.advance();
	}

}

void BuyInvoicesListContent::OnBuyInvoiceInserted(uint32_t invoiceId)
{
	BuyInvoiceViewCreator viewCreator(m_pSession);
	BuyInvoiceClientObject* pBuyInvoiceCO = m_pSession->GetBuyInvoice(invoiceId);
	BuyInvoice* invoice = pBuyInvoiceCO->Object();
	if (IsValidForThisContent(invoice))
	{
		InvoiceView* pView = viewCreator.CreateView(invoice);
		AddRow(new InvoiceRow(pView));
		NotifyCountChanged();
	}
}

void BuyInvoicesListContent::OnBuyInvoiceUpdated(uint32_t invoiceId)
{
	BuyInvoiceViewCreator viewCreator(m_pSession);
	BuyInvoiceClientObject* pBuyInvoiceCO = m_pSession->GetBuyInvoice(invoiceId);
	InvoiceRow* pRow = m_Rows.find_row(invoiceId);
	if (pRow != NULL)
	{
		viewCreator.RefreshView(pRow->Content(), pBuyInvoiceCO->Object());
		NotifyContentUpdated();
	}
}

void BuyInvoicesListContent::OnBuyInvoiceDeleted(uint32_t invoiceId)
{
	RemoveRow(invoiceId);
	NotifyCountChanged();
}

void BuyInvoicesListContent::OnBuyInvoiceLocked(uint32_t invoiceId)
{
	InvoiceRow* row = FindRow(invoiceId);
	if (row != NULL)
	{
		row->SetRowColor(Color(LOCKED_ROW_COLOR));
		NotifyContentUpdated();
	}
}

void BuyInvoicesListContent::OnBuyInvoiceUnlocked(uint32_t invoiceId)
{
	InvoiceRow* row = FindRow(invoiceId);
	if (row != NULL)
	{
		row->SetRowColor(Color());
		NotifyContentUpdated();
	}
}