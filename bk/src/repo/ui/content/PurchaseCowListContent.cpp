#include "stdafx.h"
#include "PurchaseCowListContent.h"
#include <arrays/DeepDelete.h>
#include "PurchaseHentRow.h"

PurchaseCowListContent::PurchaseCowListContent()
{
}

PurchaseCowListContent::~PurchaseCowListContent()
{	
	DeepDelete(m_Rows);
}

void PurchaseCowListContent::AddRow(IPurchaseCow* purchaseCow)
{
	PurchaseCowView* view = m_viewCreator.CreateView(purchaseCow);
	PurchaseCowRow* row = new PurchaseCowRow(view);
	m_Rows.push_back(row);
	NotifyCountChanged();
}


int PurchaseCowListContent::GetRowCount() const
{
	return m_Rows.size();
}

ListRow* PurchaseCowListContent::GetRow(int row)
{
	return GetPurchaseCowRowAt(row);
}

void PurchaseCowListContent::RemoveRow(const std::wstring& hentId)
{
	/*InvoiceRow* pRemovedRow = m_Rows.remove_row(invoiceId);
	delete pRemovedRow;
	NotifyCountChanged();*/
}

void PurchaseCowListContent::UpdateRow(IPurchaseCow* purchaseCow)
{
	PurchaseCowRow* row = FindRow(purchaseCow->GetId());
	if(row)
	{
		m_viewCreator.RefreshView(row->Content(), purchaseCow);
		NotifyContentUpdated();
	}
}

PurchaseCowRow* PurchaseCowListContent::GetPurchaseCowRowAt(int row)
{
	return m_Rows[row];
}

PurchaseCowRow* PurchaseCowListContent::FindRow(const std::wstring& cowId)
{
	const boost::multi_index::index<RowIndex, tag_Id>::type& indexById = boost::multi_index::get<tag_Id>(m_Rows);
	auto it = indexById.find(cowId);
	if (it != indexById.end())
	{
		return *it;
	}
	return NULL;
}

void PurchaseCowListContent::SortContent(const SortAttributes& attributes)
{
}

void PurchaseCowListContent::Clear()
{
	DeepDelete(m_Rows);
	m_Rows.clear();
	NotifyCountChanged();
}