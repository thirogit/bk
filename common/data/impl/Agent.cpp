#include "stdafx.h"
#include "Agent.h"

Agent::Agent(uint32_t Id) : m_agentId(Id)
{
}

Agent::Agent(const IAgent* src) : m_agentId(src->GetId())
{
	CopyFrom(src);
}

void Agent::CopyFrom(const IAgent* src)
{
	m_agentcode = src->GetAgentCode();
	m_agentname = src->GetAgentName();
	m_plateNo = src->GetPlateNo();
}

uint32_t Agent::GetId() const
{
	return m_agentId;
}

const std::wstring& Agent::GetAgentCode() const
{
	return m_agentcode;
}

const std::wstring& Agent::GetAgentName() const
{
	return m_agentname;
}

const std::wstring& Agent::GetPlateNo() const
{
	return m_plateNo;
}

void Agent::SetAgentCode(const std::wstring& sAgentCode)
{
	m_agentcode = sAgentCode;
}

void Agent::SetAgentName(const std::wstring& sAgentName)
{
	m_agentname = sAgentName;
}

void Agent::SetPlateNo(const std::wstring& sPlateNo)
{
	m_plateNo = sPlateNo;
}

