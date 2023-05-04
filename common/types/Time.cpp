#include "stdafx.h"
#include "Time.h"
#include "../format/StringFormat.h"
#include "DateTime.h"
#include <string.h>
#include <time.h>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "../string/TextUtils.h"

namespace pt = boost::posix_time;

static TIMEFMTS _timefmts[] =
{
	{Time::HHcolonMIcolonSS, L"%H:%M:%S"},
	{Time::HHcolonMI,L"%H:%M"},
	{(Time::TimeFormat)0,NULL}
};

Time::Time() 
{
	
}

Time::Time(unsigned short hour,unsigned short minutes,unsigned short seconds) 
{
	if(!Set(hour,minutes,seconds))
		MakeNull();
}

Time::Time(const DateTime& datetime) 
{
	Set(datetime.GetHour(),datetime.GetMinutes(),datetime.GetSeconds());
}

Time::Time(const Time& copy)
{
	operator=(copy);
}

Time::Time(int secsFromMidnight) 
{
	if(secsFromMidnight > SECONDS_IN_DAY || secsFromMidnight < 0)
		MakeNull();
	else
	{
		m_time = secsFromMidnight%SECONDS_IN_DAY;
	}
}

Time::Time(const NullInt& secsFromMidnight)
{
	if(secsFromMidnight.IsNull())
		MakeNull();
	else
		SetSecsFromMidnight(secsFromMidnight.ToInt());	
}

Time Time::ParseTime(TimeFormat fmt,const std::wstring& inputTm)
{
	return TimeParser::ParseTime(fmt,inputTm);	
}

Time& Time::operator=(const Time& right)
{
	m_time = right.m_time;
	return *this;
}

const Time Time::now()
{
	DateTime nowDt = DateTime::now();
	return Time(nowDt.GetHour(),nowDt.GetMinutes(),nowDt.GetSeconds());
}

std::wstring Time::GetTime(Time::TimeFormat format) const
{
	wchar_t tmBuff[100];
	TimeFormatter::FormatTime(*this,format,tmBuff,sizeof(tmBuff));
	return tmBuff;
}


unsigned short Time::GetHour() const
{
	if(!IsNull())
		return m_time.ToInt()/3600;

	return (unsigned short)(-1);
}
unsigned short Time::GetMinutes() const
{
	if(!IsNull())
		return (m_time.ToInt()%3600)/60;
	return (unsigned short)(-1);
}
unsigned short Time::GetSeconds() const
{
	if(!IsNull())
		return (m_time.ToInt()%3600)%60;
	return (unsigned short)(-1);
}

bool Time::Set(unsigned short hour,unsigned short minutes,unsigned short seconds)
{
	if( hour > 23 || minutes > 60 || seconds > 60)
		return false;
	m_time = hour*60*60 + minutes*60 + seconds;
	return true;
}


bool Time::SetSecsFromMidnight(int secsFromMidnight)
{
	if(secsFromMidnight < 0)
		return false;

	m_time = secsFromMidnight%SECONDS_IN_DAY;
	return true;
}

int Time::GetSecsFromMidnight() const
{
	return m_time.ToInt(-1);
}

void Time::MakeNull()
{
	m_time.MakeNull();
}

bool Time::IsNull() const
{
	return m_time.IsNull();
}

bool operator>=(const Time& left, const Time& right)
{
	if(left.IsNull() && right.IsNull()) return true;
	if(left.IsNull() || right.IsNull()) return false;

	return	left.GetSecsFromMidnight()   >= right.GetSecsFromMidnight();
	
}

bool operator<=(const Time& left, const Time& right)
{
	if(left.IsNull() && right.IsNull()) return true;
	if(left.IsNull() || right.IsNull()) return false;

	return left.GetSecsFromMidnight() <= right.GetSecsFromMidnight();
	
}

bool operator>(const Time& left, const Time& right)
{
	if(left.IsNull())
	{
		return false;
	}
	else
		if(right.IsNull()) return true;

	return left.GetSecsFromMidnight() > right.GetSecsFromMidnight();
		
}

bool operator<(const Time& left, const Time& right)
{
	return (right > left);
}


bool operator==(const Time& left, const Time& right)
{
	if(left.IsNull() && right.IsNull()) return true;
	if(right.IsNull() || left.IsNull()) return false;

	return left.GetSecsFromMidnight() == right.GetSecsFromMidnight();
}

bool operator!=(const Time& left, const Time& right)
{
	return !(left == right);
}



///////////////////////////////////////////////////////////////////////////////

TimeParser TimeParser::instance;

TimeParser::TimeParser()
{
	TIMEFMTS *timeFmt = _timefmts;
	while(timeFmt->strFmt)
	{
		_timefmtsMap[timeFmt->tmFmt] = timeFmt->strFmt;
		timeFmt++;
	}
}
	
Time TimeParser::ParseTime(Time::TimeFormat timeFmt,const std::wstring& inputTm)
{
	Time t;
		
	if(!inputTm.empty())
	{

		const wchar_t* strFmt = NULL;
		auto fmtIt = instance._timefmtsMap.find(timeFmt);
		if(fmtIt != instance._timefmtsMap.end())
		{
			strFmt = fmtIt->second;			
			
			std::wstringstream ss(inputTm);
			pt::wtime_input_facet* df = new pt::wtime_input_facet(strFmt);
			ss.imbue(std::locale(ss.getloc(), df));

			pt::ptime tm;  

			try
			{
				ss >> tm;
				auto tod = tm.time_of_day();
				if(!(t.Set(tod.hours(),tod.minutes(),tod.seconds())))
					BOOST_THROW_EXCEPTION(DateTimeException(TextUtils::Format(L"%s is a bad time.", inputTm)));
			}
			catch (...)
			{
				BOOST_THROW_EXCEPTION(DateTimeException(TextUtils::Format(L"Time '%s' do not match format '%s'", inputTm, strFmt)));
			}			
		}
		else
		{
			BOOST_THROW_EXCEPTION(DateTimeException(L"Unknown format specified"));		
		}
	}
	return t;
}

///////////////////////////////////////////////////////////////////////////////

TimeFormatter TimeFormatter::instance;

TimeFormatter::TimeFormatter()
{
	TIMEFMTS *tmFmt = _timefmts;
	while(tmFmt->strFmt)
	{
		_timefmtsMap[tmFmt->tmFmt] = tmFmt->strFmt;
		tmFmt++;
	}

}

void TimeFormatter::FormatTime(const Time& t,Time::TimeFormat tmFmt,wchar_t* szBuff,int maxsize)
{
	const wchar_t* strFmt = NULL;

	if(t.IsNull())
		szBuff[0] = L'\0';
	else
	{
		auto fmtIt = instance._timefmtsMap.find(tmFmt);
		if(fmtIt != instance._timefmtsMap.end())
		{
			strFmt = fmtIt->second;
			struct tm tm1;

			memset(&tm1,0,sizeof(struct tm));

			tm1.tm_year = 0;
			tm1.tm_mon = 0;
			tm1.tm_mday = 0;
			tm1.tm_hour = t.GetHour();
			tm1.tm_min = t.GetMinutes();
			tm1.tm_sec = t.GetSeconds();
		
			wcsftime(szBuff, maxsize, strFmt, &tm1);
		}
		else
			BOOST_THROW_EXCEPTION(DateTimeException(L"FormatTime(): Unknown format specified"));	
	}
}
