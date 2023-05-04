#include "stdafx.h"
#include "InvoiceHentIntegritySolution.h"
#include "SolutionVisitor.h"
#include "InvoiceHentProblemDetectingVisitor.h"
#include "IntegrityProblem.h"
#include "FirstOwnerHentProblemDetectingVisitor.h"

InvoiceHentIntegritySolution::InvoiceHentIntegritySolution(SolutionType type, const PurchaseObjPath& path, const IPurchaseHent* problemHent, const IPurchaseHent* fixHent)
{
	m_problemHent = new PurchaseHent(problemHent->GetId());
	m_problemHent->CopyFrom(problemHent);
	m_fixHent = NULL;

	if (fixHent)
	{
		m_fixHent = new PurchaseHent(fixHent->GetId());
		m_fixHent->CopyFrom(fixHent);
	}

	m_path = path;
	m_type = type;
}


InvoiceHentIntegritySolution::~InvoiceHentIntegritySolution()
{
	delete m_fixHent;
	delete m_problemHent;
}

bool InvoiceHentIntegritySolution::IsSolving(IntegrityProblem* problem)
{
	InvoiceHentProblemDetectingVisitor visitor1(m_problemHent->GetHentNo(), problem->GetPath());
	problem->Accept(&visitor1);

	if (visitor1.WasDetected()) return true;

	if (m_type == SolutionType_Add)
	{
		FirstOwnerHentProblemDetectingVisitor visitor2(m_problemHent->GetHentNo(), PurchaseObjPath());
		problem->Accept(&visitor2);
		return visitor2.WasDetected();
	}

	return false;
}

void InvoiceHentIntegritySolution::Accept(SolutionVisitor* visitor)
{
	visitor->visit(this);
}

SolutionType InvoiceHentIntegritySolution::GetType() const
{
	return m_type;
}

IPurchaseHent* InvoiceHentIntegritySolution::GetProblemHent() const
{
	return m_problemHent;
}

IPurchaseHent* InvoiceHentIntegritySolution::GetFixHent() const
{
	return m_fixHent;
}

Solution* InvoiceHentIntegritySolution::Generalize() const
{
	return new InvoiceHentIntegritySolution(m_type, PurchaseObjPath(), m_problemHent, m_fixHent);
}

const PurchaseObjPath& InvoiceHentIntegritySolution::GetPath() const
{
	return m_path;

}

bool InvoiceHentIntegritySolution::IsImmediate() const
{
	return false;
}