#include "stdafx.h"
#include "SerializableDeductionDefinition.h"
#include "../../configuration/serialization/SerializableDeductionDetails.h"

SerializableDeductionDefinition::SerializableDeductionDefinition(DeductionDefinition* pDefinition) : m_pDefinition(pDefinition)
{
}

void SerializableDeductionDefinition::serialize(SF::Archive &archive)
{
	archive & Id();
	
	SerializableDeductionDetails(Details()).serialize(archive);
	
	archive & EnabledByDefault();
}

uint32_t& SerializableDeductionDefinition::Id()
{
	return m_pDefinition->m_Id;
}

DeductionDetails& SerializableDeductionDefinition::Details()
{
	return m_pDefinition->m_details;
}

bool& SerializableDeductionDefinition::EnabledByDefault()
{
	return m_pDefinition->m_enabledByDefault;
}



