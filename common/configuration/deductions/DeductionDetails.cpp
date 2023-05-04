#include "stdafx.h"
#include "DeductionDetails.h"
#include <utils/JsonUtils.h>

DeductionDetails::DeductionDetails()
{
}

DeductionDetails::DeductionDetails(const DeductionDetails& src)
{
	CopyFrom(src);
}

DeductionDetails& DeductionDetails::operator=(const DeductionDetails& src)
{
	CopyFrom(src);
	return *this;
}


void DeductionDetails::CopyFrom(const DeductionDetails& src)
{
	SetCode(src.GetCode());
	SetReason(src.GetReason());
	SetFraction(src.GetFraction());	
}


const std::wstring& DeductionDetails::GetCode() const
{
	return m_code;
}

const std::wstring& DeductionDetails::GetReason() const
{
	return m_reason;
}

const Decimal&	DeductionDetails::GetFraction() const
{
	return m_fraction;
}

void DeductionDetails::SetCode(const std::wstring& sCode)
{
	m_code = sCode;
}

void DeductionDetails::SetReason(const std::wstring& sReason)
{
	m_reason = sReason;
}

void DeductionDetails::SetFraction(const Decimal& fraction)
{
	m_fraction = fraction;
}

static const wchar_t* code_attribute_name = L"code";
static const wchar_t* reason_attribute_name = L"reason";
static const wchar_t* fraction_attribute_name = L"fraction";

DeductionDetails DeductionDetails::FromJson(const json::value& value)
{
	DeductionDetails result;

	result.m_fraction = json_Decimal(value[fraction_attribute_name]);
	result.m_code = value[code_attribute_name].as_string();
	result.m_reason = value[reason_attribute_name].as_string();

	return result;
}

json::value DeductionDetails::ToJson() const
{
	json::value result;

	result[fraction_attribute_name] = json_Decimal(this->m_fraction);
	result[code_attribute_name] = json::value(this->m_code);
	result[reason_attribute_name] = json::value(this->m_reason);


	return result;
}