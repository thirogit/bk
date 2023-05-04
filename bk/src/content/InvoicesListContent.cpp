#include "stdafx.h"
#include "InvoicesListContent.h"
#include "../view/InvoiceViewCreator.h"
#include <arrays/DeepDelete.h>
#include "../compare/DataComparator.h"
#include "compare/RowComparator.h"
#include "../compare/InvoiceViewComparator.h"

InvoicesListContent::InvoicesListContent() : m_pSession(NULL)
{
}

InvoicesListContent::~InvoicesListContent()
{	
}

void InvoicesListContent::Create(SeasonSession* pSession, const InvoiceKind& kind)
{
	m_pSession = pSession;		
	m_kind = kind;
	CreateContent();
	m_pSession->AddSeasonObserver(this);	
}


void InvoicesListContent::AddRow(InvoiceRow* row)
{
	m_Rows.add_row(row);
}

int InvoicesListContent::GetRowCount() const
{
	return m_Rows.size();
}

ListRow* InvoicesListContent::GetRow(int row)
{
	return GetInvoiceRowAt(row);
}

void InvoicesListContent::RemoveRow(uint32_t invoiceId)
{
	InvoiceRow* pRemovedRow = m_Rows.remove_row(invoiceId);
	delete pRemovedRow;
}

void InvoicesListContent::Destroy()
{
	if(m_pSession)
		m_pSession->RemoveSeasonObserver(this);	

	m_pSession = NULL;
	DeepDelete(m_Rows);	
}

InvoiceRow* InvoicesListContent::GetInvoiceRowAt(int row)
{
	return m_Rows.row_at(row);
}

InvoiceRow* InvoicesListContent::GetInvoiceRowWithId(uint32_t id)
{
	return m_Rows.find_row(id);
}

InvoiceRow* InvoicesListContent::FindRow(uint32_t invoiceId)
{
	return m_Rows.find_row(invoiceId);
}

void InvoicesListContent::SortContent(const SortAttributes& attributes)
{
	RowComparator< DataComparator<InvoiceViewComparator> > comparator(attributes);
	m_Rows.sort(comparator);
}

bool InvoicesListContent::IsValidForThisContent(IInvoice* invoice)
{
	InvoiceType type = m_kind.GetInvoiceType();
	return (type == InvoiceType_None || type == invoice->GetInvoiceType());
}

InvoiceType InvoicesListContent::GetInvoiceType() const
{
	return m_kind.GetInvoiceType();
}

const InvoiceKind& InvoicesListContent::GetInvoiceKind() const
{
	return m_kind;
}