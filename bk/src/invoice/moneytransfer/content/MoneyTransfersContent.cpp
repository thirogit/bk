#include "stdafx.h"
#include "MoneyTransfersContent.h"
#include <arrays\DeepDelete.h>
#include "../view/MoneyTransferViewCreator.h"

MoneyTransfersContent::MoneyTransfersContent()
{
}

MoneyTransfersContent::~MoneyTransfersContent()
{
	DeepDelete(m_Rows);
}

int MoneyTransfersContent::GetRowCount() const
{
	return m_Rows.size();
}

ListRow* MoneyTransfersContent::GetRow(int row)
{
	return GetMoneyTransferRow(row);
}

void MoneyTransfersContent::SortContent(const SortAttributes& attributes)
{
}

MoneyTransferRow* MoneyTransfersContent::GetMoneyTransferRow(int row)
{
	return m_Rows[row];
}

void MoneyTransfersContent::AddRow(const MoneyTransferEntry* entry)
{
	MoneyTransferViewCreator viewCreator;
	MoneyTransferView* view = viewCreator.CreateView(entry);
	MoneyTransferRow* row = new MoneyTransferRow(view);
	m_Rows.push_back(row);
	NotifyCountChanged();
}

void MoneyTransfersContent::UpdateRow(const MoneyTransferEntry* entry)
{
	boost::multi_index::index<RowIndexType, tag_InoiceId>::type& indexById = m_Rows.get<tag_InoiceId>();

	auto it = indexById.find(entry->GetInvoiceId());
	if (it != indexById.end())
	{
		MoneyTransferViewCreator viewCreator;
		viewCreator.RefreshView((*it)->Content(), entry);
		NotifyContentUpdated();
	}
}

void MoneyTransfersContent::RemoveRow(uint32_t invoiceId)
{
	boost::multi_index::index<RowIndexType, tag_InoiceId>::type& indexById = m_Rows.get<tag_InoiceId>();

	auto it = indexById.find(invoiceId);
	if (it != indexById.end())
	{
		indexById.erase(it);
		NotifyCountChanged();
	}	
}


void MoneyTransfersContent::Clear()
{
	DeepDelete(m_Rows);
	m_Rows.clear();
}