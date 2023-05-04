#include "stdafx.h"
#include "AgentIntegritySolution.h"
#include "SolutionVisitor.h"
#include "AgentProblemDetectingVisitor.h"
#include "IntegrityProblem.h"

AgentIntegritySolution::AgentIntegritySolution(SolutionType type, const PurchaseObjPath& path, const std::wstring& problemAgentCd, const std::wstring& fixAgentCd)
{
	m_type = type;
	m_problemAgentCd = problemAgentCd;
	m_fixAgentCd = fixAgentCd;
	m_path = path;
}

SolutionType AgentIntegritySolution::GetType() const
{
	return m_type;
}

const std::wstring& AgentIntegritySolution::GetProblemAgentCd() const
{
	return m_problemAgentCd;
}

const std::wstring& AgentIntegritySolution::GetFixAgentCd()
{
	return m_fixAgentCd;
}

bool AgentIntegritySolution::IsSolving(IntegrityProblem* problem)
{
	AgentProblemDetectingVisitor visitor(m_problemAgentCd);
	problem->Accept(&visitor);
	return visitor.WasDetected();
}

void AgentIntegritySolution::Accept(SolutionVisitor* visitor)
{
	visitor->visit(this);
}

Solution* AgentIntegritySolution::Generalize() const
{
	return new AgentIntegritySolution(m_type, PurchaseObjPath(), m_problemAgentCd, m_fixAgentCd);
}

bool AgentIntegritySolution::IsImmediate() const
{
	return true;
}