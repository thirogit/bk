#include "stdafx.h"
#include "InvoiceContent.h"
#include "../../arrays/DeepDelete.h"
#include <boost/foreach.hpp>

InvoiceContent::InvoiceContent()
{
}

InvoiceContent::~InvoiceContent()
{
	RemoveAll();
}
		
int InvoiceContent::GetCount() const
{
	return m_Entries.size();
}

PtrEnumerator<ICowInvoiceEntry> InvoiceContent::EnumEntries() const
{
	return PtrEnumerator<ICowInvoiceEntry>(m_Entries);
}

PtrEnumerator<CowInvoiceEntry> InvoiceContent::EnumEntries()
{
	return PtrEnumerator<CowInvoiceEntry>(m_Entries);
}

CowInvoiceEntry* InvoiceContent::GetEntry(uint32_t cowId) const
{
	const boost::multi_index::index<ContentIndex,index_by_id>::type& indexById = boost::multi_index::get<index_by_id>(m_Entries);

	auto entryIt = indexById.find(cowId);
	if(entryIt != indexById.end())
		return *entryIt;

	return NULL;
}

void InvoiceContent::RemoveEntry(uint32_t cowId)
{
	boost::multi_index::index<ContentIndex,index_by_id>::type& indexById = boost::multi_index::get<index_by_id>(m_Entries);

	auto entryIt = indexById.find(cowId);
	if(entryIt != indexById.end())
	{
		delete *entryIt;
		indexById.erase(entryIt);
	}
}

CowInvoiceEntry* InvoiceContent::GetEntryAt(int index) const
{
	return m_Entries.at(index);
}

void InvoiceContent::RemoveAt(int index)
{
	if(index >= 0 && index < GetCount())
	{
		delete m_Entries[index];
		m_Entries.erase(m_Entries.begin()+index);	
	}
}

void InvoiceContent::RemoveAll()
{
	DeepDelete(m_Entries);
	m_Entries.clear();
}

CowInvoiceEntry* InvoiceContent::AddEntry(uint32_t cowId)
{
	CowInvoiceEntry* pEntry = new CowInvoiceEntry(cowId);
	m_Entries.push_back(pEntry);
	return pEntry;
}

void InvoiceContent::AddEntry(CowInvoiceEntry* entry)
{
	m_Entries.push_back(entry);
}


InvoiceContent InvoiceContent::FromJson(const json::value& value)
{
	InvoiceContent result;
	json::array jsonContent = value.as_array();

	BOOST_FOREACH(json::value jsonEntry, jsonContent)
	{
		result.m_Entries.push_back(new CowInvoiceEntry(CowInvoiceEntry::FromJson(jsonEntry)));
	}
	return std::move(result);
}

json::value InvoiceContent::ToJson() const
{
	std::vector<json::value> jsonEntries;

	BOOST_FOREACH(CowInvoiceEntry* entry, m_Entries)
	{
		jsonEntries.push_back(entry->ToJson());
	}

	return json::value::array(jsonEntries);

}