#include "stdafx.h"
#include "CowInvoiceEntryDetails.h"
#include "../IIdable.h"
#include <utils/JsonUtils.h>

CowInvoiceEntryDetails::CowInvoiceEntryDetails() : m_classId(NULL_ID)
{
}


void CowInvoiceEntryDetails::CopyFrom(const ICowInvoiceEntry* src)
{
	m_classId = src->GetClassId();
	m_Weight = src->GetWeight();
	m_Price = src->GetPrice();
}

void CowInvoiceEntryDetails::CopyFrom(const CowInvoiceEntryDetails& src)
{
	m_classId = src.GetClassId();
	m_Weight = src.GetWeight();
	m_Price = src.GetPrice();
}

uint32_t CowInvoiceEntryDetails::GetClassId() const
{
	return m_classId;
}

const Decimal& CowInvoiceEntryDetails::GetWeight() const
{
	return m_Weight;
}

const Decimal& CowInvoiceEntryDetails::GetPrice() const
{
	return m_Price;
}

void CowInvoiceEntryDetails::SetClassId(uint32_t classId)
{
	m_classId = classId;
}

void CowInvoiceEntryDetails::SetWeight(const Decimal& weight)
{
	m_Weight = weight;
}

void CowInvoiceEntryDetails::SetPrice(const Decimal& price)
{
	m_Price = price;
}


static const wchar_t* weight_attribute_name = L"weight";
static const wchar_t* price_attribute_name = L"price";
static const wchar_t* classid_attribute_name = L"class_id";

CowInvoiceEntryDetails CowInvoiceEntryDetails::FromJson(const json::value& value)
{
	CowInvoiceEntryDetails result;

	result.m_Weight = json_Decimal(value[weight_attribute_name]);
	result.m_classId = value[classid_attribute_name].as_integer();
	result.m_Price = json_Decimal(value[price_attribute_name]);

	return result;
}

json::value CowInvoiceEntryDetails::ToJson() const
{
	json::value result;

	result[weight_attribute_name] = json_Decimal(this->m_Weight);
	result[classid_attribute_name] = json::value(this->m_classId);
	result[price_attribute_name] = json_Decimal(this->m_Price);


	return result;
}