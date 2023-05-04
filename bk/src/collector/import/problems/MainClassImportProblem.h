#pragma once

#include "ImportProblem.h"
#include "ProblemVisitor.h"

class MainClassImportProblem : public ImportProblem
{
public:

	MainClassImportProblem(ItemId id, const std::wstring& classCd);
	//virtual PurchaseDownloadIssue ToIssue() const;
	virtual void Accept(ProblemVisitor* visitor);
	//virtual Solution* Solve(SolutionParameters& parameters);
	const std::wstring& GetClassCd() const;
	virtual ImportProblem* Clone() const;
	virtual std::wstring DescribeProblem() const;
private:
	std::wstring m_classCd;
};
	
