#include "stdafx.h"
#include "InStockCowDuplicationProblem.h"
#include <errortoken/ErrorToken.h>
#include "InStockCowDuplicationSolution.h"
#include "InStockCowDuplictionProblemDetectingVisitor.h"
#include <string/TextUtils.h>

InStockCowDuplicationProblem::InStockCowDuplicationProblem(ItemId id, const PurchaseObjPath& path, const CowNo& cowNo, int herdIndex) :
IntegrityProblem(id, path), m_cowNo(cowNo), m_herdIndex(herdIndex)
{
}

PurchaseDownloadIssue InStockCowDuplicationProblem::ToIssue() const
{
	PurchaseDownloadIssue issue(GetId());
	issue.SetCategory(GetCategory());
	issue.SetPath(GetPath());
	issue.SetError(ErrorToken(L"repo_issue_herd_cow_duplication")(L"cow_no", m_cowNo.ToString())(L"herd_index",TextUtils::Format(L"%03d",m_herdIndex)).str());
	issue.SetSolutions(SolutionType_Replace);
	return issue;
};


std::wstring InStockCowDuplicationProblem::DescribeProblem() const
{
	return L"duplicated cow " + m_cowNo.ToString();
}

PurchaseDownloadIssueCategory InStockCowDuplicationProblem::GetCategory() const
{
	return PurchaseDownloadIssueCategory(PurchaseDownloadIssueDomain_Cow, m_cowNo.ToString());
}

void InStockCowDuplicationProblem::Accept(ProblemVisitor* visitor)
{
	visitor->visit(this);
}

Solution* InStockCowDuplicationProblem::Solve(SolutionParameters& parameters)
{
	return new InStockCowDuplicationSolution(parameters.type(), GetPath(), m_cowNo, m_herdIndex, parameters.parameter_default<CowNo>(CowNo()));
}

bool InStockCowDuplicationProblem::IsSameAs(IntegrityProblem* problem)
{
	InStockCowDuplictionProblemDetectingVisitor visitor(m_cowNo, m_herdIndex, GetPath());
	problem->Accept(&visitor);
	return visitor.WasDetected();
}

const CowNo& InStockCowDuplicationProblem::GetCowNo() const
{
	return m_cowNo;
}

int InStockCowDuplicationProblem::GetHerdIndex() const
{
	return m_herdIndex;
}

IntegrityProblem* InStockCowDuplicationProblem::Clone() const
{
	return new InStockCowDuplicationProblem(GetId(), GetPath(), GetCowNo(), GetHerdIndex());
}
