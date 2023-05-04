#include "stdafx.h"
#include "ClassIntegrityProblem.h"
#include <errortoken/ErrorToken.h>
#include "ClassIntegritySolution.h"
#include "ClassProblemDetectingVisitor.h"

ClassIntegrityProblem::ClassIntegrityProblem(ItemId id, const PurchaseObjPath& path, const std::wstring& classCd) : IntegrityProblem(id, path),m_classCd(classCd)
{
	
}

PurchaseDownloadIssue ClassIntegrityProblem::ToIssue() const
{	
	PurchaseDownloadIssue issue(GetId());
	issue.SetCategory(GetCategory());
	issue.SetPath(GetPath());
	issue.SetError(ErrorToken(L"repo_issue_no_class")(L"classCd", m_classCd).str());
	issue.SetSolutions(SolutionType_Replace);
	return issue;
}

std::wstring ClassIntegrityProblem::DescribeProblem() const
{
	return L"no class with code " + m_classCd;
}


PurchaseDownloadIssueCategory ClassIntegrityProblem::GetCategory() const
{
	return PurchaseDownloadIssueCategory(PurchaseDownloadIssueDomain_Class, m_classCd);
}

void ClassIntegrityProblem::Accept(ProblemVisitor* visitor)
{
	visitor->visit(this);
}

Solution* ClassIntegrityProblem::Solve(SolutionParameters& parameters)
{
	return new ClassIntegritySolution(parameters.type(), GetPath(), m_classCd, parameters.parameter_default<std::wstring>(L""));
}

const std::wstring& ClassIntegrityProblem::GetClassCd() const
{
	return m_classCd;
}

bool ClassIntegrityProblem::IsSameAs(IntegrityProblem* problem)
{
	ClassProblemDetectingVisitor visitor(m_classCd, GetPath());
	problem->Accept(&visitor);
	return visitor.WasDetected();
}

IntegrityProblem* ClassIntegrityProblem::Clone() const
{
	return new ClassIntegrityProblem(GetId(), GetPath(), GetClassCd());
}
