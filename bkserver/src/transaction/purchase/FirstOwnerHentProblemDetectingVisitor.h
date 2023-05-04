#pragma once
#include "EmptyProblemVisitor.h"
#include <string>
#include <data/repo/PurchaseObjPath.h>
#include <data/types/HentNo.h>

class FirstOwnerHentProblemDetectingVisitor : public EmptyProblemVisitor
{
public:
	FirstOwnerHentProblemDetectingVisitor(const HentNo& hentNo);
	FirstOwnerHentProblemDetectingVisitor(const HentNo& hentNo, const PurchaseObjPath& path);
	virtual void visit(FirstOwnerHentIntegrityProblem* problem);
	bool WasDetected() const;
private:
	HentNo m_hentNo;;
	PurchaseObjPath m_path;
	bool m_detected;
};
