#pragma once

#include "IntegrityProblem.h"

class ClassIntegrityProblem : public IntegrityProblem
{
public:
	ClassIntegrityProblem(ItemId id,const PurchaseObjPath& path,const std::wstring& classCd);
	virtual PurchaseDownloadIssue ToIssue() const;
	virtual void Accept(ProblemVisitor* visitor);
	virtual Solution* Solve(SolutionParameters& parameters);
	virtual bool IsSameAs(IntegrityProblem* problem);
	const std::wstring& GetClassCd() const;
	virtual IntegrityProblem* Clone() const;
	virtual PurchaseDownloadIssueCategory GetCategory() const;
	virtual std::wstring DescribeProblem() const;
private:
	std::wstring m_classCd;
};
