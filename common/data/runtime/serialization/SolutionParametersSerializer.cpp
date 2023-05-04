#include "stdafx.h"
#include "SolutionParametersSerializer.h"
#include <SF/any.hpp>

SolutionParametersSerializer::SolutionParametersSerializer(SolutionParameters* pParameters) : m_pParameters(pParameters)
{
}

void SolutionParametersSerializer::serialize(SF::Archive &archive)
{
	archive & m_pParameters->m_type & m_pParameters->m_parameter;
}