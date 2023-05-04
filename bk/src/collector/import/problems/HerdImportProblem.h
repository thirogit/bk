#pragma once

#include "ImportProblem.h"


class HerdImportProblem : public ImportProblem
{
public:

	HerdImportProblem(ItemId id, int herdIndex);
	//virtual PurchaseDownloadIssue ToIssue() const;
	//virtual void Accept(ProblemVisitor* visitor);
	//virtual Solution* Solve(SolutionParameters& parameters);
	int GetHerdIndex() const;
	//virtual IntegrityProblem* Clone() const;
	virtual std::wstring DescribeProblem() const;
private:
	int m_herdIndex;
};
	
