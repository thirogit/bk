#include "stdafx.h"
#include "InvoiceDeduction.h"
#include <utils/JsonUtils.h>

InvoiceDeduction::InvoiceDeduction() : m_enabled(false)
{

}

InvoiceDeduction::InvoiceDeduction(const InvoiceDeduction& src)
{
	CopyFrom(&src);
}

InvoiceDeduction::InvoiceDeduction(const IInvoiceDeduction* src)
{
	CopyFrom(src);
}

InvoiceDeduction::InvoiceDeduction(const IDeductionDefinition* src)
{
	SetCode(src->GetCode());
	SetFraction(src->GetFraction());
	SetReason(src->GetReason());
	SetIsEnabled(src->GetIsEnabledByDefault());
}

void InvoiceDeduction::CopyFrom(const IInvoiceDeduction* src)
{
	CopyFrom(src->GetDetails());
	SetIsEnabled(src->GetIsEnabled());
}

InvoiceDeduction& InvoiceDeduction::operator=(const InvoiceDeduction& right)
{
	CopyFrom(&right);
	return *this;
}

void InvoiceDeduction::CopyFrom(const DeductionDetails& details)
{
	SetCode(details.GetCode());
	SetFraction(details.GetFraction());
	SetReason(details.GetReason());
}

DeductionDetails InvoiceDeduction::GetDetails() const
{
	DeductionDetails details;
	details.SetCode(this->GetCode());
	details.SetFraction(this->GetFraction());
	details.SetReason(this->GetReason());

	return details;
}

const std::wstring& InvoiceDeduction::GetCode() const
{
	return m_details.GetCode();
}

const std::wstring& 	InvoiceDeduction::GetReason() const
{
	return m_details.GetReason();
}

const Decimal&	InvoiceDeduction::GetFraction() const
{
	return m_details.GetFraction();
}

bool InvoiceDeduction::GetIsEnabled() const
{
	return m_enabled;
}

void InvoiceDeduction::SetCode(const std::wstring& sCode)
{
	m_details.SetCode(sCode);
}

void InvoiceDeduction::SetReason(const std::wstring& sReason)
{
	m_details.SetReason(sReason);
}

void InvoiceDeduction::SetFraction(const Decimal& fraction)
{
	m_details.SetFraction(fraction);
}

void InvoiceDeduction::SetIsEnabled(bool bEnabled)
{
	m_enabled = bEnabled;
}


static const wchar_t* enabled_attribute_name = L"enabled";
static const wchar_t* details_attribute_name = L"details";


InvoiceDeduction InvoiceDeduction::FromJson(const json::value& value)
{
	InvoiceDeduction result;

	result.m_enabled = json_bool(value[enabled_attribute_name]);
	result.m_details = DeductionDetails::FromJson(value[details_attribute_name]);
	return result;
}

json::value InvoiceDeduction::ToJson() const
{
	json::value result;

	result[enabled_attribute_name] = json_bool(this->m_enabled);
	result[details_attribute_name] = this->m_details.ToJson();


	return result;
}