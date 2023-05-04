#pragma once 

#include <boost/shared_ptr.hpp>
#include <SF/Archive.hpp>
#include <arrays/NewPtrFlushArray.h>
#include <arrays/Enumerator.h>
#include "../../configuration/deductions/DeductionDefinition.h"
#include "../../CommonTypes.h"

class FetchDeductionDefinitionsResponse
{
private:
	NewPtrFlushArray<DeductionDefinition> m_Definitions;
public:
	FetchDeductionDefinitionsResponse();
	~FetchDeductionDefinitionsResponse();
	
	DeductionDefinition* AddDefinition(uint32_t definitionId);
		
	PtrEnumerator<IDeductionDefinition> GetDefinitions();
	count_t GetDefinitionsCount() const;
	
	void serialize(SF::Archive &archive);
};

typedef boost::shared_ptr<FetchDeductionDefinitionsResponse> FetchDeductionDefinitionsResponseObj;
