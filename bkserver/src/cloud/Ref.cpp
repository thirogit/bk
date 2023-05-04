#include "stdafx.h"
#include "Ref.h"
#include <boost/algorithm/string.hpp>

namespace gcp
{

	
	Ref::Ref()
	{
	}

	Ref::Ref(const std::wstring& s)
	{		
		boost::split(m_segments, s, boost::is_any_of(L"/"), boost::token_compress_on);
	}

	Ref::Ref(const Ref& src)
	{		
		std::copy(src.m_segments.begin(), src.m_segments.end(), std::back_inserter(m_segments));
	}

	Ref::Ref(Ref&& src)
	{
		m_segments.swap(src.m_segments);
	}

	Ref& Ref::operator/=(const std::wstring& segment)
	{
		return operator/=(Ref(segment));
	}

	Ref& Ref::operator/=(const Ref& path)
	{
		std::copy(path.m_segments.begin(), path.m_segments.end(), std::back_inserter(m_segments));
		return *this;
	}

	std::wstring Ref::ToString() const
	{
		std::wstring result;
		auto it = m_segments.cbegin();
		while(it != m_segments.cend())
		{
			result += *it;
			it++;
			if (it != m_segments.cend())
				result += L"/";
		}
		return result;
	}

	std::wstring Ref::LastSegment()
	{
		if (!m_segments.empty())
		{
			return m_segments.back();
		}
		return L"";
	}

	Ref operator/(const Ref& ref, const std::wstring& right)
	{
		Ref copy(ref);
		copy /= right;
		return std::move(copy);
	}
	
	
}