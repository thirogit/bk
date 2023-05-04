#pragma once

#include "IntegrityProblem.h"
#include <data/repo/PurchaseHent.h>

class FirstOwnerHentIntegrityProblem : public IntegrityProblem
{
public:
	FirstOwnerHentIntegrityProblem(ItemId id,const PurchaseObjPath& path,const IPurchaseHent* hent);
	virtual PurchaseDownloadIssue ToIssue() const;
	virtual void Accept(ProblemVisitor* visitor);
	virtual Solution* Solve(SolutionParameters& parameters);
	virtual bool IsSameAs(IntegrityProblem* problem);
	const IPurchaseHent* GetHent() const;
	~FirstOwnerHentIntegrityProblem();
	virtual IntegrityProblem* Clone() const;
	virtual PurchaseDownloadIssueCategory GetCategory() const;
	virtual std::wstring DescribeProblem() const;
private:
	PurchaseHent* m_hent;
};

