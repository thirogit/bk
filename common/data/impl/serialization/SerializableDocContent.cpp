#include "stdafx.h"
#include "SerializableDocContent.h"
#include "../../types/serialization/TypesSerialization.h"
#include "../../IIdable.h"
#include "SerializableCowEntry.h"
#include <boost/foreach.hpp>
#include <CommonTypes.h>

SerializableDocContent::SerializableDocContent(DocContent* pDocContent) : m_pDocContent(pDocContent)
{
}

void SerializableDocContent::serialize(SF::Archive &archive)
{
	count_t count;

	if(archive.isRead())
	{
		archive & count;
		for(int i = 0;i < count;i++)
		{
			CowEntry* pNewEntry = new CowEntry(NULL_ID);
			SerializableCowEntry(pNewEntry).serialize(archive);
			m_pDocContent->m_Cows.push_back(pNewEntry);
		}
	}
	else
	{
		count = m_pDocContent->m_Cows.size();
		archive & count;

		BOOST_FOREACH(CowEntry* entry,m_pDocContent->m_Cows)
		{
			SerializableCowEntry(entry).serialize(archive);
		}
	}
}
