#include "stdafx.h"
#include "InStockCowDuplicationSolution.h"
#include "SolutionVisitor.h"
#include "InStockCowDuplictionProblemDetectingVisitor.h"
#include "IntegrityProblem.h"

InStockCowDuplicationSolution::InStockCowDuplicationSolution(SolutionType type, const PurchaseObjPath& path, const CowNo& problemCowNo,int herdIndex, const CowNo& fixCowNo)
{
	m_type = type;
	m_problemCowNo = problemCowNo;
	m_fixCowNo = fixCowNo;
	m_path = path;
	m_herdIndex = herdIndex;
}

SolutionType InStockCowDuplicationSolution::GetType() const
{
	return m_type;
}

const CowNo& InStockCowDuplicationSolution::GetProblemCowNo() const
{
	return m_problemCowNo;
}

const CowNo& InStockCowDuplicationSolution::GetFixCowNo() const
{
	return m_fixCowNo;
}

bool InStockCowDuplicationSolution::IsSolving(IntegrityProblem* problem)
{
	InStockCowDuplictionProblemDetectingVisitor visitor(m_problemCowNo, m_herdIndex, GetPath());
	problem->Accept(&visitor);
	return visitor.WasDetected();
}

void InStockCowDuplicationSolution::Accept(SolutionVisitor* visitor)
{
	visitor->visit(this);
}

Solution* InStockCowDuplicationSolution::Generalize() const
{
	return new InStockCowDuplicationSolution(m_type, PurchaseObjPath(), m_problemCowNo, m_herdIndex, m_fixCowNo);
}

const PurchaseObjPath& InStockCowDuplicationSolution::GetPath() const
{
	return m_path;
}

bool InStockCowDuplicationSolution::IsImmediate() const
{
	return GetType() == SolutionType_Replace;
}