#pragma once
#include "EmptyProblemVisitor.h"
#include "InStockCowDuplicationProblem.h"

class InStockCowDuplictionProblemDetectingVisitor : public EmptyProblemVisitor
{
public:
	InStockCowDuplictionProblemDetectingVisitor(const CowNo& cowNo, int herdIndex, const PurchaseObjPath& path);
	virtual void visit(InStockCowDuplicationProblem* problem);
	bool WasDetected() const;
private:
	CowNo m_cowNo;
	bool m_detected;
	PurchaseObjPath m_path;
};
