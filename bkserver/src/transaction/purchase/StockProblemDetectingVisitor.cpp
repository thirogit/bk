#include "stdafx.h"
#include "StockProblemDetectingVisitor.h"
#include "StockIntegrityProblem.h"

StockProblemDetectingVisitor::StockProblemDetectingVisitor(const std::wstring& stockCd) : m_stockCd(stockCd), m_detected(false)
{
}

StockProblemDetectingVisitor::StockProblemDetectingVisitor(const std::wstring& stockCd, const PurchaseObjPath& path)
: StockProblemDetectingVisitor(stockCd)
{
	m_path = path;
}

void StockProblemDetectingVisitor::visit(StockIntegrityProblem* problem)
{
	bool samePath = m_path.IsNull() || problem->GetPath() == m_path;
	m_detected = samePath && (problem->GetStockCd() == m_stockCd);
}

bool StockProblemDetectingVisitor::WasDetected() const
{
	return m_detected;
}
