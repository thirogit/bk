#include "stdafx.h"
#include "SortAttribute.h"

SortAttribute::SortAttribute(uint32_t memberId,SortOrder order) : m_Order(order),m_MemberId(memberId)
{
}

SortAttribute::SortAttribute(const SortAttribute& src) : m_Order(src.m_Order),m_MemberId(src.m_MemberId)
{
}

	
uint32_t SortAttribute::MemberId() const
{
	return m_MemberId;
}

SortOrder SortAttribute::Order() const
{
	return m_Order;
}

void SortAttribute::Order(SortOrder order)
{
	 m_Order = order;
}