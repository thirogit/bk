#include "stdafx.h"
#include "HerdNo.h"
#include <stdlib.h>
#include <algorithm>
#include <boost/format.hpp>

HerdNo HerdNoFactory::Create(uint32_t herdIndex)
{
	if(herdIndex >= 1 && herdIndex <= 999)
		return HerdNo(herdIndex);

	BOOST_THROW_EXCEPTION(HerdNoException(L"index must be >= 1 and <= 999"));
}


HerdNo::HerdNo()
{
}

HerdNo::HerdNo(uint32_t herdIndex)
{
	m_HerdIndex = herdIndex;
}

HerdNo::HerdNo(const HerdNo& src)
{
	CopyFrom(src);
}

std::wstring HerdNo::ToString() const
{
	return (boost::wformat(L"%03u") % m_HerdIndex.get()).str();	
}

void HerdNo::CopyFrom(const HerdNo& src)
{
	m_HerdIndex = src.m_HerdIndex;

}

HerdNo& HerdNo::operator=(const HerdNo& right)
{
	CopyFrom(right);
	return *this;
}

uint32_t HerdNo::GetIndex() const
{
	if(IsNull())
		return 0;

	return m_HerdIndex.get();
}
	
void HerdNo::MakeNull()
{
	m_HerdIndex.reset();
}

bool HerdNo::IsNull() const
{
	return !m_HerdIndex.is_initialized();
}

bool operator>(const HerdNo& left, const HerdNo& right)
{	
	if(!left.IsNull()) 
	{
		if(right.IsNull())
		{
				return true; 
		}
		else 
		{
			return left.m_HerdIndex.get() > right.m_HerdIndex.get();
		}
	}
	return false; 
}

bool operator<(const HerdNo& left, const HerdNo& right)
{
	return right > left;
}

bool operator==(const HerdNo& left, const HerdNo& right)
{
	bool l = left.IsNull();
	bool r = right.IsNull();
	
	if(l && r) return true;
	else if(r || l) return false;

	return (left.m_HerdIndex.get() == right.m_HerdIndex.get());
}
