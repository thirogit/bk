#include "stdafx.h"
#include "Values.h"

namespace bkprint
{
	Values::Values()
	{
	}

	Values::Values(const Values& src)
	{
		operator=(src);
	}

	Values::Values(Values&& src)
	{
		m_values = std::move(src.m_values);
	}

	Values& Values::operator=(const Values& right)
	{
		m_values = right.m_values;
		return *this;
	}

	std::wstring Values::Get(const std::wstring& name) const
	{
		auto it = m_values.find(name);
		if (it != m_values.end())
			return it->second;

		return L"";
	}

	void Values::Set(const std::wstring& name, const std::wstring& value)
	{
		m_values[name] = value;
	}

	Values& Values::operator+=(const Values& values)
	{
		auto it = values.m_values.begin();
		while (it != values.m_values.end())
		{
			m_values[it->first] = it->second;
			it++;
		}
		return *this;
	}

}