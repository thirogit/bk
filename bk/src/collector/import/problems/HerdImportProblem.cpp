#include "stdafx.h"
#include "HerdImportProblem.h"
//#include <errortoken/ErrorToken.h>
//#include "HerdIntegritySolution.h"
//#include "HerdProblemDetectingVisitor.h"
//#include <string/TextUtils.h>
//
//HerdIntegrityProblem::HerdIntegrityProblem(ItemId id, int herdIndex) :
//	IntegrityProblem(id, path), m_herdIndex(herdIndex)
//{
//}
//
//PurchaseDownloadIssue HerdIntegrityProblem::ToIssue() const
//{
//	PurchaseDownloadIssue issue(GetId());
//	issue.SetCategory(GetCategory());
//	issue.SetPath(GetPath());
//	issue.SetError(ErrorToken(L"repo_issue_no_herd")(L"herdNo", m_herdIndex).str());
//	issue.SetSolutions(SolutionType_Replace);
//	return issue;
//}
//
//std::wstring HerdIntegrityProblem::DescribeProblem() const
//{
//	return L"no herd with index " + m_herdIndex;
//}
//
//PurchaseDownloadIssueCategory HerdIntegrityProblem::GetCategory() const
//{
//	return PurchaseDownloadIssueCategory(PurchaseDownloadIssueDomain_Herd, TextUtils::Format(L"%03d", m_herdIndex));
//}
//
//void HerdIntegrityProblem::Accept(ProblemVisitor* visitor)
//{
//	visitor->visit(this);
//}
//
//Solution* HerdIntegrityProblem::Solve(SolutionParameters& parameters)
//{
//	return new HerdIntegritySolution(parameters.type(), GetPath(), m_herdIndex, parameters.parameter<int>());
//}
//
//bool HerdIntegrityProblem::IsSameAs(IntegrityProblem* problem)
//{
//	HerdProblemDetectingVisitor visitor(m_herdIndex);
//	problem->Accept(&visitor);
//	return visitor.WasDetected();
//}
//
//int HerdIntegrityProblem::GetHerdIndex() const
//{
//	return m_herdIndex;
//}
//
//IntegrityProblem* HerdIntegrityProblem::Clone() const
//{
//	return new HerdIntegrityProblem(GetId(), GetPath(), GetHerdIndex());
//}
