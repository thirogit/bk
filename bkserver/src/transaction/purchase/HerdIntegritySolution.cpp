#include "stdafx.h"
#include "HerdIntegritySolution.h"
#include "SolutionVisitor.h"
#include "HerdProblemDetectingVisitor.h"
#include "IntegrityProblem.h"

HerdIntegritySolution::HerdIntegritySolution(SolutionType type, const PurchaseObjPath& path, int problemHerdIndex, int fixHerdIndex)
{
	m_type = type;
	m_problemHerdIndex = problemHerdIndex;
	m_fixHerdIndex = fixHerdIndex;
	m_path = path;
}

SolutionType HerdIntegritySolution::GetType() const
{
	return m_type;
}

int HerdIntegritySolution::GetProblemHerd() const
{
	return m_problemHerdIndex;
}
int HerdIntegritySolution::GetFixHerd()
{
	return m_fixHerdIndex;
}

bool HerdIntegritySolution::IsSolving(IntegrityProblem* problem)
{
	HerdProblemDetectingVisitor visitor(m_problemHerdIndex);
	problem->Accept(&visitor);
	return visitor.WasDetected();
}

void HerdIntegritySolution::Accept(SolutionVisitor* visitor)
{
	visitor->visit(this);
}

Solution* HerdIntegritySolution::Generalize() const
{
	return new HerdIntegritySolution(m_type, PurchaseObjPath(), m_problemHerdIndex, m_fixHerdIndex);
}

bool HerdIntegritySolution::IsImmediate() const
{
	return GetType() == SolutionType_Replace;
}