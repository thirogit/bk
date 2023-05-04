#include "stdafx.h"
#include "ClassProblemDetectingVisitor.h"
#include "ClassIntegrityProblem.h"

ClassProblemDetectingVisitor::ClassProblemDetectingVisitor(const std::wstring& classCd) : m_classCd(classCd), m_detected(false)
{
}

ClassProblemDetectingVisitor::ClassProblemDetectingVisitor(const std::wstring& classCd, const PurchaseObjPath& path)
: ClassProblemDetectingVisitor(classCd)
{
	m_path = path;
}

void ClassProblemDetectingVisitor::visit(ClassIntegrityProblem* problem)
{
	bool samePath = m_path.IsNull() || problem->GetPath() == m_path;
	m_detected = samePath && (problem->GetClassCd() == m_classCd);
}

bool ClassProblemDetectingVisitor::WasDetected() const
{
	return m_detected;
}
