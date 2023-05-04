#pragma once

#include "Solution.h"
#include <data/runtime/SolutionType.h>
#include <string>
#include <data/repo/PurchaseObjPath.h>

class AgentIntegritySolution : public Solution
{
public:
	AgentIntegritySolution(SolutionType type, const PurchaseObjPath& path, const std::wstring& problemAgentCd, const std::wstring& fixAgentCd);
	virtual bool IsSolving(IntegrityProblem* problem);
	virtual void Accept(SolutionVisitor* visitor);
	virtual Solution* Generalize() const;
	virtual bool IsImmediate() const;
		
	SolutionType GetType() const;
	const std::wstring& GetProblemAgentCd() const;
	const std::wstring& GetFixAgentCd();
	
private:
	SolutionType m_type;
	PurchaseObjPath m_path;
	std::wstring m_problemAgentCd;
	std::wstring m_fixAgentCd;
};

