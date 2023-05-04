#include "stdafx.h"
#include "StockIntegritySolution.h"
#include "SolutionVisitor.h"
#include "StockProblemDetectingVisitor.h"
#include "IntegrityProblem.h"

StockIntegritySolution::StockIntegritySolution(SolutionType type, const PurchaseObjPath& path, const std::wstring& problemStockCd, const std::wstring& fixStockCd)
{
	m_type = type;
	m_problemStockCd = problemStockCd;
	m_fixStockCd = fixStockCd;
	m_path = path;
}

SolutionType StockIntegritySolution::GetType() const
{
	return m_type;
}

const std::wstring& StockIntegritySolution::GetProblemStockCd() const
{
	return m_problemStockCd;
}

const std::wstring& StockIntegritySolution::GetFixStockCd() const
{
	return m_fixStockCd;
}

bool StockIntegritySolution::IsSolving(IntegrityProblem* problem)
{
	StockProblemDetectingVisitor visitor(m_problemStockCd,GetPath());
	problem->Accept(&visitor);
	return visitor.WasDetected();
}

void StockIntegritySolution::Accept(SolutionVisitor* visitor)
{
	visitor->visit(this);
}

Solution* StockIntegritySolution::Generalize() const
{
	return new StockIntegritySolution(m_type, PurchaseObjPath(), m_problemStockCd, m_fixStockCd);
}

const PurchaseObjPath& StockIntegritySolution::GetPath() const
{
	return m_path;
}

bool StockIntegritySolution::IsImmediate() const
{
	return true;
}