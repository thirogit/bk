#include "stdafx.h"
#include "AgentIntegrityProblem.h"
#include <errortoken/ErrorToken.h>
#include "AgentIntegritySolution.h"
#include "AgentProblemDetectingVisitor.h"

AgentIntegrityProblem::AgentIntegrityProblem(ItemId id, const PurchaseObjPath& path, const std::wstring& agentCd) : 
	IntegrityProblem(id, path), m_agentCd(agentCd)
{
}

PurchaseDownloadIssue AgentIntegrityProblem::ToIssue() const
{
	PurchaseDownloadIssue issue(GetId());
	issue.SetCategory(GetCategory());
	issue.SetPath(GetPath());
	issue.SetError(ErrorToken(L"repo_issue_no_agent")(L"agentCd", m_agentCd).str());
	issue.SetSolutions(SolutionType_Clear | SolutionType_Replace);
	return issue;
};


std::wstring AgentIntegrityProblem::DescribeProblem() const
{
	return L"no agent with code " + m_agentCd;
}

PurchaseDownloadIssueCategory AgentIntegrityProblem::GetCategory() const
{
	return PurchaseDownloadIssueCategory(PurchaseDownloadIssueDomain_Agent, m_agentCd);
}

void AgentIntegrityProblem::Accept(ProblemVisitor* visitor)
{
	visitor->visit(this);
}

Solution* AgentIntegrityProblem::Solve(SolutionParameters& parameters)
{
	return new AgentIntegritySolution(parameters.type(), GetPath(), m_agentCd, parameters.parameter_default<std::wstring>(L""));
}

bool AgentIntegrityProblem::IsSameAs(IntegrityProblem* problem)
{
	AgentProblemDetectingVisitor visitor(m_agentCd);
	problem->Accept(&visitor);
	return visitor.WasDetected();
}

const std::wstring& AgentIntegrityProblem::GetAgentCd() const
{
	return m_agentCd;
}

IntegrityProblem* AgentIntegrityProblem::Clone() const
{
	return new AgentIntegrityProblem(GetId(), GetPath(), GetAgentCd());
}
