#include "stdafx.h"
#include "HentsLitContent.h"
#include "../../compare\HentComparator.h"


HentsListContent::HentsListContent() 
{
}

HentsListContent::~HentsListContent()
{	
}

void  HentsListContent::BeforeHentChange()
{
	NotifyBeforeUpdate();
}

void  HentsListContent::AfterHentChange()
{
	NotifyAfterUpdate();
}

void  HentsListContent::OnHentInserted(uint32_t hentId) 
{
	HentClientObject* pHentCO = m_pSession->GetHent(hentId);
	HentViewCreator viewCreator(m_pSession);
	HentRow* pRow = new HentRow(viewCreator.CreateView(pHentCO));
	m_Rows.push_back(pRow);
	m_RowToHentId[pHentCO->Object()->GetId()] = pRow;	

	NotifyCountChanged();
}

void  HentsListContent::OnHentLocked(uint32_t hentId)
{
	RefreshHentsRow(hentId);
}

void HentsListContent::RefreshHentsRow(uint32_t hentId)
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

void  HentsListContent::OnHentUnlocked(uint32_t hentId)
{
	RefreshHentsRow(hentId);
}	

void  HentsListContent::OnHentUpdated(uint32_t hentId) 
{
	RefreshHentsRow(hentId);
}

void  HentsListContent::OnHentDeleted(uint32_t hentId) 
{
	auto rowIt = m_Rows.begin();
	auto endIt = m_Rows.end();
	HentRow* row;
	while(rowIt != endIt)
	{
		row = *rowIt;
		if(row->Content()->GetHentId() == hentId)
		{
			m_Rows.erase(rowIt);
			delete row;
			break;
		}
		rowIt++;
	}

	NotifyCountChanged();
}

void HentsListContent::CreateContent(SeasonSession* pSession)
{
	m_pSession = pSession;
			 
	PtrEnumerator<HentClientObject> hentCOEnum = m_pSession->EnumHents();
	HentViewCreator viewCreator(m_pSession);
	HentClientObject* pHentCO = NULL;
	HentRow* pRow = NULL;
	while(hentCOEnum.hasNext())
	{
		pHentCO = *hentCOEnum;
		pRow = new HentRow(viewCreator.CreateView(pHentCO));
		m_Rows.push_back(pRow);
		m_RowToHentId[pHentCO->Object()->GetId()] = pRow;
		hentCOEnum.advance();
	}
	m_pSession->AddSeasonObserver(this);		
}

void HentsListContent::DestroyContent()
{
	if(m_pSession)
		m_pSession->RemoveSeasonObserver(this);		

	DeepDelete(m_Rows);						
	m_Rows.clear();
	m_RowToHentId.clear();
	m_pSession = NULL;
}

int HentsListContent::GetRowCount() const
{
	return m_Rows.size();
}

ListRow* HentsListContent::GetRow(int row)
{
	return GetHentAtRow(row);
}

HentRow* HentsListContent::GetHentAtRow(int row)
{
	return m_Rows[row];
}

void HentsListContent::SortContent(const SortAttributes& attributes)
{
	RowComparator< DataComparator<HentComparator> > comparator(attributes);
	std::sort(m_Rows.begin(), m_Rows.end(), comparator);
}

FindResult HentsListContent::FindHents(Predicate* predicate)
{
	FindResult result;
	result.count = 0;
	result.firstOccurence = -1;
	
	int i = 0;
	BOOST_FOREACH(HentRow* row, m_Rows)
	{
		row->SetMarked(false);
		if (predicate->Test(row->Content()))
		{
			result.count++;
			row->SetMarked(true);

			if(result.firstOccurence < 0)
			{ 
				result.firstOccurence = i;
			}
		}
		i++;
	}
	
	NotifyContentUpdated();
	 
	return result;
}
