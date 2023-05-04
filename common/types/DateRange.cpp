#include "stdafx.h"
#include "DateRange.h"


DateRange::DateRange()
{
}

DateRange::DateRange(const DateTime& strDt,const DateTime& endDt) : m_strDt(strDt),m_endDt(endDt)
{
}

DateRange::DateRange(const DateRange& src)
{
	CopyFrom(src);
}

DateRange& DateRange::operator=(const DateRange& right)
{
	CopyFrom(right);
	return *this;
}

void DateRange::CopyFrom(const DateRange& src)
{
	m_strDt = src.m_strDt;
	m_endDt = src.m_endDt;
}

const DateTime& DateRange::GetStrDt() const
{
	return m_strDt;
}

const DateTime& DateRange::GetEndDt() const
{
	return m_endDt;
}

bool DateRange::IsDateWithin(const DateTime& dt) const
{
	if(m_strDt.IsNull() && m_endDt.IsNull()) return true;

	if (dt.IsNull()) return false;

	if (m_strDt.IsNull())
	{
		return m_endDt >= dt;
	}
	else if (m_endDt.IsNull())
	{
		return m_strDt <= dt;
	}

	return m_strDt <= dt && m_endDt >= dt;
}

