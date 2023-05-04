#include "stdafx.h"
#include "SimpleInvoicesListContent.h"

#include <arrays/DeepDelete.h>

SimpleInvoicesListContent::SimpleInvoicesListContent(SeasonSession* session) : m_viewCreator(session)
{
}

SimpleInvoicesListContent::~SimpleInvoicesListContent()
{	
	DeepDelete(m_Rows);
}

void SimpleInvoicesListContent::AddRow(IInvoice* invoice)
{
	InvoiceView* view = m_viewCreator.CreateView(invoice);
	InvoiceRow* row = new InvoiceRow(view);
	m_Rows.add_row(row);
	NotifyCountChanged();
}

int SimpleInvoicesListContent::GetRowCount() const
{
	return m_Rows.size();
}

ListRow* SimpleInvoicesListContent::GetRow(int row)
{
	return GetInvoiceRowAt(row);
}

void SimpleInvoicesListContent::RemoveRow(uint32_t invoiceId)
{
	InvoiceRow* pRemovedRow = m_Rows.remove_row(invoiceId);
	delete pRemovedRow;
	NotifyCountChanged();
}


void SimpleInvoicesListContent::RemoveAll()
{
	DeepDelete(m_Rows);
	m_Rows.clear();
	NotifyCountChanged();
}

void SimpleInvoicesListContent::UpdateRow(IInvoice* invoice)
{
	InvoiceRow* row = m_Rows.find_row(invoice->GetId());
	if(row)
	{
		m_viewCreator.RefreshView(row->Content(),invoice);
		NotifyContentUpdated();
	}
}

InvoiceRow* SimpleInvoicesListContent::GetInvoiceRowAt(int row)
{
	return m_Rows.row_at(row);
}

InvoiceRow* SimpleInvoicesListContent::FindRow(uint32_t invoiceId)
{
	return m_Rows.find_row(invoiceId);
}

void SimpleInvoicesListContent::SortContent(const SortAttributes& attributes)
{
}

SimpleInvoicesListContent::iterator SimpleInvoicesListContent::begin()
{
	return m_Rows.begin();
}

SimpleInvoicesListContent::iterator SimpleInvoicesListContent::end()
{
	return m_Rows.end();
}