#pragma once

#include "Solution.h"
#include <data/runtime/SolutionType.h>
#include <string>
#include <data/repo/PurchaseObjPath.h>

class HerdIntegritySolution : public Solution
{
public:
	HerdIntegritySolution(SolutionType type, const PurchaseObjPath& path, int problemHerdIndex, int fixHerdIndex);
	virtual bool IsSolving(IntegrityProblem* problem);
	virtual void Accept(SolutionVisitor* visitor);
	virtual Solution* Generalize() const;
	virtual bool IsImmediate() const;
	
	SolutionType GetType() const;
	int GetProblemHerd() const;
	int GetFixHerd();
	
private:
	SolutionType m_type;
	PurchaseObjPath m_path;
	int m_problemHerdIndex;
	int m_fixHerdIndex;
};

