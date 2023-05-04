#include "stdafx.h"
#include "CowEntry.h"
#include "../IIdable.h"

CowEntry::CowEntry(uint32_t cowId) : m_cowId(cowId)
{
}

void CowEntry::CopyFrom(ICowEntry* pSrc)
{
	m_details.SetClassId(pSrc->GetClassId());
	m_details.SetGroup(pSrc->GetGroup());
	m_details.SetStockId(pSrc->GetStockId());
	m_details.SetWeight(pSrc->GetWeight());

}

void CowEntry::SetFrom(const CowEntryDetails& details)
{
	m_details.CopyFrom(details);
}

const CowEntryDetails& CowEntry::GetDetails() const
{
	return m_details;
}

const NullDecimal& CowEntry::GetWeight() const
{
	return m_details.GetWeight();
}

uint32_t CowEntry::GetStockId() const
{
	return m_details.GetStockId();
}

uint32_t CowEntry::GetClassId() const
{
	return m_details.GetClassId();
}

const NullInt& CowEntry::GetGroup() const
{
	return m_details.GetGroup();
}

uint32_t CowEntry::GetCowId() const
{
	return m_cowId;
}
	
void CowEntry::SetWeight(const NullDecimal& weight)
{
	m_details.SetWeight(weight);
}

void CowEntry::SetStockId(uint32_t stockId)
{
	m_details.SetStockId(stockId);
}

void CowEntry::SetClassId(uint32_t classId)
{
	m_details.SetClassId(classId);
}

void CowEntry::SetGroup(const NullInt& group)
{
	m_details.SetGroup(group);
}


