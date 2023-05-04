#include "stdafx.h"
#include "DeductionDefinitions.h"
#include "../../arrays/DeepDelete.h"
#include "../../CommonTypes.h"
#include <boost/foreach.hpp>

DeductionDefinitions::DeductionDefinitions()
{
}

DeductionDefinitions::DeductionDefinitions(DeductionDefinitions&& src)
{
	std::copy(src.m_definitions.begin(), src.m_definitions.end(), std::back_inserter(m_definitions));
	src.m_definitions.clear();
}

DeductionDefinitions::DeductionDefinitions(const DeductionDefinitions& src)
{
	BOOST_FOREACH(DeductionDefinition* deduction, src)
	{
		Add(deduction);
	}
}

DeductionDefinitions::~DeductionDefinitions()
{
	DeepDelete(m_definitions);
	m_definitions.clear();
}

void DeductionDefinitions::Add(const IDeductionDefinition* deduction)
{
	m_definitions.push_back(new DeductionDefinition(deduction));
}

PtrEnumerator<IDeductionDefinition> DeductionDefinitions::Enum()
{
	return PtrEnumerator<IDeductionDefinition>(m_definitions);
}

DeductionDefinitions::const_iterator DeductionDefinitions::begin() const
{
	return m_definitions.begin();
}

DeductionDefinitions::const_iterator DeductionDefinitions::end() const
{
	return m_definitions.end();
}

DeductionDefinitions::iterator DeductionDefinitions::begin()
{
	return m_definitions.begin();
}

DeductionDefinitions::iterator DeductionDefinitions::end()
{
	return m_definitions.end();
}

size_t DeductionDefinitions::size() const
{
	return m_definitions.size();
}