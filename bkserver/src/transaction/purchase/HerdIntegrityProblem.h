#pragma once

#include "IntegrityProblem.h"

class HerdIntegrityProblem : public IntegrityProblem
{
public:
	HerdIntegrityProblem(ItemId id,const PurchaseObjPath& path,int herdIndex);
	virtual PurchaseDownloadIssue ToIssue() const;
	virtual void Accept(ProblemVisitor* visitor);
	virtual Solution* Solve(SolutionParameters& parameters);
	virtual bool IsSameAs(IntegrityProblem* problem);

	int GetHerdIndex() const;
	virtual IntegrityProblem* Clone() const;
	virtual PurchaseDownloadIssueCategory GetCategory() const;
	virtual std::wstring DescribeProblem() const;
private:
	int m_herdIndex;
};

