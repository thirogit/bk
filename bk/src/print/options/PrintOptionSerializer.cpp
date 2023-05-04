#include "stdafx.h"
#include "PrintOptionSerializer.h"
#include <string/IntString.h>

std::wstring PrintOptionSerializer::GetStringValue(const PrintOption& option)
{
	return option.get();
}

NullInt PrintOptionSerializer::GetIntegerValue(const PrintOption& option)
{
	NullInt result;
	std::wstring sInt = option.get();
	if (sInt.length() > 0)
		result = _wtoi(sInt.c_str());

	return result;
}

uint32_t PrintOptionSerializer::GetDisplacementEventsValue(const PrintOption& option)
{
	return std::wcstoul(option.get().c_str(),NULL,10);
}

DecimalFormat PrintOptionSerializer::GetDecimalFormatValue(const PrintOption& option)
{
	std::wstring sDecimalFmt = option.get();
	wchar_t decimalPoint = sDecimalFmt.at(0);
	std::wstring sPrecision = sDecimalFmt.substr(1);
	int precision = _wtoi(sPrecision.c_str());
	return DecimalFormat(precision, decimalPoint);
}

bool PrintOptionSerializer::GetBooleanValue(const PrintOption& option)
{
	std::wstring sValue = option.get();
	wchar_t boolChar = sValue[0];
	return boolChar == L't' || boolChar == L'Y' || boolChar == L'y' || boolChar == L'1';
}

NullDecimal PrintOptionSerializer::GetDecimalValue(const PrintOption& option)
{
	NullDecimal result;
	std::wstring sValue = option.get();
	if (!sValue.empty())
	{
		result = Decimal::FromString(sValue);
	}
	return result;	
}

DateTime PrintOptionSerializer::GetDateTimeValue(const PrintOption& option)
{
	DateTime result;
	std::wstring sValue = option.get();
	if (!sValue.empty())
	{
		result = DateTime::ParseDate(DateTime::YYYYMMDDHHMISS, sValue);
	}
	return result;
}

void PrintOptionSerializer::SetValue(PrintOption& option, const DateTime& dt)
{
	option.set(dt.GetDateInFmt(DateTime::YYYYMMDDHHMISS));
}

void PrintOptionSerializer::SetValue(PrintOption& option,const std::wstring& s)
{
	option.set(s);
}

void PrintOptionSerializer::SetValue(PrintOption& option, const NullInt& i)
{
	option.set(i.ToString());
}

void PrintOptionSerializer::SetValue(PrintOption& option,uint32_t uiEvents)
{
	option.set(IntString(uiEvents));
}

void PrintOptionSerializer::SetValue(PrintOption& option,const DecimalFormat& decFmt)
{
	std::wstring sDecFmt;
	sDecFmt += decFmt.GetDecimalPoint();
	sDecFmt += IntString(decFmt.GetPrecision());
	option.set(sDecFmt);
}

void PrintOptionSerializer::SetValue(PrintOption& option,bool b)
{
	std::wstring sBool;
	sBool += (b ? L't' : L'f');
	option.set(sBool);
}

void PrintOptionSerializer::SetValue(PrintOption& option, const NullDecimal& d)
{
	option.set(d.ToString(DecimalFormat(6,L'.')));
}
