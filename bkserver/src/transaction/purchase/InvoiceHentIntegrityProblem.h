#pragma once

#include "IntegrityProblem.h"
#include <data/repo/PurchaseHent.h>

class InvoiceHentIntegrityProblem : public IntegrityProblem
{
public:
	InvoiceHentIntegrityProblem(ItemId id,const PurchaseObjPath& path,const IPurchaseHent* hent);
	~InvoiceHentIntegrityProblem();

	virtual PurchaseDownloadIssue ToIssue() const;
	virtual void Accept(ProblemVisitor* visitor);
	virtual Solution* Solve(SolutionParameters& parameters);
	virtual bool IsSameAs(IntegrityProblem* problem);
	const IPurchaseHent* GetHent() const;
	virtual IntegrityProblem* Clone() const;
	virtual PurchaseDownloadIssueCategory GetCategory() const;
	virtual std::wstring DescribeProblem() const;
	
private:
	PurchaseHent* m_hent;
};
