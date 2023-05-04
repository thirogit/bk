#include "stdafx.h"
#include "FirstOwnerHentIntegrityProblem.h"
#include <errortoken/ErrorToken.h>
#include "FirstOwnerHentProblemDetectingVisitor.h"
#include "FstOwnerHentIntegritySolution.h"

FirstOwnerHentIntegrityProblem::FirstOwnerHentIntegrityProblem(ItemId id, const PurchaseObjPath& path, const IPurchaseHent* hent) :
	IntegrityProblem(id, path)
{
	m_hent = new PurchaseHent(hent->GetId());
	m_hent->CopyFrom(hent);
}

PurchaseDownloadIssue FirstOwnerHentIntegrityProblem::ToIssue() const
{
	HentNo firstOwnerNo = m_hent->GetHentNo();
	PurchaseDownloadIssue issue(GetId());
	issue.SetCategory(GetCategory());
	issue.SetPath(GetPath());
	issue.SetError(ErrorToken(L"repo_issue_no_firstowner")(L"hentNo", firstOwnerNo.ToString())(L"hentName", m_hent->GetName()).str());
	issue.SetSolutions(SolutionType_Replace | SolutionType_Add | SolutionType_Clear);
	return issue;
}

std::wstring FirstOwnerHentIntegrityProblem::DescribeProblem() const
{
	return L"missing first owner " + m_hent->GetName() + L" with hentno " + m_hent->GetHentNo().ToString();
}

PurchaseDownloadIssueCategory FirstOwnerHentIntegrityProblem::GetCategory() const
{
	return PurchaseDownloadIssueCategory(PurchaseDownloadIssueDomain_Hent, m_hent->GetHentNo().ToString());
}

FirstOwnerHentIntegrityProblem::~FirstOwnerHentIntegrityProblem()
{
	delete m_hent;
}

bool FirstOwnerHentIntegrityProblem::IsSameAs(IntegrityProblem* problem)
{
	FirstOwnerHentProblemDetectingVisitor visitor(m_hent->GetHentNo(), GetPath());
	problem->Accept(&visitor);
	return visitor.WasDetected();
}

Solution* FirstOwnerHentIntegrityProblem::Solve(SolutionParameters& parameters)
{
	SolutionType type = parameters.type();
	std::auto_ptr<PurchaseHent> fixHent;
	if (type == SolutionType_Replace)
	{
		HentNo fixHentNo = parameters.parameter<HentNo>();
		fixHent.reset(new PurchaseHent(NULL_ID));
		fixHent->SetHentNo(fixHentNo);
	}
	
	FstOwnerHentIntegritySolution* solution = new FstOwnerHentIntegritySolution(parameters.type(), GetPath(), m_hent, fixHent.get());
	return solution;
}

void FirstOwnerHentIntegrityProblem::Accept(ProblemVisitor* visitor)
{
	visitor->visit(this);
}

const IPurchaseHent* FirstOwnerHentIntegrityProblem::GetHent() const
{
	return m_hent;
}

IntegrityProblem* FirstOwnerHentIntegrityProblem::Clone() const
{
	return new FirstOwnerHentIntegrityProblem(GetId(), GetPath(), GetHent());
}
