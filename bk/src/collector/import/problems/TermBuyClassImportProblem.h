#pragma once

#include "ImportProblem.h"


class TermBuyClassImportProblem : public ImportProblem
{
public:

	TermBuyClassImportProblem(ItemId id, const std::wstring& classCd);
	//virtual PurchaseDownloadIssue ToIssue() const;
	//virtual void Accept(ProblemVisitor* visitor);
	//virtual Solution* Solve(SolutionParameters& parameters);
	const std::wstring& GetClassCd() const;
	virtual ImportProblem* Clone() const;
	virtual std::wstring DescribeProblem() const;
private:
	std::wstring m_classCd;
};

