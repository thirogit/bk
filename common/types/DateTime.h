#ifndef __DATETIME_H__
#define __DATETIME_H__

#include <stdint.h>
#include "Nullable.h"
#include <map>
#include <boost/optional.hpp>
#include "../exception/TagException.h"


#define SECONDS_IN_DAY (24*60*60)
#define SECONDS_IN_YEAR (365 * SECONDS_IN_DAY)     

/*
 * Macro to determine if a given year, expressed as the number of years since
 * 1900, is a leap year.
 */
#define _IS_LEAP_YEAR(y)        (((y % 4 == 0) && (y % 100 != 0)) || ((y) % 400 == 0))

#define _BASE_YEAR         1970                   /* 1970 is the base year */
#define _MAX_YEAR          2038                  /* 2038 is the max year */

#define _LEAP_YEAR_ADJUST  17                   /* Leap years 1900 - 1970 */

/*
 * Number of leap years from 1970 up to, but not including, the specified year
 * (expressed as the number of years since 1900).
 */
#define _ELAPSED_LEAP_YEARS(y)  (((y - 1)/4) - ((y - 1)/100) + ((y + 299)/400) \
                                - _LEAP_YEAR_ADJUST)

//#define _BASE_DOW          4                    /* 01-01-70 was a Thursday */

struct tag_DateTimeException{};
typedef TagException<tag_DateTimeException> DateTimeException;


class DateTime : public Nullable
{
public:
	enum DateFormat
	{
		YYYYdotMMdotDD,
		YYYYMMDD,
		DDMMYYYY,
		YYYYdashMMdashDD, //YYYY-MM-DD
		YYYYdotMMdotDD_HHcolonMMcolonSS, //30.12.2007 11:17:01
		YYYYMMDDHHMISS,
		DDdashMMdashYYYY,
		DDSMMSYYYY, //DD/MM/YYYY
		YYYYdashMMdashDD_HHcolonMI, //YYYY-MM-DD HH:MI
		YYYYdashMMdashDDTHHcMIcSS //YYYY-MM-DDTHH:MI:SS
	};

private:
	class InternalResresentation
	{
	public:
		InternalResresentation();

		uint32_t m_year;
		uint32_t m_month1_12;
		uint32_t m_day1_31;

		uint32_t hour0_23;
		uint32_t minute0_59;
		uint32_t sec0_59;
	};

public:
	DateTime();
	DateTime(time_t secsSinceEpoch);	
	DateTime(uint32_t year,uint32_t month,uint32_t day);
	DateTime(uint32_t year,uint32_t month,uint32_t day,uint32_t hour,uint32_t min,uint32_t sec);
	DateTime(const DateTime& copy);
	bool IsNull() const;
	std::wstring GetDateInFmt(DateFormat dtFmt) const;
	std::wstring GetDayDate() const;
	std::wstring GetFullDate() const;
	
	int GetDiffrenceInSeconds(const DateTime& dt) const;
	int GetDiffrenceInDays(const DateTime& dt) const;
	DateTime& operator=(const DateTime& right);

	uint32_t GetYear() const;
	uint32_t GetMonth() const;
	uint32_t GetDay() const;
	uint32_t GetHour() const;
	uint32_t GetMinutes() const;
	uint32_t GetSeconds() const;

	void SetYearMonthDay(uint32_t year,uint32_t month,uint32_t day);
	void SetHourMinSec(uint32_t hour,uint32_t minutes,uint32_t seconds);

	bool EqualDay(const DateTime& dt) const;
	uint32_t SecondsSinceEpoh() const;
	void SetDateTime(time_t secsSinceEpoch);
	
	void MakeNull();

	static DateTime ParseDate(DateFormat fmt, const std::wstring& inputDt);
	static const DateTime now();

	bool SetTmStruct(struct tm &tm1) const;

	DateTime ToMonthBegin() const;
	DateTime ToMonthEnd() const;
	DateTime AddMonth(int months) const;
	DateTime AddDays(int days) const;

	
protected:
	boost::optional<InternalResresentation> m_Value;
		
};

	bool operator>=(const DateTime& left, const DateTime& right);
	bool operator<=(const DateTime& left, const DateTime& right);
	bool operator>(const DateTime& left, const DateTime& right);
	bool operator<(const DateTime& left, const DateTime& right);
	bool operator==(const DateTime& left, const DateTime& right);
	bool operator!=(const DateTime& left, const DateTime& right);
	
/////////////////////////////////////////////////////////////////////////////////////////

typedef struct _tagDATETIMEFMTS
{
	DateTime::DateFormat dtFmt;
	const wchar_t* strFmt;
} DATETIMEFMTS;

class DateTimeFormatter
{
private:
	DateTimeFormatter();
	std::map<DateTime::DateFormat,const wchar_t*> _datetimefmtsMap;
	static DateTimeFormatter instance;
public:
	static std::wstring FormatDateTime(const DateTime& dt,DateTime::DateFormat dtFmt);

};

class DateTimeParser
{
private:
	DateTimeParser();
	std::map<DateTime::DateFormat,const wchar_t*> _datetimefmtsMap;
	static DateTimeParser instance;
public:
	static DateTime ParseDateTime(DateTime::DateFormat dtFmt, const std::wstring& dateStr);
};


#endif
