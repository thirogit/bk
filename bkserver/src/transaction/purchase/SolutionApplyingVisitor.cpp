#include "stdafx.h"
#include "SolutionApplyingVisitor.h"
#include "AgentIntegritySolution.h"
#include "HerdIntegritySolution.h"
#include "FstOwnerHentIntegritySolution.h"
#include "InvoiceHentIntegritySolution.h"
#include "ClassIntegritySolution.h"
#include "StockIntegritySolution.h"
#include "InStockCowDuplicationSolution.h"
#include "PurchaseTraverser.h"
#include <boost/foreach.hpp>

SolutionApplyingVisitor::SolutionApplyingVisitor(Purchase* purchaseToFix) : m_purchaseToFix(purchaseToFix)
{
}

void SolutionApplyingVisitor::visit(AgentIntegritySolution* solution)
{
	switch (solution->GetType())
	{
	case SolutionType_Clear:
		m_purchaseToFix->SetAgentCode(std::wstring());
		break;
	case SolutionType_Replace:
		if (m_purchaseToFix->GetAgentCode() == solution->GetProblemAgentCd())
		{
			m_purchaseToFix->SetAgentCode(solution->GetFixAgentCd());
		}
		break;
	}
}

void SolutionApplyingVisitor::visit(HerdIntegritySolution* solution)
{
	if (solution->GetType() == SolutionType_Replace)
	{
		if (m_purchaseToFix->GetHerd() == solution->GetProblemHerd())
		{
			m_purchaseToFix->SetHerd(solution->GetFixHerd());
		}
	}
}

void SolutionApplyingVisitor::visit(FstOwnerHentIntegritySolution* solution)
{
	PurchaseTraverser traverser(m_purchaseToFix);
	
	PurchaseObjPath path = solution->GetPath();
	SolutionType type = solution->GetType();
	IPurchaseHent* fixhent = solution->GetFixHent();

	std::vector<PurchaseCow*> cows;
	if (path.IsNull())
	{
		cows = traverser.GetCows();
	}
	else
	{
		PurchaseCow* cow = traverser.GetCow(solution->GetPath());
		cows.push_back(cow);
	}
	
	BOOST_FOREACH(PurchaseCow* cow,cows)
	{
		switch (type)
		{
		case SolutionType_Clear:
			cow->SetFirstOwner(HentNo());
			break;
		case SolutionType_Replace:
			{
				IPurchaseHent* hent = solution->GetFixHent();
				cow->SetFirstOwner(hent->GetHentNo());
				if (m_purchaseToFix->GetHent(hent->GetHentNo()) == NULL)
				{
					m_purchaseToFix->AddHent(hent);
				}									
			}
			break;
		}
	}

	if (type == SolutionType_Replace)
	{
		if (m_purchaseToFix->GetHent(fixhent->GetHentNo()) == NULL)
		{
			m_purchaseToFix->AddHent(fixhent);
		}
	}
}

void SolutionApplyingVisitor::visit(InvoiceHentIntegritySolution* solution)
{
	PurchaseTraverser traverser(m_purchaseToFix);
	PurchaseObjPath path = solution->GetPath();
	SolutionType type = solution->GetType();
	IPurchaseHent* fixhent = solution->GetFixHent();

	std::vector<PurchaseInvoice*> invoices;
	if (path.IsNull())
	{
		invoices = traverser.GetInvoices();
	}
	else
	{
		PurchaseInvoice* invoice = traverser.GetInvoice(solution->GetPath());
		invoices.push_back(invoice);
	}
	
	if (type == SolutionType_Replace)
	{
		BOOST_FOREACH(PurchaseInvoice* invoice,invoices)
		{
			if (invoice->GetInvoiceHent()->GetHentNo() == solution->GetProblemHent()->GetHentNo())
			{
				invoice->SetInvoiceHent(fixhent);
			}
		}

		if (m_purchaseToFix->GetHent(fixhent->GetHentNo()) == NULL)
		{
			m_purchaseToFix->AddHent(fixhent);
		}
	}
}

void SolutionApplyingVisitor::visit(ClassIntegritySolution* solution)
{
	PurchaseTraverser traverser(m_purchaseToFix);
	PurchaseObjPath path = solution->GetPath();
	std::wstring problemClassCd = solution->GetProblemClassCd();

	if (solution->GetType() == SolutionType_Replace)
	{
		std::vector<PurchaseCow*> cows;
		if (path.IsNull())
		{
			BOOST_FOREACH(PurchaseCow* cow,traverser.GetCows())
			{
				if (cow->GetClassCd() == problemClassCd)
				{
					cows.push_back(cow);
				}
			};
		}
		else
		{
			PurchaseCow* cow = traverser.GetCow(solution->GetPath());
			cows.push_back(cow);
		}

		BOOST_FOREACH(PurchaseCow* cow, cows)
		{	
			cow->SetClassCd(solution->GetFixClassCd());
		}
	}	
}

void SolutionApplyingVisitor::visit(StockIntegritySolution* solution)
{
	PurchaseTraverser traverser(m_purchaseToFix);
	PurchaseObjPath path = solution->GetPath();
	std::wstring problemStockCd = solution->GetProblemStockCd();

	std::vector<PurchaseCow*> cows;
	if (path.IsNull())
	{
		cows = traverser.GetCows();
		std::remove_if(cows.begin(), cows.end(), [&problemStockCd](PurchaseCow* cow) -> bool
		{
			return cow->GetStockCd() != problemStockCd;
		});
	}
	else
	{
		PurchaseCow* cow = traverser.GetCow(solution->GetPath());
		cows.push_back(cow);
	}

	BOOST_FOREACH(PurchaseCow* cow, cows)
	{	
		switch (solution->GetType())
		{
		case SolutionType_Clear:
			cow->SetStockCd(std::wstring());
			break;
		case SolutionType_Replace:
			cow->SetStockCd(solution->GetFixStockCd());
			break;
		}
	}
}

void SolutionApplyingVisitor::visit(InStockCowDuplicationSolution* solution)
{
	PurchaseTraverser traverser(m_purchaseToFix);
	PurchaseObjPath path = solution->GetPath();
	CowNo problemCowNo = solution->GetProblemCowNo();

	std::vector<PurchaseCow*> cows;
	if (path.IsNull())
	{
		cows = traverser.GetCows();
		std::remove_if(cows.begin(), cows.end(), [&problemCowNo](PurchaseCow* cow) -> bool
		{
			return cow->GetCowNo() != problemCowNo;
		});
	}
	else
	{
		PurchaseCow* cow = traverser.GetCow(solution->GetPath());
		cows.push_back(cow);
	}

	BOOST_FOREACH(PurchaseCow* cow, cows)
	{
		if (solution->GetType() == SolutionType_Replace)
		{
			cow->SetCowNo(solution->GetFixCowNo());		
		}
	}
}