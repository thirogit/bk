#include "stdafx.h"
#include "InStockCowDuplictionProblemDetectingVisitor.h"
#include "InStockCowDuplicationProblem.h"

InStockCowDuplictionProblemDetectingVisitor::InStockCowDuplictionProblemDetectingVisitor(const CowNo& cowNo, int herdIndex, const PurchaseObjPath& path) : 
	m_cowNo(cowNo), m_detected(false), m_path(path)
{

}

void InStockCowDuplictionProblemDetectingVisitor::visit(InStockCowDuplicationProblem* problem)
{
	m_detected = false;
	PurchaseObjPath problemPath = problem->GetPath();
	if ((!problemPath.IsNull() && problemPath == m_path) || m_path.IsNull())
	{
		m_detected = problem->GetCowNo() == m_cowNo;
	}	
}

bool InStockCowDuplictionProblemDetectingVisitor::WasDetected() const
{
	return m_detected;
}
