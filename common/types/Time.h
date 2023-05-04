#ifndef __TIME_H__
#define __TIME_H__

#include "NullInt.h"
#include "DateTime.h"

class Time : public Nullable
{
public:

	enum TimeFormat
	{
		HHcolonMIcolonSS,
		HHcolonMI,
	};

	Time();
	Time(unsigned short hour,unsigned short minutes,unsigned short seconds);
	Time(const DateTime& datetime);
	Time(const Time& copy);
	explicit Time(const NullInt& src);
	explicit Time(int secsFromMidnight);

	bool Set(unsigned short hour,unsigned short minutes,unsigned short seconds); 
	bool SetSecsFromMidnight(int secsFromMidnight); 
	int GetSecsFromMidnight() const; 
	
	Time& operator=(const Time& right);
	
	std::wstring GetTime(Time::TimeFormat format) const;
	
	static Time ParseTime(TimeFormat fmt,const std::wstring& inputTm);
	static const Time now();

	unsigned short GetHour() const;
	unsigned short GetMinutes() const;
	unsigned short GetSeconds() const;

	virtual void MakeNull();
	virtual bool IsNull() const;

protected:

	NullInt m_time;

};

bool operator>=(const Time& left, const Time& right);
bool operator<=(const Time& left, const Time& right);
bool operator>(const Time& left, const Time& right);
bool operator<(const Time& left, const Time& right);
bool operator==(const Time& left, const Time& right);
bool operator!=(const Time& left, const Time& right);


typedef struct _tagTIMEFMTS
{
	Time::TimeFormat tmFmt;
	const wchar_t* strFmt;
} TIMEFMTS;

class TimeParser
{
private:
	TimeParser();
	std::map<Time::TimeFormat,const wchar_t*> _timefmtsMap;
	static TimeParser instance;
public:
	static Time ParseTime(Time::TimeFormat timeFmt, const std::wstring& sTime);
};

class TimeFormatter
{
private:
	TimeFormatter();
	std::map<Time::TimeFormat,const wchar_t*> _timefmtsMap;
	static TimeFormatter instance;
public:
	static void FormatTime(const Time& t,Time::TimeFormat tmFmt,wchar_t* szBuff,int maxsize);

};


#endif
