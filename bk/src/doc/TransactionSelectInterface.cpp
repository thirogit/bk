#include "stdafx.h"
#include "TransactionSelectInterface.h"
#include <string/TextUtils.h>

TransactionSelectInterface::TransactionSelectInterface() : m_transaction(NULL),m_pCandidates(NULL)
{
}

TransactionSelectInterface::~TransactionSelectInterface()
{
	Destroy();
}

void TransactionSelectInterface::Destroy()
{
	if(m_pCandidates)
	{
		m_pCandidates->DestroyContent();
		delete m_pCandidates;
		m_pCandidates = NULL;
	}
	
	m_transaction = NULL;
}

void TransactionSelectInterface::Create(SeasonSession* pSession,
										uint32_t herdId,
										MinusDocTransactionFacade* pTransactionFacade)
{
	Destroy();

	m_pCandidates = new CandidatesContent();
	m_pCandidates->CreateContent(pSession,herdId);
	m_transaction = pTransactionFacade;

	const DocItem* pItem;
	for(int i = 0,count = m_transaction->GetItemCount();i < count;i++)
	{
		pItem = m_transaction->GetItemAt(i);
		m_pCandidates->RemoveCow(pItem->GetCowId());
	}

}

ListContent* TransactionSelectInterface::GetCandidates()
{
	return m_pCandidates;
}

void TransactionSelectInterface::PickRow(int row)
{
	if(row >= 0 && row < m_pCandidates->GetRowCount())
	{
		CowPresenceSelectRow* pRow = m_pCandidates->FindRow(row);
		if(!pRow->IsSelected())
		{
			CowPresenceView* pContent = pRow->Content();
			ItemId id = m_transaction->MinusCow(pContent->GetKey().GetCowId());
			pRow->SetItemId(id);
			pRow->SetSelected(true);	
			m_Picked.insert(id);
		}
	}
}

void TransactionSelectInterface::UnpickRow(int row)
{
	if(row >= 0 && row < m_pCandidates->GetRowCount())
	{
		CowPresenceSelectRow* pRow = m_pCandidates->FindRow(row);
		if(pRow->IsSelected())
		{
			ItemId id = pRow->GetItemId();
			m_transaction->DeleteItem(id);
			pRow->SetSelected(false);
			pRow->SetItemId(ItemId());
			m_Picked.erase(id);
		}
	}
}

bool TransactionSelectInterface::SelectCow(const CowNo& cowNo)
{
	for(int row = 0,count = m_pCandidates->GetRowCount();row < count;row++)
	{
		CowPresenceSelectRow* pRow = m_pCandidates->FindRow(row);
		CowPresenceView* pRowContent = pRow->Content();
		if(pRowContent->GetCowNo() == cowNo)
		{
			PickRow(row);
			return true;
		}
	}
	return false;
}

SelectResult TransactionSelectInterface::SelectCows(const std::wstring& cowNoWildcard)
{
	SelectResult result;
	result.matched = 0;
	result.picked = 0;

	for (int row = 0, count = m_pCandidates->GetRowCount(); row < count; row++)
	{
		CowPresenceSelectRow* pRow = m_pCandidates->FindRow(row);
		CowPresenceView* pRowContent = pRow->Content();
		if (TextUtils::ContainsNoCase(pRowContent->GetCowNo().ToString(), cowNoWildcard))
		{
			result.matched++;
			try
			{
				PickRow(row);
				result.picked++;
			}
			catch (...)
			{

			}
			
		}
	}
	return result;
}

bool TransactionSelectInterface::IsRowPicked(int row)
{
	if(row >= 0 && row < m_pCandidates->GetRowCount())
	{
		CowPresenceSelectRow* pRow = m_pCandidates->FindRow(row);
		return m_Picked.find(pRow->GetItemId()) != m_Picked.end();
	}
	return false;
}

size_t TransactionSelectInterface::GetPickedCount() const
{
	return m_Picked.size();
}



