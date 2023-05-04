#pragma once

#include "Solution.h"
#include <data/runtime/SolutionType.h>
#include <string>
#include <data/repo/PurchaseObjPath.h>
#include <data/types/CowNo.h>

class InStockCowDuplicationSolution : public Solution
{
public:
	InStockCowDuplicationSolution(SolutionType type, const PurchaseObjPath& path, const CowNo& problemCowNo, int herdIndex, const CowNo& fixCowNo);
	virtual bool IsSolving(IntegrityProblem* problem);
	virtual void Accept(SolutionVisitor* visitor);
	virtual Solution* Generalize() const;
	virtual bool IsImmediate() const;
	
	SolutionType GetType() const;
	const CowNo& GetProblemCowNo() const;
	const CowNo& GetFixCowNo() const;
	const PurchaseObjPath& GetPath() const;
	
private:
	SolutionType m_type;
	PurchaseObjPath m_path;
	CowNo m_problemCowNo;
	CowNo m_fixCowNo;
	int m_herdIndex;
};

