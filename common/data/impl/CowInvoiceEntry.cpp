#include "stdafx.h"
#include "CowInvoiceEntry.h"
#include "../IIdable.h"

CowInvoiceEntry::CowInvoiceEntry(uint32_t cowId) : m_cowId(cowId)
{
}

CowInvoiceEntry::CowInvoiceEntry(const CowInvoiceEntry& src) : m_cowId(src.m_cowId)
{
	CopyFrom(&src);
}

CowInvoiceEntry::CowInvoiceEntry(const ICowInvoiceEntry* src) : m_cowId(src->GetCowId())
{
	CopyFrom(src);
}

void CowInvoiceEntry::CopyFrom(const ICowInvoiceEntry* src)
{
	m_details.SetClassId(src->GetClassId());
	m_details.SetWeight(src->GetWeight());
	m_details.SetPrice(src->GetPrice());
}

void CowInvoiceEntry::SetFrom(const CowInvoiceEntryDetails& details)
{
	m_details.CopyFrom(details);
}

uint32_t CowInvoiceEntry::GetCowId() const
{
	return m_cowId;
}

uint32_t CowInvoiceEntry::GetClassId() const
{
	return m_details.GetClassId();
}

const Decimal& CowInvoiceEntry::GetWeight() const
{
	return m_details.GetWeight();
}

const Decimal& CowInvoiceEntry::GetPrice() const
{
	return m_details.GetPrice();
}

void CowInvoiceEntry::SetClassId(uint32_t classId)
{
	m_details.SetClassId(classId);
}

void CowInvoiceEntry::SetWeight(const Decimal& weight)
{
	m_details.SetWeight(weight);
}

void CowInvoiceEntry::SetPrice(const Decimal& price)
{
	m_details.SetPrice(price);
}

static const wchar_t* cowid_attribute_name = L"cow_id";
static const wchar_t* details_attribute_name = L"details";

CowInvoiceEntry CowInvoiceEntry::FromJson(const json::value& value)
{
	uint32_t cowId = value[cowid_attribute_name].as_integer();

	CowInvoiceEntry result(cowId);
	result.m_details = CowInvoiceEntryDetails::FromJson(value[details_attribute_name]);
	return result;
}

json::value CowInvoiceEntry::ToJson() const
{
	json::value result;

	result[cowid_attribute_name] = json::value(this->m_cowId);
	result[details_attribute_name] = this->m_details.ToJson();


	return result;
}
