#include "stdafx.h"
#include "BuyInvoice.h"
#include <utils/JsonUtils.h>

BuyInvoice::BuyInvoice(uint32_t id,InvoiceScope scope) : InvoiceTemplate<IBuyInvoice>(id,scope)
{
}

InvoiceDirection BuyInvoice::GetInvoiceDirection() const
{
	return InvoiceDirection_Buy;
}

BuyInvoice* BuyInvoice::Clone() const
{
	BuyInvoice* pClone = new BuyInvoice(GetId(),GetInvoiceScope());
	pClone->CopyFrom(this);
	return pClone;
}

static const wchar_t* _id_attribute_name = L"_id";
static const wchar_t* scope_attribute_name = L"scope";
static const wchar_t* invoicetype_attribute_name = L"invoice_type";
static const wchar_t* invoiceno_attribute_name = L"invoice_no";
static const wchar_t* hent_attribute_name = L"hent";
static const wchar_t* details_attribute_name = L"details";
static const wchar_t* cows_attribute_name = L"cows";
static const wchar_t* deductions_attribute_name = L"deductions";


BuyInvoice BuyInvoice::FromJson(const json::value& value)
{
	uint32_t id = value[_id_attribute_name].as_integer();
	InvoiceScope scope = json_InvoiceScope(value[scope_attribute_name]);

	BuyInvoice result(id, scope);

	result.m_invoicetype = json_InvoiceType(value[invoicetype_attribute_name]);
	result.m_invoiceno = value[invoiceno_attribute_name].as_integer();



	result.m_Hent = InvoiceHent::FromJson(value[hent_attribute_name]);
	result.m_Deductions = std::move(InvoiceDeductions::FromJson(value[deductions_attribute_name]));
	result.m_Details = InvoiceDetails::FromJson(value[details_attribute_name]);
	result.m_Content = std::move(InvoiceContent::FromJson(value[cows_attribute_name]));

	return std::move(result);
}

json::value BuyInvoice::ToJson() const
{
	json::value result;

	result[_id_attribute_name] = json::value(this->m_invoiceid);
	result[scope_attribute_name] = json_InvoiceScope(this->m_invoicescope);
	result[invoicetype_attribute_name] = json_InvoiceType(this->m_invoicetype);
	result[invoiceno_attribute_name] = json::value(this->m_invoiceno);

	result[hent_attribute_name] = this->m_Hent.ToJson();
	result[deductions_attribute_name] = this->m_Deductions.ToJson();
	result[details_attribute_name] = this->m_Details.ToJson();
	result[cows_attribute_name] = this->m_Content.ToJson();

	return result;
}
