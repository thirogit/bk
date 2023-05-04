#include "stdafx.h"
#include "DeductionDefinitionRequest.h"
#include "../../configuration/serialization/SerializableDeductionDefinition.h"

DeductionDefinitionRequest::DeductionDefinitionRequest() : m_pDefinition(NULL)
{
}

DeductionDefinitionRequest::~DeductionDefinitionRequest()
{
	if (m_pDefinition)
		delete m_pDefinition;
}

IDeductionDefinition* DeductionDefinitionRequest::GetDefinition() const
{
	return m_pDefinition;
}

DeductionDefinition* DeductionDefinitionRequest::CreateDefinition(uint32_t definitionId)
{
	if (!m_pDefinition)
		m_pDefinition = new DeductionDefinition(definitionId);
	return m_pDefinition;
}

void DeductionDefinitionRequest::serialize(SF::Archive &archive)
{
	if(archive.isRead())
	{
		CreateDefinition(NULL_ID);
		SerializableDeductionDefinition(m_pDefinition).serialize(archive);
	}
	else
	{
		SerializableDeductionDefinition(m_pDefinition).serialize(archive);
	}
}
	
