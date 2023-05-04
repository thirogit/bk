#include "stdafx.h"
#include "SellInvoicesListContent.h"
#include "../view/SellInvoiceViewCreator.h"
#include "../uiconstants/ColorConstants.h"

void SellInvoicesListContent::CreateContent()
{
	PtrEnumerator<SellInvoiceClientObject> invoiceEnum = m_pSession->EnumSellInvoices();

	SellInvoiceClientObject* invoiceCO;
	SellInvoice* invoice;
	InvoiceView* view;
	InvoiceRow* row;

	SellInvoiceViewCreator viewCreator(m_pSession);
	while(invoiceEnum.hasNext())
	{
		invoiceCO = *invoiceEnum;
		invoice = invoiceCO->Object();
		view = viewCreator.CreateView(invoice);
		row = new InvoiceRow(view);

		if(invoiceCO->IsLocked()) 
			row->SetRowColor(Color(LOCKED_ROW_COLOR));

		AddRow(row);
		invoiceEnum.advance();
	}
}


void SellInvoicesListContent::OnSellInvoiceInserted(uint32_t invoiceId)
{
	SellInvoiceViewCreator viewCreator(m_pSession);
	SellInvoiceClientObject* pInvoiceCO = m_pSession->GetSellInvoice(invoiceId);
	SellInvoice* invoice = pInvoiceCO->Object();

	if (IsValidForThisContent(invoice))
	{
		InvoiceView* pView = viewCreator.CreateView(pInvoiceCO->Object());
		AddRow(new InvoiceRow(pView));
		NotifyCountChanged();
	}
}

void SellInvoicesListContent::OnSellInvoiceUpdated(uint32_t invoiceId)
{
	SellInvoiceViewCreator viewCreator(m_pSession);
	SellInvoiceClientObject* pInvoiceCO = m_pSession->GetSellInvoice(invoiceId);
	SellInvoice* invoice = pInvoiceCO->Object();

	if (IsValidForThisContent(invoice))
	{
		InvoiceRow* pRow = m_Rows.find_row(invoiceId);
		viewCreator.RefreshView(pRow->Content(), pInvoiceCO->Object());
		NotifyContentUpdated();
	}

}

void SellInvoicesListContent::OnSellInvoiceDeleted(uint32_t invoiceId)
{
	RemoveRow(invoiceId);
	NotifyCountChanged();
}

void SellInvoicesListContent::OnSellInvoiceLocked(uint32_t invoiceId)
{
	InvoiceRow* row = FindRow(invoiceId);
	if (row)
	{
		row->SetRowColor(Color(LOCKED_ROW_COLOR));
		NotifyContentUpdated();
	}
}

void SellInvoicesListContent::OnSellInvoiceUnlocked(uint32_t invoiceId)
{
	InvoiceRow* row = FindRow(invoiceId);
	if (row)
	{
		row->SetRowColor(Color());
		NotifyContentUpdated();
	}
}
