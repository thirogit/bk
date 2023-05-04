#include "stdafx.h"
#include "PurchaseHentListContent.h"
#include <arrays/DeepDelete.h>
#include "PurchaseHentRow.h"

PurchaseHentListContent::PurchaseHentListContent()
{
}

PurchaseHentListContent::~PurchaseHentListContent()
{	
	DeepDelete(m_Rows);
}

void PurchaseHentListContent::AddRow(IPurchaseHent* purchaseHent)
{
	PurchaseHentView* view = m_viewCreator.CreateView(purchaseHent);
	PurchaseHentRow* row = new PurchaseHentRow(view);
	m_Rows.push_back(row);
	NotifyCountChanged();
}


int PurchaseHentListContent::GetRowCount() const
{
	return m_Rows.size();
}

ListRow* PurchaseHentListContent::GetRow(int row)
{
	return GetPurchaseHentRowAt(row);
}

void PurchaseHentListContent::RemoveRow(const std::wstring& hentId)
{
	/*InvoiceRow* pRemovedRow = m_Rows.remove_row(invoiceId);
	delete pRemovedRow;
	NotifyCountChanged();*/
}

void PurchaseHentListContent::UpdateRow(IPurchaseHent* purchaseHent)
{
	PurchaseHentRow* row = FindRow(purchaseHent->GetId());
	if(row)
	{
		m_viewCreator.RefreshView(row->Content(), purchaseHent);
		NotifyContentUpdated();
	}
}

PurchaseHentRow* PurchaseHentListContent::GetPurchaseHentRowAt(int row)
{
	return m_Rows[row];
}

PurchaseHentRow* PurchaseHentListContent::FindRow(const std::wstring& hentId)
{
	const boost::multi_index::index<RowIndex, tag_Id>::type& indexById = boost::multi_index::get<tag_Id>(m_Rows);
	auto it = indexById.find(hentId);
	if (it != indexById.end())
	{
		return *it;
	}
	return NULL;
}

void PurchaseHentListContent::SortContent(const SortAttributes& attributes)
{
}

void PurchaseHentListContent::Clear()
{
	DeepDelete(m_Rows);
	m_Rows.clear();
}