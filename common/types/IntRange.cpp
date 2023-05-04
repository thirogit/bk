#include "stdafx.h"
#include "IntRange.h"


IntRange::IntRange()
{
}

IntRange::IntRange(const NullInt& low, const NullInt& hi) : m_low(low), m_hi(hi)
{
}

IntRange::IntRange(const IntRange& src)
{
	CopyFrom(src);
}

IntRange& IntRange::operator=(const IntRange& right)
{
	CopyFrom(right);
	return *this;
}

void IntRange::CopyFrom(const IntRange& src)
{
	m_low = src.m_low;
	m_hi = src.m_hi;
}

const NullInt& IntRange::GetLow() const
{
	return m_low;
}

const NullInt& IntRange::GetHi() const
{
	return m_hi;
}

bool IntRange::IsWithin(int i) const
{
	NullInt ni(i);
	if(m_low.IsNull() && m_hi.IsNull()) return true;

	if (m_low.IsNull())
	{
		return m_hi >= ni;
	}
	else if (m_hi.IsNull())
	{
		return m_low <= ni;
	}

	return m_low <= ni && m_hi >= ni;
}

