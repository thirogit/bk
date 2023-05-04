#pragma once
#include "EmptyProblemVisitor.h"
#include <string>

class AgentProblemDetectingVisitor : public EmptyProblemVisitor
{
public:
	AgentProblemDetectingVisitor(const std::wstring& agentCd);
	virtual void visit(AgentIntegrityProblem* problem);
	bool WasDetected() const;
private:
	std::wstring m_agentCd;
	bool m_detected;
};
