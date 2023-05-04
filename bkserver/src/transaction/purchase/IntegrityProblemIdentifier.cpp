#include "stdafx.h"
#include "IntegrityProblemIdentifier.h"
#include <arrays/DeepDelete.h>
#include "AgentIntegrityProblem.h"
#include "HerdIntegrityProblem.h"
#include "ClassIntegrityProblem.h"
#include "StockIntegrityProblem.h"
#include "FirstOwnerHentIntegrityProblem.h"
#include "InvoiceHentIntegrityProblem.h"
#include "InStockCowDuplicationProblem.h"

IntegrityProblemIdentifier::IntegrityProblemIdentifier(const IPurchase* purchase) : m_Purchase(purchase)
{
}

IntegrityProblemIdentifier::~IntegrityProblemIdentifier()
{
	DeepDelete(m_problems);
}

void IntegrityProblemIdentifier::IdentifyProblems(SeasonSessionFacade* facade)
{
	DeepDelete(m_problems);
	m_problems.clear();

	PurchaseObjPath root(PurchaseObjType_Root, m_Purchase->GetId());

	std::wstring agentCd = m_Purchase->GetAgentCode();
	if (!agentCd.empty())
	{
		if (!facade->FindAgent(agentCd))
		{
			m_problems.push_back(new AgentIntegrityProblem(m_IdGenerator(), root, agentCd));
		}
	}

	int herdIndex = m_Purchase->GetHerd();
	IHerd* herd = facade->FindHerdByIndex(herdIndex);
	if (!herd)
	{
		m_problems.push_back(new HerdIntegrityProblem(m_IdGenerator(), root, herdIndex));
	}
		
	
	PtrEnumerator<IPurchaseInvoice> enumInvoices = m_Purchase->EnumInvoices();
	IPurchaseInvoice* invoice;
	IPurchaseCow* cow;
	while (enumInvoices.hasNext())
	{
		invoice = *enumInvoices;
		PtrEnumerator<IPurchaseCow> enumCows = invoice->EnumCows();

		PurchaseObjPath invoicePath = root / PurchaseObjPath(PurchaseObjType_Invoice, invoice->GetId());

		const IPurchaseHent* invoiceHent = invoice->GetInvoiceHent();

		if (!facade->FindHent(invoiceHent->GetHentNo()))
		{
			m_problems.push_back(new InvoiceHentIntegrityProblem(m_IdGenerator(), invoicePath, invoiceHent));
		}


		while (enumCows.hasNext())
		{
			cow = *enumCows;
			PurchaseObjPath cowPath = invoicePath / PurchaseObjPath(PurchaseObjType_Cow, cow->GetId());

			if (herd != NULL)
			{
				CowNo cowNo = cow->GetCowNo();
				if (facade->IsCowInStock(herd->GetId(),cowNo))
				{
					m_problems.push_back(new InStockCowDuplicationProblem(m_IdGenerator(), cowPath, cowNo, herdIndex));
				}
			}			

			if (!facade->FindClass(cow->GetClassCd()))
			{
				m_problems.push_back(new ClassIntegrityProblem(m_IdGenerator(), cowPath, cow->GetClassCd()));
			}

			std::wstring stockCd = cow->GetStockCd();
			if (!stockCd.empty())
			{
				if (!facade->FindStock(stockCd))
				{
					m_problems.push_back(new StockIntegrityProblem(m_IdGenerator(), cowPath, cow->GetStockCd()));
				}
			}

			
			enumCows.advance();
		}
		enumInvoices.advance();
	}


	
}

size_t IntegrityProblemIdentifier::GetProblemCount() const
{
	return m_problems.size();
}

PtrEnumerator<IntegrityProblem> IntegrityProblemIdentifier::EnumProblems()
{
	return PtrEnumerator<IntegrityProblem>(m_problems);
}



