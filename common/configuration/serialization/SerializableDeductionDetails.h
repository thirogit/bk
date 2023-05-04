#pragma once 

#include <SF/Archive.hpp>
#include <SF/string.hpp> 
#include "../deductions/DeductionDetails.h"

class SerializableDeductionDetails
{
public:
	SerializableDeductionDetails(DeductionDetails& details);
	void serialize(SF::Archive &archive);
private:
	DeductionDetails& m_details;
};
