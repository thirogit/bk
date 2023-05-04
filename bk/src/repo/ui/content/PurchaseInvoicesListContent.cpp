#include "stdafx.h"
#include "PurchaseInvoicesListContent.h"
#include <arrays/DeepDelete.h>
#include "PurchaseHeaderRow.h"

PurchaseInvoicesListContent::PurchaseInvoicesListContent()
{
}

PurchaseInvoicesListContent::~PurchaseInvoicesListContent()
{	
	DeepDelete(m_Rows);
}

void PurchaseInvoicesListContent::AddRow(IPurchaseInvoice* invoice)
{
	PurchaseInvoiceView* view = m_viewCreator.CreateView(invoice);
	PurchaseInvoiceRow* row = new PurchaseInvoiceRow(view);
	m_Rows.push_back(row);
	NotifyCountChanged();
}

void PurchaseInvoicesListContent::AddRows(invoice_iterator beginIt, invoice_iterator endIt)
{
	invoice_iterator it = beginIt;
	while (it != endIt)
	{
		PurchaseInvoiceView* view = m_viewCreator.CreateView(*it);
		PurchaseInvoiceRow* row = new PurchaseInvoiceRow(view);
		m_Rows.push_back(row);
		it++;
	}
	NotifyCountChanged();
}

int PurchaseInvoicesListContent::GetRowCount() const
{
	return m_Rows.size();
}

ListRow* PurchaseInvoicesListContent::GetRow(int row)
{
	return GetInvoiceRowAt(row);
}

void PurchaseInvoicesListContent::RemoveRow(const std::wstring& invoiceId)
{
	boost::multi_index::index<RowIndex, tag_Id>::type& indexById = boost::multi_index::get<tag_Id>(m_Rows);
	auto it = indexById.find(invoiceId);
	if (it != indexById.end())
	{
		indexById.erase(it);
		NotifyCountChanged();
	}	
}

void PurchaseInvoicesListContent::UpdateRow(IPurchaseInvoice* invoice)
{
	PurchaseInvoiceRow* row = FindRow(invoice->GetId());
	if(row)
	{
		m_viewCreator.RefreshView(row->Content(), invoice);
		NotifyContentUpdated();
	}
}

PurchaseInvoiceRow* PurchaseInvoicesListContent::GetInvoiceRowAt(int row)
{
	return m_Rows[row];
}

PurchaseInvoiceRow* PurchaseInvoicesListContent::FindRow(const std::wstring& invoiceId)
{
	const boost::multi_index::index<RowIndex, tag_Id>::type& indexById = boost::multi_index::get<tag_Id>(m_Rows);
	auto it = indexById.find(invoiceId);
	if (it != indexById.end())
	{
		return *it;
	}
	return NULL;
}

void PurchaseInvoicesListContent::SortContent(const SortAttributes& attributes)
{
}

void PurchaseInvoicesListContent::Clear()
{
	DeepDelete(m_Rows);
	m_Rows.clear();
}