#include "stdafx.h"
#include "InvoiceHentProblemDetectingVisitor.h"
#include "InvoiceHentIntegrityProblem.h"

InvoiceHentProblemDetectingVisitor::InvoiceHentProblemDetectingVisitor(const HentNo& hentNo) : m_hentNo(hentNo), m_detected(false)
{
}

InvoiceHentProblemDetectingVisitor::InvoiceHentProblemDetectingVisitor(const HentNo& hentNo, const PurchaseObjPath& path)
: InvoiceHentProblemDetectingVisitor(hentNo)
{
	m_path = path;
}

void InvoiceHentProblemDetectingVisitor::visit(InvoiceHentIntegrityProblem* problem)
{
	bool samePath = m_path.IsNull() || problem->GetPath() == m_path;
	const IPurchaseHent* hent = problem->GetHent();
	m_detected = samePath && (hent->GetHentNo() == m_hentNo);
}

bool InvoiceHentProblemDetectingVisitor::WasDetected() const
{
	return m_detected;
}
