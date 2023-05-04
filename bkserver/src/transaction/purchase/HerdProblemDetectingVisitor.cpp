#include "stdafx.h"
#include "HerdProblemDetectingVisitor.h"
#include "HerdIntegrityProblem.h"

HerdProblemDetectingVisitor::HerdProblemDetectingVisitor(int herdIndex) : m_herdIndex(herdIndex), m_detected(false)
{
}

HerdProblemDetectingVisitor::HerdProblemDetectingVisitor(int herdIndex, const PurchaseObjPath& path)
: HerdProblemDetectingVisitor(herdIndex)
{
	m_path = path;
}

void HerdProblemDetectingVisitor::visit(HerdIntegrityProblem* problem)
{
	bool samePath = m_path.IsNull() || problem->GetPath() == m_path;
	m_detected = samePath && (problem->GetHerdIndex() == m_herdIndex);
}

bool HerdProblemDetectingVisitor::WasDetected() const
{
	return m_detected;
}
