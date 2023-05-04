#include "stdafx.h"
#include "HentSearchContent.h"
#include "../view/HentViewCreator.h"
#include <arrays/DeepDelete.h>
#include <boost/algorithm/string/find.hpp>

HentSearchContent::HentSearchContent() : m_pSession(NULL)
{
}

HentSearchContent::~HentSearchContent()
{	
}

void  HentSearchContent::OnHentInserted(uint32_t hentId) 
{
	HentClientObject* pHentCO = m_pSession->GetHent(hentId);
	if(IsHentMatching(pHentCO->Object()))
	{
		HentViewCreator viewCreator(m_pSession);
		AddRow(new HentRow(viewCreator.CreateView(pHentCO)));
	}
	NotifyCountChanged();
}

void  HentSearchContent::OnHentLocked(uint32_t hentId)
{
	RefreshHentsRow(hentId);
}

void HentSearchContent::RefreshHentsRow(uint32_t hentId)
{
	auto rowIt = m_RowToHentId.find(hentId);
	if(rowIt != m_RowToHentId.end())
	{
		HentRow* pRow = rowIt->second;
		HentViewCreator viewCreator(m_pSession);
		HentView* pView = pRow->Content();
		viewCreator.RefreshView(pView,m_pSession->GetHent(pView->GetHentId()));
		
		NotifyContentUpdated();		
	}
}

void  HentSearchContent::OnHentUnlocked(uint32_t hentId)
{
	RefreshHentsRow(hentId);
}	

void  HentSearchContent::OnHentUpdated(uint32_t hentId) 
{
	RefreshHentsRow(hentId);
}

void  HentSearchContent::OnHentDeleted(uint32_t hentId) 
{
	auto rowIt = m_Rows.begin();
	auto endIt = m_Rows.end();
	HentRow* row;

	if(m_RowToHentId.find(hentId) != m_RowToHentId.end())
	{
		while(rowIt != endIt)
		{
			row = *rowIt;
			if(row->Content()->GetHentId() == hentId)
			{
				m_Rows.erase(rowIt);
				delete row;
				break;
			}
		}

		NotifyCountChanged();
	}
}

bool HentSearchContent::IsHentMatching(const IHent* pHent)
{
	return (boost::ifind_first(pHent->GetName(),m_sQueryPattern) || boost::ifind_first(pHent->GetHentNo().ToString(),m_sQueryPattern));
}

void HentSearchContent::AddRow(HentRow* row)
{
	m_Rows.push_back(row);
	HentView* pView = row->Content();	
	m_RowToHentId.insert(std::pair<uint32_t,HentRow*>(pView->GetHentId(),row));
}

void HentSearchContent::CreateContent(SeasonSession* pSession,const std::wstring& sQueryPatern)
{
	m_pSession = pSession;
	m_sQueryPattern = sQueryPatern;

			 
	PtrEnumerator<HentClientObject> hentCOEnum = m_pSession->EnumHents();
															 
	HentClientObject* pHentCO = NULL;
	IHent* pHent;
	HentViewCreator viewCreator(m_pSession);
	while(hentCOEnum.hasNext())
	{
		pHentCO = *hentCOEnum;	
		pHent = pHentCO->Object();
																 
		if(IsHentMatching(pHent))
		{
			AddRow(new HentRow(viewCreator.CreateView(pHentCO)));																		
		}
		hentCOEnum.advance();
	}
	m_pSession->AddSeasonObserver(this);															 
	
	NotifyCountChanged();
}

void HentSearchContent::DestroyContent()
{
	if(m_pSession)
	{		
		m_pSession->RemoveSeasonObserver(this);		
		m_pSession = NULL;
	}

	DeepDelete(m_Rows);						
	m_Rows.clear();
	m_RowToHentId.clear();
	NotifyCountChanged();
	
}

int HentSearchContent::GetRowCount() const
{
	return m_Rows.size();
}

ListRow* HentSearchContent::GetRow(int row)
{
	return GetHentAtRow(row);
}

HentRow* HentSearchContent::GetHentAtRow(int row)
{
	return m_Rows[row];
}


void HentSearchContent::SortContent(const SortAttributes& attributes)
{
}