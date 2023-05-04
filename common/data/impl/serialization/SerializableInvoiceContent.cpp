#include "stdafx.h"
#include "SerializableInvoiceContent.h"
#include "../../types/serialization/TypesSerialization.h"
#include "../../IIdable.h"
#include "SerializableCowInvoiceEntry.h"
#include <CommonTypes.h>

SerializableInvoiceContent::SerializableInvoiceContent(InvoiceContent& content) : m_Content(content)
{
}

void SerializableInvoiceContent::serialize(SF::Archive &archive)
{
	count_t count;

	if(archive.isRead())
	{
		archive & count;
		for(int i = 0;i < count;i++)
		{
			CowInvoiceEntry* pNewEntry = new CowInvoiceEntry(NULL_ID);
			SerializableCowInvoiceEntry(pNewEntry).serialize(archive);
			m_Content.m_Entries.push_back(pNewEntry);
		}
	}
	else
	{
		count = m_Content.m_Entries.size();
		archive & count;
		for(int i = 0;i < count;i++)
		{
			SerializableCowInvoiceEntry(m_Content.m_Entries.at(i)).serialize(archive);
		}
	}
}