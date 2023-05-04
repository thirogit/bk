#pragma once

#include <SF/Archive.hpp>
#include "../SolutionParameters.h"

class SolutionParametersSerializer
{
public:
	SolutionParametersSerializer(SolutionParameters* pParameters);
	void serialize(SF::Archive &archive);	
private:
	SolutionParameters* m_pParameters;
};
