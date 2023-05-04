#pragma once 

#include <SF/Archive.hpp>
#include <SF/string.hpp> 
#include "../deductions/DeductionDefinition.h"

class SerializableDeductionDefinition
{
public:
	SerializableDeductionDefinition(DeductionDefinition* pDefinition);
	void serialize(SF::Archive &archive);
private:
	uint32_t&		Id();	
	DeductionDetails& Details();
	bool& EnabledByDefault();
private:
	DeductionDefinition* m_pDefinition;
};
