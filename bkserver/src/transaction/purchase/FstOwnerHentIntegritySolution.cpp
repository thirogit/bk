#include "stdafx.h"
#include "FstOwnerHentIntegritySolution.h"
#include "SolutionVisitor.h"
#include "FirstOwnerHentProblemDetectingVisitor.h"
#include "IntegrityProblem.h"
#include "InvoiceHentProblemDetectingVisitor.h"

FstOwnerHentIntegritySolution::FstOwnerHentIntegritySolution(SolutionType type, const PurchaseObjPath& path, const IPurchaseHent* problemHent, const IPurchaseHent* fixHent)
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


FstOwnerHentIntegritySolution::~FstOwnerHentIntegritySolution()
{
	delete m_fixHent;
	delete m_problemHent;
}

bool FstOwnerHentIntegritySolution::IsSolving(IntegrityProblem* problem)
{
	FirstOwnerHentProblemDetectingVisitor visitor1(m_problemHent->GetHentNo(), problem->GetPath());
	problem->Accept(&visitor1);
	
	if (visitor1.WasDetected()) return true;

	if (m_type == SolutionType_Add)
	{
		InvoiceHentProblemDetectingVisitor visitor2(m_problemHent->GetHentNo(), PurchaseObjPath());
		problem->Accept(&visitor2);
		return visitor2.WasDetected();
	}

	return false;
}

void FstOwnerHentIntegritySolution::Accept(SolutionVisitor* visitor)
{
	visitor->visit(this);
}

SolutionType FstOwnerHentIntegritySolution::GetType() const
{
	return m_type;
}

IPurchaseHent* FstOwnerHentIntegritySolution::GetProblemHent() const
{
	return m_problemHent;
}

IPurchaseHent* FstOwnerHentIntegritySolution::GetFixHent() const
{
	return m_fixHent;
}

const PurchaseObjPath& FstOwnerHentIntegritySolution::GetPath() const
{
	return m_path;
}

Solution* FstOwnerHentIntegritySolution::Generalize() const
{
	return new FstOwnerHentIntegritySolution(m_type, PurchaseObjPath(), m_problemHent, m_fixHent);
}

bool FstOwnerHentIntegritySolution::IsImmediate() const
{
	return false;
}