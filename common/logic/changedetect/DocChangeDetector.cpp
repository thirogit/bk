#include "stdafx.h"
#include "DocChangeDetector.h"
#include "../../arrays/DeepDelete.h"

DocChange::DocChange(ChangeEvent event,ICowEntry* pOld,ICowEntry* pNew) : m_pOld(pOld),m_pNew(pNew),m_event(event)
{
}

DocChange::DocChange(const DocChange& src) : m_pOld(src.m_pOld),m_pNew(src.m_pNew),m_event(src.m_event)
{
}

ICowEntry* DocChange::Old() const
{
	return m_pOld;
}

ICowEntry* DocChange::New() const
{
	return m_pNew;
}

ChangeEvent DocChange::Event() const
{
	return m_event;
}

//-------------------------------------------------------------------------------------------------
	
DocChanges::DocChanges()
{
}

DocChanges::~DocChanges()
{
	DeepDelete(m_changes);
}

DocChanges::DocChanges(DocChanges&& src)
{
	m_changes.assign(src.m_changes.begin(),src.m_changes.end());
	src.m_changes.clear();
}

DocChanges::DocChanges(const DocChanges& src)
{
	std::for_each(src.begin(),src.end(),[this](DocChange* change)
	{
		m_changes.push_back(new DocChange(*change));
	});
}

void DocChanges::add(ChangeEvent event,ICowEntry* pOld,ICowEntry* pNew)
{
	m_changes.push_back(new DocChange(event,pOld,pNew));
}

DocChanges::const_iterator DocChanges::begin() const
{
	return m_changes.begin();
}

DocChanges::const_iterator DocChanges::end() const
{
	return m_changes.end();
}

DocChanges::iterator DocChanges::begin()
{
	return m_changes.begin();
}

DocChanges::iterator DocChanges::end()
{
	return m_changes.end();
}

//-------------------------------------------------------------------------------------------------

DocChangeDetector::DocChangeDetector(IDoc* pOldDoc,IDoc* pNewDoc) : m_pOldDoc(pOldDoc), m_pNewDoc(pNewDoc)
{
}

DocChanges DocChangeDetector::Detect()
{
	DocEntriesIndex newDocEntries;

	PtrEnumerator<ICowEntry> cowsEnum = m_pNewDoc->EnumCows();
	while(cowsEnum.hasNext())
	{
		newDocEntries.insert((*cowsEnum));
		cowsEnum.advance();
	}

	DocChanges DocChanges;
	uint32_t cowId;
	ICowEntry* pEntry;
	cowsEnum = m_pOldDoc->EnumCows();

	boost::multi_index::index<DocEntriesIndex,index_tag_cowid>::type& entriesByCowId = boost::multi_index::get<index_tag_cowid>(newDocEntries);

	while(cowsEnum.hasNext())
	{
		pEntry = *cowsEnum;
		cowId = pEntry->GetCowId();

		auto entryIt = entriesByCowId.find(cowId);

		if(entryIt != entriesByCowId.end())
		{
			DocChanges.add(ChangeEvent_Updated,pEntry,*entryIt);
			entriesByCowId.erase(entryIt);
		}
		else
		{
			DocChanges.add(ChangeEvent_Deleted,pEntry,NULL);			
		}
		cowsEnum.advance();
	}

	std::for_each(newDocEntries.begin(),newDocEntries.end(),[&DocChanges](ICowEntry* pNewEntry)
	{
		DocChanges.add(ChangeEvent_Added,NULL,pNewEntry);	
	});

	return std::move(DocChanges);

}

