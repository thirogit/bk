#include "stdafx.h"
#include "PurchaseInvoiceDeductionsListContent.h"
#include <arrays/DeepDelete.h>
#include "PurchaseHentRow.h"

PurchaseInvoiceDeductionsListContent::PurchaseInvoiceDeductionsListContent()
{
}

PurchaseInvoiceDeductionsListContent::~PurchaseInvoiceDeductionsListContent()
{	
	DeepDelete(m_Rows);
}

void PurchaseInvoiceDeductionsListContent::AddRow(IPurchaseInvoiceDeduction* deduction)
{
	PurchaseInvoiceDeductionView* view = m_viewCreator.CreateView(deduction);
	PurchaseInvoiceDeductionRow* row = new PurchaseInvoiceDeductionRow(view);
	m_Rows.push_back(row);
	NotifyCountChanged();
}


int PurchaseInvoiceDeductionsListContent::GetRowCount() const
{
	return m_Rows.size();
}

ListRow* PurchaseInvoiceDeductionsListContent::GetRow(int row)
{
	return GetDeductionRowAt(row);
}

void PurchaseInvoiceDeductionsListContent::RemoveRow(const std::wstring& deductionId)
{
	/*InvoiceRow* pRemovedRow = m_Rows.remove_row(invoiceId);
	delete pRemovedRow;
	NotifyCountChanged();*/
}

void PurchaseInvoiceDeductionsListContent::UpdateRow(IPurchaseInvoiceDeduction* deduction)
{
	PurchaseInvoiceDeductionRow* row = FindRow(deduction->GetId());
	if(row)
	{
		m_viewCreator.RefreshView(row->Content(), deduction);
		NotifyContentUpdated();
	}
}

PurchaseInvoiceDeductionRow* PurchaseInvoiceDeductionsListContent::GetDeductionRowAt(int row)
{
	return m_Rows[row];
}

PurchaseInvoiceDeductionRow* PurchaseInvoiceDeductionsListContent::FindRow(const std::wstring& deductionId)
{
	const boost::multi_index::index<RowIndex, tag_Id>::type& indexById = boost::multi_index::get<tag_Id>(m_Rows);
	auto it = indexById.find(deductionId);
	if (it != indexById.end())
	{
		return *it;
	}
	return NULL;
}

void PurchaseInvoiceDeductionsListContent::SortContent(const SortAttributes& attributes)
{
}

void PurchaseInvoiceDeductionsListContent::Clear()
{
	DeepDelete(m_Rows);
	m_Rows.clear();
}