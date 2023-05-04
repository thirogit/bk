#include "stdafx.h"
#include "InsertPurchaseOperation.h"
#include "../../SeasonSession.h"
#include "../../SessionException.h"
#include <errortoken/ErrorToken.h>
#include "../ScopedUpdateState.h"
#include <arrays/DeepDelete.h>
#include <boost/foreach.hpp>
#include <string/TextUtils.h>
#include <data/datalimits.h>

InsertPurchaseOperation::InsertPurchaseOperation(IPurchase* purchase, SeasonSession* session, LogInterface* logger) :
	LoggerAwareSeasonOperation(session, logger),
	m_InDoc(NULL),
	m_purchase(purchase)
{
}

void InsertPurchaseOperation::SetDocDate(const DateTime& docDate)
{
	m_docDate = docDate;
}

void InsertPurchaseOperation::execute()
{	
	Cleanup();

	Log(LogLevel_Debug, L"insert purchase operation, purchase id = %d", m_purchase->GetId());

	IHerd* herd = m_session->FindHerdByIndex(m_purchase->GetHerd());
	if (!herd)
	{
		Log(LogLevel_Error, L"purchase references invalid herd %03d",m_purchase->GetHerd());
		BOOST_THROW_EXCEPTION(SessionException(ErrorToken(L"invalid_herd")(L"herd_index", m_purchase->GetHerd()).str()));
	}

	HerdRegistry* pHerdRegistry = m_session->m_Registers.at(herd->GetId());
	DocDetails docDetails = ExtractDocDetails(m_purchase);
	
	Log(LogLevel_Info, L"indoc created from purchase will contain following details: "
		L"docDate=%s,loadDate=%s,plateNo=%s,loadStartDtTm=%s,loadEndDtTm=%s,agentId=%d",
		docDetails.GetDocDate().GetDayDate(),
		docDetails.GetLoadDate().GetDayDate(),
		docDetails.GetPlateNo(),
		docDetails.GetLoadStartDtTm().GetTime(Time::HHcolonMI),
		docDetails.GetLoadEndDtTm().GetTime(Time::HHcolonMI),
		docDetails.GetAgentId());
	
	PtrEnumerator<IPurchaseInvoice> enumInvoices = m_purchase->EnumInvoices();
	IPurchaseInvoice* purchaseInvoice;	
	IPurchaseCow* purchaseCow;

	Log(LogLevel_Debug, L"validating cows references");
	while (enumInvoices.hasNext())
	{
		purchaseInvoice = *enumInvoices;
		
		PtrEnumerator<IPurchaseCow> enumCows = purchaseInvoice->EnumCows();
		while (enumCows.hasNext())
		{
			purchaseCow = *enumCows;
			std::wstring classCd = purchaseCow->GetClassCd();
			if (!m_session->FindClass(classCd))
			{
				BOOST_THROW_EXCEPTION(SessionException(ErrorToken(L"class_not_found")(L"class_cd", classCd).str()));
			}

			std::wstring stockCd = purchaseCow->GetStockCd();
			if (!stockCd.empty())
			{
				if (!m_session->FindStock(stockCd))
				{
					BOOST_THROW_EXCEPTION(SessionException(ErrorToken(L"stock_not_found")(L"stock_cd", stockCd).str()));
				}
			}
		
			m_session->CheckForInStockDuplication(herd->GetId(), purchaseCow->GetCowNo());
			enumCows.advance();
		}
		enumInvoices.advance();
	}

	ScopedUpdateState state(m_session);

	Transaction transaction = m_session->m_DAO.OpenTransaction();
		
	Log(LogLevel_Debug, L"collecting missing hents");
	try
	{				
		IPurchaseHent* purchaseHent;
		PtrEnumerator<IPurchaseHent> enumHents = m_purchase->EnumHents();
		while (enumHents.hasNext())
		{
			purchaseHent = *enumHents;

			HentServerObject* hentSO = m_session->FindHent(purchaseHent->GetHentNo());
			Hent* hent;
			if (!hentSO)
			{
				hent = InsertPurchaseHent(transaction,purchaseHent);
				m_newHents.push_back(hent);
			}
			else
			{
				hent = hentSO->Object();
			}
			m_hents.add(hent);
			enumHents.advance();
		}

		
		std::auto_ptr<InDoc> newInDoc(new InDoc(NULL_ID));
		newInDoc->SetDetails(docDetails);
		newInDoc->SetHerdId(herd->GetId());
				
		Cow* pCow;
						
		Log(LogLevel_Debug, L"inserting purchase");
		enumInvoices = m_purchase->EnumInvoices();
		while (enumInvoices.hasNext())
		{
			purchaseInvoice = *enumInvoices;
			std::auto_ptr<BuyInvoice> buyInvoice(new BuyInvoice(NULL_ID,InvoiceScope_Cows));
			
			InvoiceDetails invoiceDetails = ExtractInvoiceDetails(purchaseInvoice);
			m_session->ValidateInvoiceDetails(invoiceDetails);
			buyInvoice->SetInvoiceNo(m_session->ObtainNextBuyInvoiceNo(transaction));
			buyInvoice->SetInvoiceType(purchaseInvoice->GetInvoiceType());
			buyInvoice->SetFrom(invoiceDetails);

			InvoiceHent* invoiceHent = CreateInvoiceHent(purchaseInvoice->GetInvoiceHent());
			buyInvoice->SetInvoiceHent(*invoiceHent);
			delete invoiceHent;
			
			PtrEnumerator<IPurchaseCow> enumCows = purchaseInvoice->EnumCows();			
			while (enumCows.hasNext())
			{
				purchaseCow = *enumCows;
				CowDetails cowDetails = ExtractCowDetails(purchaseCow);
				pCow = m_session->m_DAO.InsertCow(transaction, m_session->m_pSeason->GetId(), cowDetails);
				m_cows.add(pCow);
				newInDoc->AddCow(pCow->GetId());
				CowInvoiceEntry* invoiceEntry = buyInvoice->AddEntry(pCow->GetId());
				invoiceEntry->SetPrice(purchaseCow->GetPrice());
				invoiceEntry->SetWeight(purchaseCow->GetWeight());
				invoiceEntry->SetClassId(cowDetails.GetClassId());				
				
				enumCows.advance();
			}

			PtrEnumerator<IPurchaseInvoiceDeduction> enumDeductions = purchaseInvoice->EnumDeductions();
			while (enumDeductions.hasNext())
			{
				buyInvoice->AppendDeduction(CreateDeduction(*enumDeductions));
				enumDeductions.advance();
			}
			
						
			BuyInvoice* insertedInvoice = m_session->m_DAO.InsertBuyInvoice(transaction, m_session->m_pSeason->GetId(), buyInvoice.get());
			m_invoices.push_back(insertedInvoice);
			
			PtrEnumerator<ICowInvoiceEntry> enumEntries = insertedInvoice->EnumEntries();
			while (enumEntries.hasNext())
			{
				ICowInvoiceEntry* entry = *enumEntries;
				m_session->m_DAO.UpdateCowWithBuyInvoice(transaction, m_cows.find(entry->GetCowId())->GetId(), insertedInvoice->GetId());
				enumEntries.advance();
			}

			enumInvoices.advance();	
		}

		m_InDoc = m_session->m_DAO.InsertInDoc(transaction, m_session->m_pSeason->GetId(), newInDoc.get());

		transaction->Commit();		
		Log(LogLevel_Debug, L"inserting purchase successful - commit invoked");
						
	}
	catch (...)
	{
		Log(LogLevel_Debug, L"error inserting purchase");
		transaction->Rollback();
		Cleanup();
		std::rethrow_exception(std::current_exception());
	}

	Log(LogLevel_Debug, L"creating notifications");
	
	ObserverTransactionPtr observerTransaction = m_session->m_pSpectator->OpenTransaction();
	
	BOOST_FOREACH(Hent* newHent, m_newHents)
	{
		m_session->m_Hents.insert(new HentServerObject(newHent));
		observerTransaction->OnHentInserted(newHent);
	}

	m_session->m_InDocs.add(new InDocServerObject(m_InDoc));
	pHerdRegistry->AddInDoc(m_InDoc);

	ICowEntry* pEntry;

	for (CowsIndex::iterator cowIt = m_cows.begin(); cowIt != m_cows.end(); cowIt++)
	{
		Cow* pCow = *cowIt;
		m_session->m_Cows.add(new CowServerObject(pCow));
		observerTransaction->OnCowInserted(pCow);
		pEntry = m_InDoc->GetCow(pCow->GetId());
		pHerdRegistry->AddEnteringEvent(pCow, m_InDoc, pEntry);
		CowTrail* pTrail = m_session->m_Tracker.StartTrail(pCow);
		pTrail->SetEntryFootprint(m_InDoc, pEntry);
		m_session->m_CowNoReference.AddReference(pCow->GetCowNo(), pCow->GetId());
	}

	observerTransaction->OnInDocInserted(m_InDoc);

	BOOST_FOREACH(BuyInvoice* buyInvoice, m_invoices)
	{
		m_session->m_BuyInvoices.add(new BuyInvoiceServerObject(buyInvoice));
		m_session->m_BuyInvoiceRelations.AddRelationship(buyInvoice->GetId(), DocKey(DocType_In, m_InDoc->GetId()));

		observerTransaction->OnBuyInvoiceInserted(buyInvoice);

		uint32_t invoiceId = buyInvoice->GetId();

		PtrEnumerator<ICowInvoiceEntry> enumEntries = buyInvoice->EnumEntries();
		
		while (enumEntries.hasNext())
		{
			ICowInvoiceEntry* entry = *enumEntries;
			Cow* cow = m_cows.find(entry->GetCowId());
			cow->SetBuyInvoiceId(invoiceId);
			observerTransaction->OnCowUpdated(cow);
			enumEntries.advance();
		}
	}
	observerTransaction->Commit();
}

void InsertPurchaseOperation::Cleanup()
{
	DeepDelete(m_cows);
	m_cows.clear();
	delete m_InDoc;
	m_InDoc = NULL;
	DeepDelete(m_invoices);
	m_invoices.clear();
	DeepDelete(m_newHents);
	m_newHents.clear();
	m_hents.clear();
}
	
CowDetails InsertPurchaseOperation::ExtractCowDetails(const IPurchaseCow* cow)
{
	CowDetails details;
		
	details.SetWeight(cow->GetWeight());

	std::wstring stockCd = cow->GetStockCd();
	if (!stockCd.empty())
	{
		IStock* stock = m_session->FindStock(stockCd);
		details.SetStockId(stock->GetId());
	}

	IClass* cowclass = m_session->FindClass(cow->GetClassCd());
	details.SetClassId(cowclass->GetId());

	details.SetCowNo(cow->GetCowNo());
	details.SetMotherNo(cow->GetMotherNo());
	details.SetBirthDate(cow->GetBirthDt());
	details.SetPassNo(cow->GetPassportNo());
	details.SetHealthCertNo(cow->GetHealthCertNo());
	details.SetPassDate(cow->GetPassportIssueDt());
	details.SetBirthPlace(cow->GetBirthPlace());
	details.SetSex(cow->GetSex());

	HentNo firstOwnerNo = cow->GetFirstOwner();
	if (!firstOwnerNo.IsNull())
	{
		IHent* firstOwner = m_hents.find(firstOwnerNo);
		details.SetFirstOwnerId(firstOwner->GetId());
	}	
		
	return details;
}

DocDetails InsertPurchaseOperation::ExtractDocDetails(const IPurchase* purchase)
{
	DocDetails details;
	
	if (m_docDate.IsNull())
	{
		details.SetDocDate(DateTime::now());
	}
	else
	{
		details.SetDocDate(m_docDate);
	}
	
	
	details.SetPlateNo(purchase->GetPlateNo());
	details.SetMotive(Motive_Buy);
	
	details.SetLoadDate(purchase->GetStartDate());
	//details.SetLoadStartDtTm(const Time& loadStartDtTm);
	//details.SetLoadEndDtTm(const Time& loadEndDtTm);

	std::wstring agentCd = purchase->GetAgentCode();
	if (!agentCd.empty())
	{
		IAgent* agent = m_session->FindAgent(agentCd);

		if (!agent)
		{			
			BOOST_THROW_EXCEPTION(SessionException(ErrorToken(L"unknown_agent")(L"agent_cd", purchase->GetAgentCode()).str()));
		}

		details.SetAgentId(agent->GetId());
	}
		
	m_session->ValidateDocDetails(details);
	
	return details;
}

Hent* InsertPurchaseOperation::CreateHent(IPurchaseHent* purchaseHent)
{
	Hent* hent = new Hent(NULL_ID);
	hent->SetName(purchaseHent->GetName());	
		
	std::wstring alias = purchaseHent->GetAlias();
	if (alias.empty())
	{
		alias = TextUtils::RemoveWhitespace(alias);
		alias = alias.substr(0, MAXALIAS);
	}
	
	hent->SetAlias(alias);

	hent->SetStreet(purchaseHent->GetStreet());
	hent->SetPOBox(purchaseHent->GetPOBox());
	hent->SetCity(purchaseHent->GetCity());
	hent->SetZip(purchaseHent->GetZip());
	hent->SetPhoneNo(purchaseHent->GetPhoneNo());
	hent->SetFiscalNo(purchaseHent->GetFiscalNo());
	hent->SetHentNo(purchaseHent->GetHentNo());
	hent->SetWetNo(purchaseHent->GetWetNo());	
	hent->SetPlateNo(purchaseHent->GetPlateNo());
	hent->SetHentType(purchaseHent->GetHentType());	
	hent->SetPESEL(purchaseHent->GetPersonalNo());
	hent->SetREGON(purchaseHent->GetStatsNo());
	hent->SetIdNo(purchaseHent->GetPersonalIdNo());
	hent->SetIssueDate(purchaseHent->GetIssueDate());
	hent->SetIssuePost(purchaseHent->GetIssuePost());
	hent->SetAccountNo(purchaseHent->GetAccountNo());
	hent->SetBankName(purchaseHent->GetBankName());
	hent->SetWetLicenceNo(purchaseHent->GetWetLicenceNo());
	hent->SetCellPhoneNo(purchaseHent->GetCellPhoneNo());
	hent->SetEmailAddress(purchaseHent->GetEmailAddress());
	hent->SetLatitude(purchaseHent->GetLatitude());
	hent->SetLongitude(purchaseHent->GetLongitude());
	return hent;
}

Hent* InsertPurchaseOperation::InsertPurchaseHent(Transaction transaction,IPurchaseHent* purchaseHent)
{
	Hent* hent = CreateHent(purchaseHent);
	Hent* insertedHent = m_session->m_DAO.InsertHent(transaction,m_session->m_pSeason->GetId(),hent);
	delete hent;
	return insertedHent;
}

InvoiceDetails InsertPurchaseOperation::ExtractInvoiceDetails(IPurchaseInvoice* purchaseInvoice)
{
	InvoiceDetails details;
	//details.SetExtras(purchaseInvoice->GetExtras());
	details.SetInvoiceDate(purchaseInvoice->GetInvoiceDate());	
	details.SetVATRate(purchaseInvoice->GetVatRate());
	details.SetPayWay(purchaseInvoice->GetPayWay());
	details.SetCustomNumber(purchaseInvoice->GetCustomNumber());
	details.SetTransactionPlace(purchaseInvoice->GetTransactionPlace());
	details.SetTransactionDate(purchaseInvoice->GetTransactionDate());
	details.SetPayDueDays(purchaseInvoice->GetPayDueDays());
	
	
	return details;
}

InvoiceHent* InsertPurchaseOperation::CreateInvoiceHent(IPurchaseHent* purchaseHent)
{
	InvoiceHent* invoiceHent = new InvoiceHent();
	Hent* hent = CreateHent(purchaseHent);
	invoiceHent->CopyFrom(hent);	
	Hent* insertedHent = m_hents.find(invoiceHent->GetHentNo());
	invoiceHent->SetParentHentId(insertedHent->GetId());
	delete hent;
	return invoiceHent;
}

InvoiceDeduction* InsertPurchaseOperation::CreateDeduction(IPurchaseInvoiceDeduction* deduction)
{
	InvoiceDeduction* result = new InvoiceDeduction();

	result->SetCode(deduction->GetCode());
	result->SetFraction(deduction->GetFraction());
	result->SetReason(deduction->GetReason());
	result->SetIsEnabled(deduction->GetIsEnabled());

	return result;
}
