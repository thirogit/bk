#pragma once

#include <data/runtime/IItem.h>
#include <data/repo/PurchaseObjPath.h>
#include <data/runtime/PurchaseDownloadIssue.h>
#include "ProblemVisitor.h"
#include "Solution.h"
#include <data/runtime/SolutionParameters.h>
#include <data/runtime/PurchaseDownloadIssueCategory.h>

class IntegrityProblem : public IItem
{
public:
	IntegrityProblem(ItemId id,const PurchaseObjPath& path);
	virtual ItemId GetId() const;
	const PurchaseObjPath& GetPath() const;
	virtual PurchaseDownloadIssue ToIssue() const = 0;
	virtual void Accept(ProblemVisitor* visitor) = 0;
	virtual Solution* Solve(SolutionParameters& parameters) = 0;
	virtual bool IsSameAs(IntegrityProblem* problem) = 0;
	virtual IntegrityProblem* Clone() const = 0;
	virtual PurchaseDownloadIssueCategory GetCategory() const = 0;
	virtual std::wstring DescribeProblem() const = 0;
private:
	ItemId m_id;
	PurchaseObjPath m_path;
};
