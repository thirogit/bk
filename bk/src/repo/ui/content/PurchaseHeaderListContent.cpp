#include "stdafx.h"
#include "PurchaseHeaderListContent.h"
#include <arrays/DeepDelete.h>
#include "PurchaseHeaderRow.h"

PurchaseHeaderListContent::PurchaseHeaderListContent()
{
}

PurchaseHeaderListContent::~PurchaseHeaderListContent()
{	
	DeepDelete(m_Rows);
}

void PurchaseHeaderListContent::AddRow(IPurchaseHeader* purchaseHeader)
{
	PurchaseHeaderView* view = m_viewCreator.CreateView(purchaseHeader);
	PurchaseHeaderRow* row = new PurchaseHeaderRow(view);
	m_Rows.push_back(row);
	NotifyCountChanged();
}

void PurchaseHeaderListContent::AddRows(purchaseheader_iterator beginIt, purchaseheader_iterator endIt)
{
	purchaseheader_iterator it = beginIt;
	while (it != endIt)
	{
		PurchaseHeaderView* view = m_viewCreator.CreateView(*it);
		PurchaseHeaderRow* row = new PurchaseHeaderRow(view);
		m_Rows.push_back(row);
		it++;
	}
	NotifyCountChanged();
}

int PurchaseHeaderListContent::GetRowCount() const
{
	return m_Rows.size();
}

ListRow* PurchaseHeaderListContent::GetRow(int row)
{
	return GetPurchaseRowAt(row);
}

void PurchaseHeaderListContent::RemoveRow(const std::wstring& purchaseId)
{
	/*InvoiceRow* pRemovedRow = m_Rows.remove_row(invoiceId);
	delete pRemovedRow;
	NotifyCountChanged();*/
}

void PurchaseHeaderListContent::UpdateRow(IPurchaseHeader* purchaseHeader)
{
	PurchaseHeaderRow* row = FindRow(purchaseHeader->GetId());
	if(row)
	{
		m_viewCreator.RefreshView(row->Content(), purchaseHeader);
		NotifyContentUpdated();
	}
}

PurchaseHeaderRow* PurchaseHeaderListContent::GetPurchaseRowAt(int row)
{
	return m_Rows[row];
}

PurchaseHeaderRow* PurchaseHeaderListContent::FindRow(const std::wstring& purchaseId)
{
	const boost::multi_index::index<RowIndex, tag_Id>::type& indexById = boost::multi_index::get<tag_Id>(m_Rows);
	auto it = indexById.find(purchaseId);
	if (it != indexById.end())
	{
		return *it;
	}
	return NULL;
}

void PurchaseHeaderListContent::SortContent(const SortAttributes& attributes)
{
}

void PurchaseHeaderListContent::Clear()
{
	DeepDelete(m_Rows);
	m_Rows.clear();
	NotifyCountChanged();
}