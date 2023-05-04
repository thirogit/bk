#pragma once

#include "Solution.h"
#include <data/runtime/SolutionType.h>
#include <string>
#include <data/repo/PurchaseObjPath.h>

class StockIntegritySolution : public Solution
{
public:
	StockIntegritySolution(SolutionType type, const PurchaseObjPath& path, const std::wstring& problemStockCd, const std::wstring& fixStockCd);
	virtual bool IsSolving(IntegrityProblem* problem);
	virtual void Accept(SolutionVisitor* visitor);
	virtual Solution* Generalize() const;
	virtual bool IsImmediate() const;
	
	SolutionType GetType() const;
	const std::wstring& GetProblemStockCd() const;
	const std::wstring& GetFixStockCd() const;
	const PurchaseObjPath& GetPath() const;
	
private:
	SolutionType m_type;
	PurchaseObjPath m_path;
	std::wstring m_problemStockCd;
	std::wstring m_fixStockCd;
};

