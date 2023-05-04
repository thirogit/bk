#include "stdafx.h"
#include "DocContent.h"
#include <algorithm>
#include <arrays/DeepDelete.h>

DocContent::DocContent()
{
}

DocContent::~DocContent()
{
	RemoveAll();
}

		
int DocContent::GetCowCount() const
{
	return m_Cows.size();
}

PtrEnumerator<ICowEntry> DocContent::EnumCows() const
{
	return PtrEnumerator<ICowEntry>(m_Cows);
}

PtrEnumerator<CowEntry> DocContent::EnumCows()
{
	return PtrEnumerator<CowEntry>(m_Cows);
}

CowEntry* DocContent::GetCow(uint32_t cowId) const
{
	const boost::multi_index::index<ContentIndex,index_by_id>::type& indexById = boost::multi_index::get<index_by_id>(m_Cows);

	auto entryIt = indexById.find(cowId);
	if(entryIt != indexById.end())
		return *entryIt;

	return NULL;
}

CowEntry* DocContent::GetCowAt(int index) const
{
	return m_Cows.at(index);
}

void DocContent::RemoveAt(int index)
{
	if(index >= 0 && index < GetCowCount())
	{
		delete m_Cows[index];
		m_Cows.erase(m_Cows.begin()+index);		
	}
}

void DocContent::RemoveAll()
{
	DeepDelete(m_Cows);
	m_Cows.clear();
}

CowEntry* DocContent::AddCow(uint32_t cowId)
{
	CowEntry* pEntry = new CowEntry(cowId);
	m_Cows.push_back(pEntry);
	return pEntry;
}

void DocContent::AddCow(CowEntry* cowEntry)
{
	m_Cows.push_back(cowEntry);
}

void DocContent::RemoveCow(uint32_t cowId)
{
	boost::multi_index::index<ContentIndex,index_by_id>::type& indexById = boost::multi_index::get<index_by_id>(m_Cows);

	auto entryIt = indexById.find(cowId);
	if(entryIt != indexById.end())
	{
		CowEntry* pRemovedEntry = *entryIt;
		indexById.erase(entryIt);	
		delete pRemovedEntry;
	}
}
