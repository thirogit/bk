#pragma once
#include "EmptyProblemVisitor.h"
#include <string>
#include <data/repo/PurchaseObjPath.h>
#include <data/types/HentNo.h>

class InvoiceHentProblemDetectingVisitor : public EmptyProblemVisitor
{
public:
	InvoiceHentProblemDetectingVisitor(const HentNo& hentNo);
	InvoiceHentProblemDetectingVisitor(const HentNo& hentNo, const PurchaseObjPath& path);
	virtual void visit(InvoiceHentIntegrityProblem* problem);
	bool WasDetected() const;
private:
	HentNo m_hentNo;
	PurchaseObjPath m_path;
	bool m_detected;
};
