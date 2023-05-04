#include "stdafx.h"
#include "DateTime.h"
#include "../ported/strptime.h"
#include "../string/TextUtils.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "boost/date_time/posix_time/posix_time.hpp"
#include <algorithm>

#undef min

namespace pt = boost::posix_time;

static DATETIMEFMTS _datetimefmts[] =
{
	{DateTime::YYYYdotMMdotDD,					L"%Y.%m.%d"},
	{DateTime::YYYYMMDD,						L"%Y%m%d"},
	{DateTime::DDMMYYYY,						L"%d%m%Y"},
	{DateTime::YYYYdashMMdashDD,				L"%Y-%m-%d"},
	{DateTime::YYYYdotMMdotDD_HHcolonMMcolonSS,	L"%Y.%m.%d %H:%M:%S"},
	{DateTime::YYYYdashMMdashDDTHHcMIcSS,	    L"%Y-%m-%dT%H:%M:%S"},
	{DateTime::YYYYMMDDHHMISS,					L"%Y%m%d%H%M%S"},
	{DateTime::DDdashMMdashYYYY,				L"%d-%m-%Y"},
	{DateTime::DDSMMSYYYY,						L"%d/%m/%Y"},
	{DateTime::YYYYdashMMdashDD_HHcolonMI,		L"%Y-%m-%d %H:%M"},
	{(DateTime::DateFormat)0,NULL}
};

static uint32_t _monthDays[] = {	0,
									31, // Jan
									0, // Feb
									31, // Mar
									30, // Apr
									31, // May
									30, // Jun
									31, // Jul
									31, // Aug
									30, // Sep
									31, // Oct
									30, // Nov
									31  // Dec
								};


DateTime::InternalResresentation::InternalResresentation() : m_year(0), m_month1_12(0), m_day1_31(0),hour0_23(0), minute0_59(0), sec0_59(0)
{
}
//#######################################################################################

DateTime::DateTime() 
{
}

DateTime::DateTime(time_t secsSinceEpoch)
{
	SetDateTime(secsSinceEpoch);
}

DateTime::DateTime(uint32_t year,uint32_t month,uint32_t day) 
{
	SetYearMonthDay(year,month,day);
	SetHourMinSec(0,0,0);
}
DateTime::DateTime(uint32_t year,uint32_t month,uint32_t day,uint32_t hour,uint32_t min,uint32_t sec)
{
	SetYearMonthDay(year,month,day);
	SetHourMinSec(hour,min,sec);
}
DateTime::DateTime(const DateTime& copy)
{
	operator=(copy);
}

DateTime& DateTime::operator=(const DateTime& right)
{
	m_Value = right.m_Value;
	return *this;
}

bool DateTime::IsNull() const
{
	return !m_Value.is_initialized();
}

std::wstring DateTime::GetDateInFmt(DateFormat dtFmt) const
{	
	return DateTimeFormatter::FormatDateTime(*this,dtFmt);
}
std::wstring DateTime::GetDayDate() const
{
	return GetDateInFmt(YYYYdashMMdashDD); 
}
std::wstring DateTime::GetFullDate() const
{
	return GetDateInFmt(YYYYdotMMdotDD_HHcolonMMcolonSS); 
}

uint32_t DateTime::GetYear() const
{
	if (IsNull()) return -1;
	return m_Value->m_year;
}
uint32_t DateTime::GetMonth() const
{
	if (IsNull()) return -1;
	return m_Value->m_month1_12;
}
uint32_t DateTime::GetDay() const
{
	if (IsNull()) return -1;
	return m_Value->m_day1_31;
}
uint32_t DateTime::GetHour() const
{
	if (IsNull()) return -1;
	return m_Value->hour0_23;
}
uint32_t DateTime::GetMinutes() const
{
	if (IsNull()) return -1;
	return m_Value->minute0_59;
}
uint32_t DateTime::GetSeconds() const
{
	if (IsNull()) return -1;
	return m_Value->sec0_59;
}

DateTime DateTime::ToMonthBegin() const
{
	DateTime result;
	if (!IsNull())
	{
		result.SetYearMonthDay(GetYear(), GetMonth(), 1);
		result.SetHourMinSec(0, 0, 0);
	}
	return result;
}

DateTime DateTime::ToMonthEnd() const
{
	DateTime result;
	if (!IsNull())
	{
		int month = GetMonth();
		int year = GetYear();
		int day = _monthDays[month];
		if (month == 2)
		{
			if (_IS_LEAP_YEAR(year))
			{
				day = 29;
			}
			else
			{
				day = 28;
			}
		}
		result.SetYearMonthDay(year, month, day);
		result.SetHourMinSec(23, 59, 59);
	}
	return result;
}

void DateTime::SetYearMonthDay(uint32_t year,uint32_t month,uint32_t day)
{
	InternalResresentation representation;
	
	if(year >=  _BASE_YEAR && year <= _MAX_YEAR)
	{
		if(month >= 1 && month <= 12)
		{
			if(month == 2)
			{
				 if(_IS_LEAP_YEAR(year))
				 {
					 if(day >= 1 && day <= 29)
					 {
						 representation.m_year = year;
						 representation.m_month1_12 = month;
						 representation.m_day1_31 = day;
						 m_Value = representation;						
						return;
					 }
					 else
					 {
						BOOST_THROW_EXCEPTION(DateTimeException(TextUtils::Format(L"%d is a wrong day for February in leap year", day)));						
					 }
				 }
				 else
				 {
					if(day >= 1 && day <= 28)
					{
						representation.m_year = year;
						representation.m_month1_12 = month;
						representation.m_day1_31 = day;
						m_Value = representation;
						return;
					}
					else
					{
						BOOST_THROW_EXCEPTION(DateTimeException(TextUtils::Format(L"%d is a wrong day for February", day)));
					}
				 }
				
			}
			else
			{
				if(day >= 1 && day <= _monthDays[month])
				{
					representation.m_year = year;
					representation.m_month1_12 = month;
					representation.m_day1_31 = day;
					m_Value = representation;
					return;
				}
				else
				{
					BOOST_THROW_EXCEPTION(DateTimeException(TextUtils::Format(L"%d is a wrong day for month %d", day, month)));					
				}
			}
		}
		else
		{
			BOOST_THROW_EXCEPTION(DateTimeException(TextUtils::Format(L"%d is a month",month)));			
		}
	}
	else
	{
		BOOST_THROW_EXCEPTION(DateTimeException(L"Accepting only dates from 1970 to 2038"));
	}	
}

void DateTime::SetHourMinSec(uint32_t hour,uint32_t minutes,uint32_t seconds)
{
	if (IsNull())
		BOOST_THROW_EXCEPTION(DateTimeException(L"Set day date first."));

	if((hour >= 0 && hour <= 23) &&
	   (minutes >= 0 && minutes <= 59) &&
	   (seconds >= 0 && seconds <= 59))
	{
		m_Value->hour0_23 = hour;
		m_Value->minute0_59 = minutes;
		m_Value->sec0_59 = seconds;		
	}
	else
	{
		BOOST_THROW_EXCEPTION(DateTimeException(L"Invalid time values"));
	}	
}
	
const DateTime DateTime::now()
{
	time_t nowTime = time(NULL);
	struct tm tm1;

#ifdef MSVC
	localtime_s(&tm1,&nowTime);
#else
	localtime_r(&nowTime,&tm1);
#endif
	
	return DateTime(tm1.tm_year+1900,tm1.tm_mon+1,tm1.tm_mday,tm1.tm_hour,tm1.tm_min,tm1.tm_sec);
}

int DateTime::GetDiffrenceInSeconds(const DateTime& dt) const
{
	if(dt.IsNull() || IsNull()) return -1;
	return dt.SecondsSinceEpoh()-SecondsSinceEpoh();
}

uint32_t DateTime::SecondsSinceEpoh() const
{
	if (IsNull())
		return 0;

        uint32_t tmpy, tmpmondays, tmpyeardays;
		int _days[] = { -1, 30, 58, 89, 119, 150, 180, 211, 242, 272, 303, 333, 364 };
        
		tmpy = GetYear();

               
        tmpmondays = _days[GetMonth()-1];
        if ( 
			!(tmpy & 3) && // in years 1970 - 2038 leap years are mod 4 
			(GetMonth() > 2) // if leap year and current month > Feb
			)
                tmpmondays++;

        tmpyeardays = (tmpy - _BASE_YEAR) * 365L + ((tmpy - 1L - 1900L) >> 2) - _LEAP_YEAR_ADJUST;        
        
        uint32_t totalDays = tmpyeardays + tmpmondays + GetDay();
        
        uint32_t totalHours = totalDays * 24L + GetHour();
        
        uint32_t totalMinutes = totalHours * 60L + GetMinutes();
                
        uint32_t totalSeconds = totalMinutes * 60L + GetSeconds(); 
      
		return totalSeconds;
}


void DateTime::SetDateTime(time_t secsSinceEpoch)
{
   
	time_t caltim = secsSinceEpoch;   /* calendar time to convert */
	int islpyr = 0;                 /* is-current-year-a-leap-year flag */
	time_t tmptim;
	int *mdays = NULL;                     /* pointer to days or lpdays */

	uint32_t tm_year = 0,tm_yday = 0,tm_mon = 0,tm_mday = 0,tm_sec = 0,
		tm_min = 0,tm_hour = 0;

	int _lpdays[] = {
         -1, 30, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365
	};

	int _days[] = {
			 -1, 30, 58, 89, 119, 150, 180, 211, 242, 272, 303, 333, 364
	};

    /*
     * Determine the years since 1900. Start by ignoring leap years.
     */
    tmptim = (caltim / SECONDS_IN_YEAR) + 70;
    caltim -= ((tmptim - 70) * SECONDS_IN_YEAR);

    /*
     * Correct for elapsed leap years
     */
    caltim -= (_ELAPSED_LEAP_YEARS(tmptim) * SECONDS_IN_DAY);

    /*
     * If we have underflowed the __time64_t range (i.e., if caltim < 0),
     * back up one year, adjusting the correction if necessary.
     */
    if ( caltim < 0 ) 
	{
        caltim += (time_t)SECONDS_IN_YEAR;
        tmptim--;
        if ( _IS_LEAP_YEAR(tmptim) ) 
		{
            caltim += SECONDS_IN_DAY;
            islpyr++;
        }
    }
    else
        if ( _IS_LEAP_YEAR(tmptim) )
            islpyr++;

    /*
     * tmptim now holds the value for tm_year. caltim now holds the
     * number of elapsed seconds since the beginning of that year.
     */
    tm_year = tmptim;

    /*
     * Determine days since January 1 (0 - 365). This is the tm_yday value.
     * Leave caltim with number of elapsed seconds in that day.
     */
    tm_yday = (uint32_t)(caltim / SECONDS_IN_DAY);
    caltim -= (time_t)(tm_yday) * SECONDS_IN_DAY;

    /*
     * Determine months since January (0 - 11) and day of month (1 - 31)
     */
    if ( islpyr )
        mdays = _lpdays;
    else
        mdays = _days;


    for ( tmptim = 1 ; mdays[tmptim] < tm_yday ; tmptim++ ) ;

    tm_mon = --tmptim;

    tm_mday = tm_yday - mdays[tmptim];

    /*
     *  Determine hours since midnight (0 - 23), minutes after the hour
     *  (0 - 59), and seconds after the minute (0 - 59).
     */
    tm_hour = (uint32_t)(caltim / 3600);
    caltim -= tm_hour * 3600L;

    tm_min = (uint32_t)(caltim / 60);
    tm_sec = (uint32_t)(caltim - tm_min * 60);

	SetYearMonthDay(tm_year+1900,tm_mon+1,tm_mday);
	SetHourMinSec(tm_hour,tm_min,tm_sec);
	
       
}


bool DateTime::SetTmStruct(struct tm &tm1) const
{
	if(IsNull()) return false;
	
	memset(&tm1,0,sizeof(struct tm));

	tm1.tm_year = m_Value->m_year-1900;
	tm1.tm_mon = m_Value->m_month1_12 - 1;
	tm1.tm_mday = m_Value->m_day1_31;
	tm1.tm_hour = m_Value->hour0_23;
	tm1.tm_min = m_Value->minute0_59;
	tm1.tm_sec = m_Value->sec0_59;

	return true;
}

int DateTime::GetDiffrenceInDays(const DateTime& dt) const
{
	if(dt.IsNull() || IsNull()) return -1;
	return GetDiffrenceInSeconds(dt) / (SECONDS_IN_DAY);	
}

void DateTime::MakeNull()
{
	m_Value.reset();
}

DateTime DateTime::AddMonth(int months) const
{
	DateTime result;
	if (!IsNull())
	{
		int month = GetMonth() + months%12;
		int year = GetYear() + months/12;
		int day = GetDay();

		if (month > 12)
		{
			year++;
			month = 1;
		}
		else if (month < 1)
		{
			year--;
			month = 12;
		}

		if (month == 2)
		{
			int maxFebDays = (_IS_LEAP_YEAR(year)) ? 29 : 28;			
			day = std::min(day, maxFebDays);				
		}
		result.SetYearMonthDay(year, month, day);		
	}
	return result;
}

DateTime DateTime::AddDays(int days) const
{
	return DateTime(SecondsSinceEpoh() + (days *SECONDS_IN_DAY));
}

DateTime DateTime::ParseDate(DateFormat fmt,const std::wstring& inputDt)
{
	DateTime r;

	if (!inputDt.empty())
	{
		r = DateTimeParser::ParseDateTime(fmt, inputDt);
	}
	return r;
}

bool DateTime::EqualDay(const DateTime& dt) const
{
	return GetYear() == dt.GetYear() && GetMonth() == dt.GetMonth() && GetDay() == dt.GetDay();
}

bool operator>=(const DateTime& left, const DateTime& right)
{
	if(left.IsNull() && right.IsNull()) return true;
	if(left.IsNull() || right.IsNull()) return false;

	return	left.SecondsSinceEpoh()   >= right.SecondsSinceEpoh();
	
}

bool operator<=(const DateTime& left, const DateTime& right)
{
	if(left.IsNull() && right.IsNull()) return true;
	if(left.IsNull() || right.IsNull()) return false;

	return left.SecondsSinceEpoh() <= right.SecondsSinceEpoh();
	
}



bool operator>(const DateTime& left, const DateTime& right)
{
	if(left.IsNull())
	{
		return false;
	}
	else
		if(right.IsNull()) return true;

	return left.SecondsSinceEpoh() > right.SecondsSinceEpoh();
		
}

bool operator<(const DateTime& left, const DateTime& right)
{
	return (right > left);
}


bool operator==(const DateTime& left, const DateTime& right)
{
	if(left.IsNull() && right.IsNull()) return true;
	if(right.IsNull() || left.IsNull()) return false;

	return left.SecondsSinceEpoh() == right.SecondsSinceEpoh();
}

bool operator!=(const DateTime& left, const DateTime& right)
{
	return !(left == right);
}

DateTimeFormatter DateTimeFormatter::instance;

DateTimeFormatter::DateTimeFormatter()
{
	DATETIMEFMTS *dateFmt = _datetimefmts;
	while(dateFmt->strFmt)
	{
		_datetimefmtsMap[dateFmt->dtFmt] = dateFmt->strFmt;
		dateFmt++;
	}
}

std::wstring DateTimeFormatter::FormatDateTime(const DateTime& dt,DateTime::DateFormat dtFmt)
{
	std::wstring result;
	if(!dt.IsNull())
	{
		auto fmtIt = instance._datetimefmtsMap.find(dtFmt);
		if(fmtIt != instance._datetimefmtsMap.end() )
		{
			struct tm tm1;
			wchar_t buffer[100];
			dt.SetTmStruct(tm1);
			wcsftime(buffer, sizeof(buffer), fmtIt->second, &tm1);
			result = buffer;
		}
		else
			BOOST_THROW_EXCEPTION(DateTimeException(L"FormatDateTime(): Unknown format specified"));	
	}
	return result;
}

/***************************************************************************************/

DateTimeParser DateTimeParser::instance;

DateTimeParser::DateTimeParser()
{
	DATETIMEFMTS *dateFmt = _datetimefmts;
	while(dateFmt->strFmt)
	{
		_datetimefmtsMap[dateFmt->dtFmt] = dateFmt->strFmt;
		dateFmt++;
	}
}
	
DateTime DateTimeParser::ParseDateTime(DateTime::DateFormat dtFmt, const std::wstring& dateStr)
{
	struct tm tm1;
	memset(&tm1,0,sizeof(tm1));

	DateTime dt;
		
	auto fmtIt = instance._datetimefmtsMap.find(dtFmt);

	if(fmtIt != instance._datetimefmtsMap.end())
	{

		std::wstringstream ss(dateStr);
		std::wstring format = fmtIt->second;
		pt::wtime_input_facet *df = new pt::wtime_input_facet(format);
		// The order of the date is reversed.
		ss.imbue(std::locale(ss.getloc(), df));
		pt::ptime t;  
		try
		{
			ss >> t;
			
			auto date = t.date();
			auto time = t.time_of_day();

			dt.SetYearMonthDay(date.year(), date.month(), date.day());
			dt.SetHourMinSec(time.hours(),time.minutes(),time.seconds());
			return dt;				 
		}
		catch (...)
		{
			BOOST_THROW_EXCEPTION(DateTimeException(TextUtils::Format(L"Date '%s' do not match format specified: %s", dateStr, format)));
		}		
	}
	else
	{
		BOOST_THROW_EXCEPTION(DateTimeException(L"Unknown format specified"));		
	}
}


