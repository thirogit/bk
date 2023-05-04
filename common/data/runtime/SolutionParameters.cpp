#include "stdafx.h"
#include "SolutionParameters.h"
	
SolutionParameters::SolutionParameters() : m_type(SolutionType_None)
{
}

SolutionParameters::SolutionParameters(const SolutionParameters& src)
{
	operator=(src);
}

SolutionParameters::SolutionParameters(SolutionParameters&& src)
{
	m_type = src.m_type;
	m_parameter = std::move(src.m_parameter);
	src.m_type = SolutionType_None;	
	m_parameter.empty();
}

SolutionParameters& SolutionParameters::operator=(const SolutionParameters& right)
{
	m_type = right.m_type;
	m_parameter = right.m_parameter;
	return *this;
}

void SolutionParameters::type(SolutionType typee)
{
	m_type = typee;
}

SolutionType SolutionParameters::type() const
{
	return m_type;
}


SolutionParameters SolutionParameters::make(SolutionType typee)
{
	SolutionParameters result;
	result.type(typee);	
	return result;
}

