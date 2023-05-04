#include "stdafx.h"
#include "StockIntegrityProblem.h"
#include <errortoken/ErrorToken.h>
#include "StockProblemDetectingVisitor.h"
#include "StockIntegritySolution.h"

StockIntegrityProblem::StockIntegrityProblem(ItemId id, const PurchaseObjPath& path, const std::wstring& stockCd) : 
	IntegrityProblem(id, path),m_stockCd(stockCd)
{
}

PurchaseDownloadIssue StockIntegrityProblem::ToIssue() const
{
	PurchaseDownloadIssue issue(GetId());
	issue.SetCategory(GetCategory());
	issue.SetPath(GetPath());
	issue.SetError(ErrorToken(L"repo_issue_no_stock")(L"stockCd", m_stockCd).str());
	issue.SetSolutions(SolutionType_Replace | SolutionType_Clear);
	return issue;
}

std::wstring StockIntegrityProblem::DescribeProblem() const
{
	return L"no stock with code " + m_stockCd;
}


void StockIntegrityProblem::Accept(ProblemVisitor* visitor)
{
	visitor->visit(this);
}

Solution* StockIntegrityProblem::Solve(SolutionParameters& parameters)
{
	return new StockIntegritySolution(parameters.type(), GetPath(), m_stockCd, parameters.parameter_default<std::wstring>(L""));
}

bool StockIntegrityProblem::IsSameAs(IntegrityProblem* problem)
{
	StockProblemDetectingVisitor visitor(m_stockCd, GetPath());
	problem->Accept(&visitor);
	return visitor.WasDetected();
}

const std::wstring& StockIntegrityProblem::GetStockCd() const
{
	return m_stockCd;
}

IntegrityProblem* StockIntegrityProblem::Clone() const
{
	return new StockIntegrityProblem(GetId(), GetPath(), GetStockCd());
}

PurchaseDownloadIssueCategory StockIntegrityProblem::GetCategory() const
{
	return PurchaseDownloadIssueCategory(PurchaseDownloadIssueDomain_Stock, m_stockCd);
}
