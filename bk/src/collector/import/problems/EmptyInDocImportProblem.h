#pragma once

#include "ImportProblem.h"


class EmptyInDocImportProblem : public ImportProblem
{
public:

	EmptyInDocImportProblem(ItemId id);
	//virtual PurchaseDownloadIssue ToIssue() const;
	//virtual void Accept(ProblemVisitor* visitor);
	//virtual Solution* Solve(SolutionParameters& parameters);
	
	virtual ImportProblem* Clone() const;
	virtual std::wstring DescribeProblem() const;
private:
	
};
	
