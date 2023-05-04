#include "stdafx.h"
#include "FirstOwnerHentProblemDetectingVisitor.h"
#include "FirstOwnerHentIntegrityProblem.h"

FirstOwnerHentProblemDetectingVisitor::FirstOwnerHentProblemDetectingVisitor(const HentNo& hentNo) : m_hentNo(hentNo), m_detected(false)
{
}

FirstOwnerHentProblemDetectingVisitor::FirstOwnerHentProblemDetectingVisitor(const HentNo& hentNo, const PurchaseObjPath& path)
: FirstOwnerHentProblemDetectingVisitor(hentNo)
{
	m_path = path;
}

void FirstOwnerHentProblemDetectingVisitor::visit(FirstOwnerHentIntegrityProblem* problem)
{
	bool samePath = m_path.IsNull() || problem->GetPath() == m_path;
	const IPurchaseHent* hent = problem->GetHent();
	m_detected = samePath && (hent->GetHentNo() == m_hentNo);
}

bool FirstOwnerHentProblemDetectingVisitor::WasDetected() const
{
	return m_detected;
}
