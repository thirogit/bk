#include "stdafx.h"
#include "InvoiceChangeDetector.h"
#include "../../arrays/DeepDelete.h"

InvoiceChange::InvoiceChange(ChangeEvent event,uint32_t cowId,ICowInvoiceEntry* pOld,ICowInvoiceEntry* pNew) : m_cowId(cowId),m_pOld(pOld),m_pNew(pNew),m_event(event)
{
}

InvoiceChange::InvoiceChange(const InvoiceChange& src) : m_cowId(src.m_cowId),m_pOld(src.m_pOld),m_pNew(src.m_pNew),m_event(src.m_event)
{
}

uint32_t InvoiceChange::GetCowId() const
{
	return m_cowId;
}

ICowInvoiceEntry* InvoiceChange::Old() const
{
	return m_pOld;
}

ICowInvoiceEntry* InvoiceChange::New() const
{
	return m_pNew;
}

ChangeEvent InvoiceChange::Event() const
{
	return m_event;
}

//-------------------------------------------------------------------------------------------------
	
InvoiceChanges::InvoiceChanges()
{
}

InvoiceChanges::~InvoiceChanges()
{
	DeepDelete(m_changes);
}

InvoiceChanges::InvoiceChanges(InvoiceChanges&& src)
{
	m_changes.assign(src.m_changes.begin(),src.m_changes.end());
	src.m_changes.clear();
}

InvoiceChanges::InvoiceChanges(const InvoiceChanges& src)
{
	std::for_each(src.begin(),src.end(),[this](InvoiceChange* change)
	{
		m_changes.push_back(new InvoiceChange(*change));
	});
}

void InvoiceChanges::add(ChangeEvent event,uint32_t cowId,ICowInvoiceEntry* pOld,ICowInvoiceEntry* pNew)
{
	m_changes.push_back(new InvoiceChange(event,cowId,pOld,pNew));
}

InvoiceChanges::const_iterator InvoiceChanges::begin() const
{
	return m_changes.begin();
}

InvoiceChanges::const_iterator InvoiceChanges::end() const
{
	return m_changes.end();
}

InvoiceChanges::iterator InvoiceChanges::begin()
{
	return m_changes.begin();
}

InvoiceChanges::iterator InvoiceChanges::end()
{
	return m_changes.end();
}

//-------------------------------------------------------------------------------------------------

InvoiceChangeDetector::InvoiceChangeDetector(IInvoice* pOld,IInvoice* pNew) : m_pOldInvoice(pOld), m_pNewInvoice(pNew)
{
}

InvoiceChanges InvoiceChangeDetector::Detect()
{
	InvoiceEntriesIndex newEntries;

	PtrEnumerator<ICowInvoiceEntry> cowsEnum = m_pNewInvoice->EnumEntries();
	while(cowsEnum.hasNext())
	{
		newEntries.insert((*cowsEnum));
		cowsEnum.advance();
	}

	InvoiceChanges changes;
	uint32_t cowId;
	ICowInvoiceEntry* pEntry;
	cowsEnum = m_pOldInvoice->EnumEntries();

	boost::multi_index::index<InvoiceEntriesIndex,index_tag_cowid>::type& entriesByCowId = boost::multi_index::get<index_tag_cowid>(newEntries);

	while(cowsEnum.hasNext())
	{
		pEntry = *cowsEnum;
		cowId = pEntry->GetCowId();

		auto entryIt = entriesByCowId.find(cowId);

		if(entryIt != entriesByCowId.end())
		{
			changes.add(ChangeEvent_Updated,cowId,pEntry,*entryIt);
			entriesByCowId.erase(entryIt);
		}
		else
		{
			changes.add(ChangeEvent_Deleted,cowId,pEntry,NULL);			
		}
		cowsEnum.advance();
	}

	std::for_each(newEntries.begin(),newEntries.end(),[&changes](ICowInvoiceEntry* pNewEntry)
	{
		changes.add(ChangeEvent_Added,pNewEntry->GetCowId(),NULL,pNewEntry);	
	});

	return std::move(changes);

}

