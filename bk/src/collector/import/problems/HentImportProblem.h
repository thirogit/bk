#pragma once

#include "ImportProblem.h"
#include "../../data/DownloadHent.h"

class HentImportProblem : public ImportProblem
{
public:
	HentImportProblem(ItemId id, const DownloadHent& hent, uint16_t solutions);
	//virtual PurchaseDownloadIssue ToIssue() const;
	//virtual void Accept(ProblemVisitor* visitor);
	//virtual Solution* Solve(SolutionParameters& parameters);
	DownloadHent GetHent() const;
	virtual ImportProblem* Clone() const;
	virtual std::wstring DescribeProblem() const;
private:
	DownloadHent m_problemHent;
	uint16_t m_solutions;
};


