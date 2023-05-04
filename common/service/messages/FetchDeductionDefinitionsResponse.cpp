#include "stdafx.h"
#include "FetchDeductionDefinitionsResponse.h"
#include "../../configuration/serialization/SerializableDeductionDefinition.h"

//-------------------------------------------------------------------------------------------------

FetchDeductionDefinitionsResponse::FetchDeductionDefinitionsResponse() 
{
}

FetchDeductionDefinitionsResponse::~FetchDeductionDefinitionsResponse()
{
	m_Definitions.FreeAndFlush();
}	

DeductionDefinition* FetchDeductionDefinitionsResponse::AddDefinition(uint32_t definitionId)
{
	DeductionDefinition* pNew = new DeductionDefinition(definitionId);
	m_Definitions.Add(pNew);
	return pNew;
}
		
PtrEnumerator<IDeductionDefinition> FetchDeductionDefinitionsResponse::GetDefinitions()
{
	return PtrEnumerator<IDeductionDefinition>(m_Definitions);
}

count_t FetchDeductionDefinitionsResponse::GetDefinitionsCount() const
{
	return m_Definitions.size();
}

void FetchDeductionDefinitionsResponse::serialize(SF::Archive &archive)
{
	count_t count;
	DeductionDefinition* deduction;

	if(archive.isRead())
	{
		archive & count;
		for (count_t i = 0; i < count; i++)
		{
			deduction = AddDefinition(NULL_ID);
			SerializableDeductionDefinition(deduction).serialize(archive);
		}
	}	
	else
	{
		count = m_Definitions.size();
		archive & count;
		for (count_t i = 0; i < count; i++)
		{
			deduction = m_Definitions[i];
			SerializableDeductionDefinition(deduction).serialize(archive);
		}
	}
}
