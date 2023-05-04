#pragma once
#include "EmptyProblemVisitor.h"
#include <string>
#include <data/repo/PurchaseObjPath.h>

class StockProblemDetectingVisitor : public EmptyProblemVisitor
{
public:
	StockProblemDetectingVisitor(const std::wstring& stockCd);
	StockProblemDetectingVisitor(const std::wstring& stockCd, const PurchaseObjPath& path);
	virtual void visit(StockIntegrityProblem* problem);
	bool WasDetected() const;
private:
	std::wstring m_stockCd;
	PurchaseObjPath m_path;
	bool m_detected;
};
