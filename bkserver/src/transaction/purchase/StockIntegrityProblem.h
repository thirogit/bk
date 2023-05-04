#pragma once

#include "IntegrityProblem.h"

class StockIntegrityProblem : public IntegrityProblem
{
public:
	StockIntegrityProblem(ItemId id,const PurchaseObjPath& path,const std::wstring& stockCd);
	virtual PurchaseDownloadIssue ToIssue() const;
	virtual void Accept(ProblemVisitor* visitor);
	virtual Solution* Solve(SolutionParameters& parameters);
	virtual bool IsSameAs(IntegrityProblem* problem);
	const std::wstring& GetStockCd() const;
	virtual IntegrityProblem* Clone() const;
	virtual PurchaseDownloadIssueCategory GetCategory() const;
	virtual std::wstring DescribeProblem() const;
private:
	std::wstring m_stockCd;
};

