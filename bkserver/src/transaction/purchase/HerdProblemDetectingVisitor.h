#pragma once
#include "EmptyProblemVisitor.h"
#include <string>
#include <data/repo/PurchaseObjPath.h>

class HerdProblemDetectingVisitor : public EmptyProblemVisitor
{
public:
	HerdProblemDetectingVisitor(int herdIndex);
	HerdProblemDetectingVisitor(int herdIndex, const PurchaseObjPath& path);
	virtual void visit(HerdIntegrityProblem* problem);
	bool WasDetected() const;
private:
	int m_herdIndex;
	PurchaseObjPath m_path;
	bool m_detected;
};
