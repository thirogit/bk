#include "stdafx.h"
#include "SellDocSelectInterface.h"

void SellDocKamilWSelectInterface::Create(SeasonSession* pSession, InvoiceDocsTransactionPtr transaction) 
{
	m_candidates.Create(pSession);
	for(int i = 0,count = transaction->GetDocCount();i <count;i++)
	{
		m_candidates.RemoveDoc(transaction->GetDocAt(i));
	}
	m_transaction = transaction;
}

void SellDocKamilWSelectInterface::Destroy()
{
	m_candidates.Destroy();
	m_transaction.reset();
}

ListContent* SellDocKamilWSelectInterface::GetCandidates()
{
	return &m_candidates;
}

void SellDocKamilWSelectInterface::PickRow(int row)
{
	SelectableHentsDocRow* pRow = m_candidates.GetDocAtRow(row);
	uint32_t docId = pRow->Content()->GetDocId();
	m_transaction->AddDoc(docId);
	m_picked.insert(docId);
	pRow->SetSelected(true);
}

void SellDocKamilWSelectInterface::UnpickRow(int row)
{
	if(IsRowPicked(row))
	{
		SelectableHentsDocRow* pRow = m_candidates.GetDocAtRow(row);
		uint32_t docId = pRow->Content()->GetDocId();
		m_transaction->DeleteDoc(docId);
		m_picked.erase(docId);
		pRow->SetSelected(false);
	}
}

bool SellDocKamilWSelectInterface::IsRowPicked(int row)
{
	SelectableHentsDocRow* pRow = m_candidates.GetDocAtRow(row);
	uint32_t docId = pRow->Content()->GetDocId();
	return m_picked.find(docId) != m_picked.end();
}

size_t SellDocKamilWSelectInterface::GetPickedCount() const
{
	return m_picked.size();
}
