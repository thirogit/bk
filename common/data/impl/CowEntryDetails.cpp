#include "stdafx.h"
#include "CowEntryDetails.h"
#include <data/IIdable.h>

CowEntryDetails::CowEntryDetails() : m_stockId(NULL_ID),m_classId(NULL_ID)
{
}

void CowEntryDetails::CopyFrom(const CowEntryDetails& src)
{
	m_weight = src.GetWeight();
	m_stockId = src.GetStockId();
	m_classId = src.GetClassId();
	m_group = src.GetGroup();	
}

void CowEntryDetails::CopyFrom(const ICowEntry* pSrc)
{
	m_weight = pSrc->GetWeight();
	m_stockId = pSrc->GetStockId();
	m_classId = pSrc->GetClassId();
	m_group = pSrc->GetGroup();
}

const NullDecimal& CowEntryDetails::GetWeight() const
{
	return m_weight;
}

uint32_t CowEntryDetails::GetStockId() const
{
	return m_stockId;
}

uint32_t CowEntryDetails::GetClassId() const
{
	return m_classId;
}

const NullInt& CowEntryDetails::GetGroup() const
{
	return m_group;
}

void CowEntryDetails::SetWeight(const NullDecimal& weight)
{
	m_weight = weight;
}

void CowEntryDetails::SetStockId(uint32_t stockId)
{
	m_stockId = stockId;
}

void CowEntryDetails::SetClassId(uint32_t classId)
{
	m_classId = classId;
}

void CowEntryDetails::SetGroup(const NullInt& group)
{
	m_group = group;
}


