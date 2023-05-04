#pragma once

#include "Solution.h"
#include <data/runtime/SolutionType.h>
#include <string>
#include <data/repo/PurchaseObjPath.h>

class ClassIntegritySolution : public Solution
{
public:
	ClassIntegritySolution(SolutionType type, const PurchaseObjPath& path, const std::wstring& problemClassCd, const std::wstring& fixClassCd);
	virtual bool IsSolving(IntegrityProblem* problem);
	virtual void Accept(SolutionVisitor* visitor);
	virtual Solution* Generalize() const;
	virtual bool IsImmediate() const;
	
	SolutionType GetType() const;
	const std::wstring& GetProblemClassCd() const;
	const std::wstring& GetFixClassCd() const;
	const PurchaseObjPath& GetPath() const;
	
private:
	SolutionType m_type;
	PurchaseObjPath m_path;
	std::wstring m_problemClassCd;
	std::wstring m_fixClassCd;
};

