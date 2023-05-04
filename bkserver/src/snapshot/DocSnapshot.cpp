#include "stdafx.h"
#include "DocSnapshot.h"
#include <string/ToString.h>
#include <boost/foreach.hpp>
#include <sstream>

DocSnapshot::DocSnapshot(uint32_t docId) : m_docId(docId)
{
}

DocSnapshot::~DocSnapshot()
{
	m_Items.FreeAndFlush();
}

uint32_t DocSnapshot::GetDocId() const
{
	return m_docId;
}

const DocDetails& DocSnapshot::GetDetails() const
{
	return m_detais;
}

void DocSnapshot::SetDetails(const DocDetails& details)
{
	m_detais.CopyFrom(details);
}

size_t DocSnapshot::GetItemsCount() const
{
	return m_Items.size();
}

const DocCowItem& DocSnapshot::GetItem(size_t index) const
{
	return *m_Items[index];
}

void DocSnapshot::AddItem(const DocCowItem& item)
{
	m_Items.push_back(new DocCowItem(item));
}

DocSnapshotItemsRange DocSnapshot::Items() const
{
	return  DocSnapshotItemsRange(m_Items.begin(),m_Items.end());
}

std::wstring DocSnapshot::ToString() const
{
	std::wstringstream s;
	
	s << ::ToString(GetDetails());
	s << L", items[" << this->GetItemsCount() << L"]:\n";
	
	BOOST_FOREACH(const DocCowItem* item,Items())
	{
		s << L"{" << item->ToString() << L"}\n";
	}
	
	return s.str();
}
