#include "stdafx.h"
#include "AgentProblemDetectingVisitor.h"
#include "AgentIntegrityProblem.h"

AgentProblemDetectingVisitor::AgentProblemDetectingVisitor(const std::wstring& agentCd) : m_agentCd(agentCd),m_detected(false)
{
}

void AgentProblemDetectingVisitor::visit(AgentIntegrityProblem* problem)
{
	m_detected = problem->GetAgentCd() == m_agentCd;
}

bool AgentProblemDetectingVisitor::WasDetected() const
{
	return m_detected;
}
