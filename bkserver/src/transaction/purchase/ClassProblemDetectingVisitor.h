#pragma once
#include "EmptyProblemVisitor.h"
#include <string>
#include <data/repo/PurchaseObjPath.h>

class ClassProblemDetectingVisitor : public EmptyProblemVisitor
{
public:
	ClassProblemDetectingVisitor(const std::wstring& classCd);
	ClassProblemDetectingVisitor(const std::wstring& classCd, const PurchaseObjPath& path);
	virtual void visit(ClassIntegrityProblem* problem);
	bool WasDetected() const;
private:
	std::wstring m_classCd;
	PurchaseObjPath m_path;
	bool m_detected;
};
