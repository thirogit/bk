#include "stdafx.h"
#include "SerializableAgent.h"
#include "../../types/serialization/TypesSerialization.h"

SerializableAgent::SerializableAgent(Agent* pAgent) : m_pAgent(pAgent)
{
}

	
uint32_t& SerializableAgent::Id()
{
	return m_pAgent->m_agentId;
}

std::wstring& 	SerializableAgent::Name()
{
	return m_pAgent->m_agentname;
}

std::wstring& 	SerializableAgent::Code()
{
	return m_pAgent->m_agentcode;
}

std::wstring& 	SerializableAgent::Plate()
{
	return m_pAgent->m_plateNo;
}

void SerializableAgent::serialize(SF::Archive &archive)
{
	archive & Id();
	archive & Name();	
	archive & Code();	
	archive & Plate();
}