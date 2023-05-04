#include "stdafx.h"
#include "ProblemVerifingVisitor.h"
#include "AgentIntegrityProblem.h"
#include "FirstOwnerHentIntegrityProblem.h"
#include "InvoiceHentIntegrityProblem.h"
#include "HerdIntegrityProblem.h"
#include "ClassIntegrityProblem.h"
#include "StockIntegrityProblem.h"
#include "InStockCowDuplicationProblem.h"

ProblemVerifingVisitor::ProblemVerifingVisitor(SeasonSessionFacade* facade) : m_verified(false), m_facade(facade)
{
}

void ProblemVerifingVisitor::visit(AgentIntegrityProblem* problem)
{
	m_verified = (m_facade->FindAgent(problem->GetAgentCd()) == NULL);
}

void ProblemVerifingVisitor::visit(HerdIntegrityProblem* problem)
{
	int herdIndex = problem->GetHerdIndex();
	m_verified = (m_facade->FindHerdByIndex(herdIndex) == NULL);
}

void ProblemVerifingVisitor::visit(FirstOwnerHentIntegrityProblem* problem)
{
	const IPurchaseHent* hent = problem->GetHent();
	m_verified = (m_facade->FindHent(hent->GetHentNo()) == NULL);
}

void ProblemVerifingVisitor::visit(InvoiceHentIntegrityProblem* problem)
{
	const IPurchaseHent* hent = problem->GetHent();
	m_verified = (m_facade->FindHent(hent->GetHentNo()) == NULL);
}

void ProblemVerifingVisitor::visit(ClassIntegrityProblem* problem)
{
	std::wstring classCd = problem->GetClassCd();
	m_verified = (m_facade->FindClass(classCd) == NULL);
}

void ProblemVerifingVisitor::visit(StockIntegrityProblem* problem)
{
	std::wstring stockCd = problem->GetStockCd();
	m_verified = (m_facade->FindStock(stockCd) == NULL);
}

bool ProblemVerifingVisitor::IsVerified() const
{
	return m_verified;
}

void ProblemVerifingVisitor::visit(InStockCowDuplicationProblem* problem)
{
	int herdIndex = problem->GetHerdIndex();
	IHerd* herd = m_facade->FindHerdByIndex(herdIndex);
	if (herd != NULL)
	{
		m_verified = m_facade->IsCowInStock(herd->GetId(), problem->GetCowNo());
	}
}
