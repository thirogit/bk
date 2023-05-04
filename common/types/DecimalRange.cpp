#include "stdafx.h"
#include "DecimalRange.h"


DecimalRange::DecimalRange()
{
}

DecimalRange::DecimalRange(const NullDecimal& low, const NullDecimal& hi) : m_low(low), m_hi(hi)
{
}

DecimalRange::DecimalRange(const DecimalRange& src)
{
	CopyFrom(src);
}

DecimalRange& DecimalRange::operator=(const DecimalRange& right)
{
	CopyFrom(right);
	return *this;
}

void DecimalRange::CopyFrom(const DecimalRange& src)
{
	m_low = src.m_low;
	m_hi = src.m_hi;
}

const NullDecimal& DecimalRange::GetLow() const
{
	return m_low;
}

const NullDecimal& DecimalRange::GetHi() const
{
	return m_hi;
}

bool DecimalRange::IsWithin(const Decimal& d) const
{
	NullDecimal nd(d);
	if(m_low.IsNull() && m_hi.IsNull()) return true;

	if (m_low.IsNull())
	{
		return m_hi >= nd;
	}
	else if (m_hi.IsNull())
	{
		return m_low <= nd;
	}

	return m_low <= nd && m_hi >= nd;
}

