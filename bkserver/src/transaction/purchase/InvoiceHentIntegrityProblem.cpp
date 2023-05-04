#include "stdafx.h"
#include "InvoiceHentIntegrityProblem.h"
#include <errortoken/ErrorToken.h>
#include "InvoiceHentIntegritySolution.h"
#include "InvoiceHentProblemDetectingVisitor.h"

InvoiceHentIntegrityProblem::InvoiceHentIntegrityProblem(ItemId id, const PurchaseObjPath& path, const IPurchaseHent* hent) : IntegrityProblem(id, path)
{
	m_hent = new PurchaseHent(hent->GetId());
	m_hent->CopyFrom(hent);
}

PurchaseDownloadIssue InvoiceHentIntegrityProblem::ToIssue() const
{
	HentNo firstOwnerNo = m_hent->GetHentNo();
	PurchaseDownloadIssue issue(GetId());
	issue.SetCategory(GetCategory());
	issue.SetPath(GetPath());
	issue.SetError(ErrorToken(L"repo_issue_no_invoice_hent")(L"hentNo", m_hent->GetHentNo().ToString())(L"hentName", m_hent->GetName()).str());
	issue.SetSolutions(SolutionType_Replace | SolutionType_Add);
	return issue;
}

std::wstring InvoiceHentIntegrityProblem::DescribeProblem() const
{
	return L"missing invoice hent " + m_hent->GetName() + L" with hentno " + m_hent->GetHentNo().ToString();
}

PurchaseDownloadIssueCategory InvoiceHentIntegrityProblem::GetCategory() const
{
	return PurchaseDownloadIssueCategory(PurchaseDownloadIssueDomain_Hent, m_hent->GetHentNo().ToString());
}

InvoiceHentIntegrityProblem::~InvoiceHentIntegrityProblem()
{
	delete m_hent;
}

void InvoiceHentIntegrityProblem::Accept(ProblemVisitor* visitor)
{
	visitor->visit(this);
}

Solution* InvoiceHentIntegrityProblem::Solve(SolutionParameters& parameters)
{
	SolutionType type = parameters.type();
	std::auto_ptr<PurchaseHent> fixHent;
	if (type == SolutionType_Replace)
	{
		HentNo fixHentNo = parameters.parameter<HentNo>();
		fixHent.reset(new PurchaseHent(NULL_ID));
		fixHent->SetHentNo(fixHentNo);
	}
	
	return new InvoiceHentIntegritySolution(parameters.type(), GetPath(), m_hent, fixHent.get());
}

bool InvoiceHentIntegrityProblem::IsSameAs(IntegrityProblem* problem)
{
	InvoiceHentProblemDetectingVisitor visitor(m_hent->GetHentNo(), GetPath());
	problem->Accept(&visitor);
	return visitor.WasDetected();
}

const IPurchaseHent* InvoiceHentIntegrityProblem::GetHent() const
{
	return m_hent;
}

IntegrityProblem* InvoiceHentIntegrityProblem::Clone() const
{
	return new InvoiceHentIntegrityProblem(GetId(), GetPath(), GetHent());
}
