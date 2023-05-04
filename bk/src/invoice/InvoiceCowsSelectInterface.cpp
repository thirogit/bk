#include "stdafx.h"
#include "InvoiceCowsSelectInterface.h"
#include "../runtime/UniqueIds.h"
#include <boost\foreach.hpp>
#include <string/TextUtils.h>

InvoiceCowsSelectInterface::InvoiceCowsSelectInterface() : m_transaction(NULL),m_pCandidates(NULL),m_pSession(NULL)
{
}

InvoiceCowsSelectInterface::~InvoiceCowsSelectInterface()
{
	Destroy();
}

void InvoiceCowsSelectInterface::Destroy()
{
	if(m_pCandidates)
	{
		m_pCandidates->DestroyContent();
		delete m_pCandidates;
		m_pCandidates = NULL;
		m_pSession = NULL;
	}
	
}

void InvoiceCowsSelectInterface::Create(SeasonSession* pSession,
										CowsScopeInvoiceTransactionFacade* transaction)
{
	Destroy();
	m_pSession = pSession;
	m_transaction = transaction;	
	m_pCandidates = new CowCandidatesContent();
	m_pCandidates->CreateContent(pSession);
	
	UniqueIds selection = GetCowsSelection();
	
	BOOST_FOREACH(uint32_t cowId,selection)
	{
		m_pCandidates->AddCow(cowId);
	}	
	
}

ListContent* InvoiceCowsSelectInterface::GetCandidates()
{
	return m_pCandidates;
}

void InvoiceCowsSelectInterface::PickRow(int row)
{
	if(row >= 0 && row < m_pCandidates->GetRowCount())
	{
		CowSelectableRow* pRow = m_pCandidates->GetRowAt(row);
		if(!pRow->IsSelected())
		{
			CowView* pContent = pRow->Content();
			m_transaction->AddCow(pContent->GetId());
			pRow->SetSelected(true);	
		}
	}
}

void InvoiceCowsSelectInterface::UnpickRow(int row)
{
	if(row >= 0 && row < m_pCandidates->GetRowCount())
	{
		CowSelectableRow* pRow = m_pCandidates->GetRowAt(row);
		if(pRow->IsSelected())
		{
			CowView* pContent = pRow->Content();
			m_transaction->DeleteCow(pContent->GetId());
			pRow->SetSelected(false);			
		}
	}
}

bool InvoiceCowsSelectInterface::SelectCow(const CowNo& cowNo)
{
	for(int row = 0,count = m_pCandidates->GetRowCount();row < count;row++)
	{
		CowSelectableRow* pRow = m_pCandidates->GetRowAt(row);
		CowView* pRowContent = pRow->Content();
		if(pRowContent->GetCowNo() == cowNo)
		{
			PickRow(row);
			return true;
		}
	}

	return false;
}

bool InvoiceCowsSelectInterface::IsRowPicked(int row)
{
	if(row >= 0 && row < m_pCandidates->GetRowCount())
	{
		CowSelectableRow* pRow = m_pCandidates->GetRowAt(row);
		return pRow->IsSelected();
	}
	return false;
}

size_t InvoiceCowsSelectInterface::GetPickedCount() const
{
	size_t pickedCount = 0;
	for(int row = 0,count = m_pCandidates->GetRowCount();row < count;row++)
	{
		CowSelectableRow* pRow = m_pCandidates->GetRowAt(row);
		if(pRow->IsSelected()) pickedCount++;		
	}
	return pickedCount;
}

SelectResult InvoiceCowsSelectInterface::SelectCows(const std::wstring& cowNoWildcard)
{
	SelectResult result;
	result.matched = 0;
	result.picked = 0;

	for (int row = 0, count = m_pCandidates->GetRowCount(); row < count; row++)
	{
		CowSelectableRow* pRow = m_pCandidates->GetRowAt(row);
		CowSelectableRow::content_type* pRowContent = pRow->Content();
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
