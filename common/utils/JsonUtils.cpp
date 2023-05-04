#include "stdafx.h"
#include "JsonUtils.h"
#include <string/TextUtils.h>
#include <data/henttypes/HentTypes.h>
#include "BoolUtil.h"
#include <data/invoicetypes/InvoiceTypes.h>
#include <data/invoicescopes/InvoiceScopes.h>
#include <data/payways/PayWays.h>
#include <data/motives/Motives.h>

const int json_decimal_precision = 5;
	

json::value j::operator[](const wchar_t* field)
{
	if (!object.has_field(field))
		json::value::null();

	return object[field];
}

j::j(const json::value& obj) : object(obj)
{
}


json::value json_emptyToNull(const std::wstring& s)
{
	if (s.empty())
	{
		return json::value();
	}

	return json::value(s);
}

std::wstring json_nullToEmpty(const json::value& value)
{
	if (value.is_null())
	{
		return std::wstring();
	}

	return value.as_string();
}

json::value json_CowSex(const CowSex& sex)
{
	if (sex.IsXX())
		return json::value(L"XX");

	if (sex.IsXY())
		return json::value(L"XY");

	return json::value::null();
}


json::value json_DateTime(const DateTime& dt, DateTime::DateFormat fmt/* = DateTime::YYYYdashMMdashDD_HHcolonMI*/)
{
	if (dt.IsNull())
		return json::value::null();

	return json::value(dt.GetDateInFmt(fmt));
}

DateTime json_DateTime(const json::value& jsonDtTm, DateTime::DateFormat fmt/* = DateTime::YYYYdashMMdashDD_HHcolonMI*/)
{
	if (jsonDtTm.is_null())
		return DateTime();

	return DateTime::ParseDate(fmt, jsonDtTm.as_string());
}


json::value json_DayDate(const DateTime& dt)
{
	if(dt.IsNull())
		return json::value::null();

	return json::value(dt.GetDateInFmt(DateTime::YYYYdashMMdashDD));
}

json::value json_Decimal(const Decimal& d)
{
	return json::value(d.ToString(json_decimal_precision));
}

json::value json_NullDecimal(const NullDecimal& d)
{
	if (d.IsNull())
		return json::value::null();
	
	return json::value(d.ToString(json_decimal_precision));
}

NullDecimal json_NullDecimal(const json::value& jsonDecimal)
{
	if (jsonDecimal.is_null())
		return NullDecimal();

	if (jsonDecimal.is_string())
	{
		return NullDecimal::FromString(jsonDecimal.as_string());
	}
	else
	{
		return NullDecimal::FromString(TextUtils::Format(L"%d", jsonDecimal.as_double()));
	}
}



DateTime json_DayDate(const json::value& value)
{
	if (value.is_null())
		return DateTime();

	return DateTime::ParseDate(DateTime::YYYYdashMMdashDD,value.as_string());
}

NullInt json_NullInt(const json::value& jsonInt)
{
	if (jsonInt.is_null())
		return NullInt();

	if (jsonInt.is_string())
	{
		return NullInt::FromString(jsonInt.as_string());
	}

	return NullInt(jsonInt.as_integer());
}

json::value json_NullInt(const NullInt& i)
{
	if (i.IsNull())
		return json::value::null();

	return json::value(i.ToString());
}


Decimal json_Decimal(const json::value& jsonDecimal)
{
	if (jsonDecimal.is_null())
		throw json::json_exception(L"cannot parse decimal from null");

	if (jsonDecimal.is_string())
	{
		return Decimal::FromString(jsonDecimal.as_string());
	}
	else
	{
		return Decimal::FromString(TextUtils::Format(L"%d",jsonDecimal.as_double()));
	}
}

CowSex json_CowSex(const json::value& jsonSex)
{
	if (jsonSex.is_null())
		return CowSex();

	std::wstring sSex = jsonSex.as_string();

	if (sSex == L"XX")
		return CowSex(SEX_XX);

	if (sSex == L"XY")
		return CowSex(SEX_XY);	

	return CowSex();
}

json::value json_HentType(HentType henttype)
{
	return json::value(HentTypes::ToString(henttype));
}

HentType json_HentType(const json::value& jsonHentType)
{
	return HentTypes::FromString(jsonHentType.as_string());
}

Latitude json_Latitude(const json::value& jsonLatitude)
{
	NullDouble d = NullDouble::FromString(jsonLatitude.as_string());

	return Latitude::FromDouble(d);
}

json::value json_Latitude(const Latitude& lati)
{
	return json::value(lati.AsDouble().ToString());
}

Longitude json_Longitude(const json::value& jsonLongitude)
{
	NullDouble d = NullDouble::FromString(jsonLongitude.as_string());
	return Longitude::FromDouble(d);
}

json::value json_Longitude(const Longitude& longi)
{
	return json::value(longi.AsDouble().ToString());
}

bool json_bool(const json::value& jsonBool, bool whenNull/* = false*/)
{
	if (jsonBool.is_null())
	{
		return whenNull;
	}

	if (jsonBool.is_boolean())
	{
		return jsonBool.as_bool();
	}

	return BoolUtil::FromString(jsonBool.as_string());
}

json::value json_bool(bool b)
{
	return json::value(BoolUtil::ToString(b));
}

json::value json_InvoiceScope(InvoiceScope invoicescope)
{
	return json::value(InvoiceScopes::ToString(invoicescope));
}

json::value json_InvoiceType(InvoiceType invoicetype)
{
	return json::value(InvoiceTypes::ToString(invoicetype));
}

InvoiceType json_InvoiceType(const json::value& jsonInvoiceType)
{
	return InvoiceTypes::FromString(jsonInvoiceType.as_string());
}

json::value json_PayWay(PayWay payway)
{
	return json::value(PayWays::ToString(payway));
}

PayWay json_PayWay(const json::value& jsonPayWay)
{
	return PayWays::FromString(jsonPayWay.as_string());
}

InvoiceScope json_InvoiceScope(const json::value& jsonInvoiceScope)
{
	return InvoiceScopes::FromString(jsonInvoiceScope.as_string());
}

json::value json_Time(const Time& t)
{
	if (t.IsNull())
		return json::value::null();

	return json::value(t.GetTime(Time::HHcolonMI));

}

Time json_Time(const json::value& jsonTm)
{
	if (jsonTm.is_null())
		return Time();

	return Time::ParseTime(Time::HHcolonMI, jsonTm.as_string());
}

json::value json_Motive(Motive motive)
{
	return json::value(Motives::ToString(motive));
}

Motive json_Motive(const json::value& jsonMotive)
{
	return Motives::FromString(jsonMotive.as_string());
}


json::value json_HentNo(const HentNo& hentNo)
{
	if (hentNo.IsNull())
	{
		return json::value::null();
	}

	return json::value(hentNo.ToString());
}

HentNo json_HentNo(const json::value& jsonHentNo)
{
	HentNo result;
	if (!jsonHentNo.is_null())
	{
		result = HentNoFactory::FromString(jsonHentNo.as_string());
	}
	return result;
}


json::value json_DateTimeZ(const DateTime& dt)
{

	if (dt.IsNull())
		return json::value::null();

	return json::value(dt.GetDateInFmt(DateTime::YYYYdashMMdashDDTHHcMIcSS) + L"Z");
}


json::value json_NullDecimalAsFloat(const NullDecimal& d)
{
	if (d.IsNull())
		return json::value::null();

	std::wstring sDecimal = d.ToString(5);
	return json::value(stod(sDecimal));
}

json::value json_DecimalAsFloat(const Decimal& d)
{
	std::wstring sDecimal = d.ToString(5);
	return json::value(stod(sDecimal));
}

