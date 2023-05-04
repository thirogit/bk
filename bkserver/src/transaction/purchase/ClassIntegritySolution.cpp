#include "stdafx.h"
#include "ClassIntegritySolution.h"
#include "SolutionVisitor.h"
#include "ClassProblemDetectingVisitor.h"
#include "IntegrityProblem.h"

ClassIntegritySolution::ClassIntegritySolution(SolutionType type, const PurchaseObjPath& path, const std::wstring& problemClassCd, const std::wstring& fixClassCd)
{
	m_type = type;
	m_problemClassCd = problemClassCd;
	m_fixClassCd = fixClassCd;
	m_path = path;
}

SolutionType ClassIntegritySolution::GetType() const
{
	return m_type;
}

const std::wstring& ClassIntegritySolution::GetProblemClassCd() const
{
	return m_problemClassCd;
}

const std::wstring& ClassIntegritySolution::GetFixClassCd() const
{
	return m_fixClassCd;
}

bool ClassIntegritySolution::IsSolving(IntegrityProblem* problem)
{
	ClassProblemDetectingVisitor visitor(m_problemClassCd,GetPath());
	problem->Accept(&visitor);
	return visitor.WasDetected();
}

void ClassIntegritySolution::Accept(SolutionVisitor* visitor)
{
	visitor->visit(this);
}

Solution* ClassIntegritySolution::Generalize() const
{
	return new ClassIntegritySolution(m_type, PurchaseObjPath(), m_problemClassCd, m_fixClassCd);
}

const PurchaseObjPath& ClassIntegritySolution::GetPath() const
{
	return m_path;
}

bool ClassIntegritySolution::IsImmediate() const
{
	return true;
}