#ifndef __JSONUTILS_H__
#define __JSONUTILS_H__

#include <string>
#include <casajson/json.h>
#include <data/types/CowSex.h>
#include <types/DateTime.h>
#include <types/Decimal.h>
#include <types/NullDecimal.h>
#include <types/NullInt.h>
#include <types/Longitude.h>
#include <types/Latitude.h>
#include <data/HentType.h>
#include <data/IInvoice.h>
#include <data/Motive.h>
#include <types/Time.h>

struct j
{
	j(const json::value& obj);
	json::value operator[](const wchar_t*);
	const json::value& object;
};

json::value json_emptyToNull(const std::wstring& s);

std::wstring json_nullToEmpty(const json::value& value);

json::value json_CowSex(const CowSex& sex);

CowSex json_CowSex(const json::value& jsonSex);

json::value json_DayDate(const DateTime& dt);

DateTime json_DayDate(const json::value& jsonDtTm);

json::value json_Decimal(const Decimal& d);

Decimal json_Decimal(const json::value& jsonDecimal);

json::value json_NullDecimal(const NullDecimal& d);

NullDecimal json_NullDecimal(const json::value& jsonDecimal);

NullInt json_NullInt(const json::value& jsonInt);

json::value json_NullInt(const NullInt& nullInt);

json::value json_DateTime(const DateTime& dt, DateTime::DateFormat fmt = DateTime::YYYYdashMMdashDD_HHcolonMI);

DateTime json_DateTime(const json::value& jsonDtTm, DateTime::DateFormat fmt = DateTime::YYYYdashMMdashDD_HHcolonMI);

json::value json_HentType(HentType henttype);
HentType json_HentType(const json::value& jsonHentType);

Latitude json_Latitude(const json::value& jsonLatitude);
json::value json_Latitude(const Latitude& lati);

Longitude json_Longitude(const json::value& jsonLongitude);
json::value json_Longitude(const Longitude& longi);

bool json_bool(const json::value& jsonBool, bool whenNull = false);
json::value json_bool(bool b);


json::value json_InvoiceScope(InvoiceScope invoicescope);
InvoiceScope json_InvoiceScope(const json::value& jsonInvoiceScope);

json::value json_InvoiceType(InvoiceType invoicetype);
InvoiceType json_InvoiceType(const json::value& jsonInvoiceType);

json::value json_PayWay(PayWay payway);
PayWay json_PayWay(const json::value& jsonPayWay);

json::value json_Motive(Motive motive);
Motive json_Motive(const json::value& jsonMotive);

json::value json_Time(const Time& t);
Time json_Time(const json::value& jsonTm);

json::value json_HentNo(const HentNo& hentNo);
HentNo json_HentNo(const json::value& jsonHentNo);
json::value json_DateTimeZ(const DateTime& dt);
json::value json_NullDecimalAsFloat(const NullDecimal& d);
json::value json_DecimalAsFloat(const Decimal& d);




#endif
