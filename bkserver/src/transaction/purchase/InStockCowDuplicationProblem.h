#pragma once

#include "IntegrityProblem.h"
#include <data/types/CowNo.h>

class InStockCowDuplicationProblem : public IntegrityProblem
{
public:
	InStockCowDuplicationProblem(ItemId id,const PurchaseObjPath& path,const CowNo& cowNo,int herdIndex);
	virtual PurchaseDownloadIssue ToIssue() const;
	virtual void Accept(ProblemVisitor* visitor);
	virtual Solution* Solve(SolutionParameters& parameters);
	virtual bool IsSameAs(IntegrityProblem* problem);
	const CowNo& GetCowNo() const;
	int GetHerdIndex() const;
	virtual IntegrityProblem* Clone() const;
	virtual PurchaseDownloadIssueCategory GetCategory() const;
	virtual std::wstring DescribeProblem() const;
private:
	CowNo m_cowNo;
	int m_herdIndex;
};
