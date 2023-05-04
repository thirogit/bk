#pragma once

#include "IntegrityProblem.h"


class AgentIntegrityProblem : public IntegrityProblem
{
public:
	AgentIntegrityProblem(ItemId id,const PurchaseObjPath& path,const std::wstring& agentCd);
	virtual PurchaseDownloadIssue ToIssue() const;
	virtual void Accept(ProblemVisitor* visitor);
	virtual Solution* Solve(SolutionParameters& parameters);
	virtual bool IsSameAs(IntegrityProblem* problem);
	const std::wstring& GetAgentCd() const;
	virtual IntegrityProblem* Clone() const;
	virtual PurchaseDownloadIssueCategory GetCategory() const;
	virtual std::wstring DescribeProblem() const;
private:
	std::wstring m_agentCd;
};
