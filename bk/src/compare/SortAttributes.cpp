#include "stdafx.h"
#include "SortAttributes.h"
#include <algorithm>
#include <arrays\DeepDelete.h>
#include <boost\foreach.hpp>

SortAttributes::SortAttributes()
{
}

SortAttributes::~SortAttributes()
{
	Clear();	
}

SortAttributes::SortAttributes(const SortAttributes& src)
{
	BOOST_FOREACH(SortAttribute* attribute,src.m_attributes)
	{
		m_attributes.push_back(new SortAttribute(*attribute));
	}
}

SortAttributes::SortAttributes(SortAttributes && src)
{
	std::copy(src.m_attributes.begin(),src.m_attributes.end(),std::back_inserter(m_attributes));
}

void SortAttributes::Clear()
{
	DeepDelete(m_attributes);
	m_attributes.clear();
}

size_t SortAttributes::Count() const
{
	return m_attributes.size();
}
	
void SortAttributes::AddAttribute(const SortAttribute& newAttribute)
{
	m_attributes.push_back(new SortAttribute(newAttribute));
}

void SortAttributes::SetAttribute(const SortAttribute& newAttribute)
{
	SortAttribute* attribute = FindAttribute(newAttribute.MemberId());

	if(attribute)
		attribute->Order(newAttribute.Order());
}

PtrEnumerator<const SortAttribute> SortAttributes::EnumAttributes() const
{
	return PtrEnumerator<const SortAttribute>(m_attributes.begin(),m_attributes.end());
}

void SortAttributes::RemoveAttribute(uint32_t memberId)
{
	boost::multi_index::index<AttributeIndex,tag_MemberId>::type& indexByMemberId = boost::multi_index::get<tag_MemberId>(m_attributes);

	indexByMemberId.erase(memberId);
	
}

SortAttribute* SortAttributes::FindAttribute(uint32_t memberId) const
{
	const boost::multi_index::index<AttributeIndex,tag_MemberId>::type& indexByMemberId = boost::multi_index::get<tag_MemberId>(m_attributes);

	auto it = indexByMemberId.find(memberId);
	if(it != indexByMemberId.end())
		return *it;

	return NULL;

}

const SortAttribute* SortAttributes::GetAttribute(uint32_t memberId) const
{
	return FindAttribute(memberId);
}
	
const SortAttribute* SortAttributes::GetAttributeAt(uint32_t index) const
{
	return m_attributes[index];
}

const SortAttribute* SortAttributes::operator[](uint32_t index) const
{
	return GetAttributeAt(index);
}
	
