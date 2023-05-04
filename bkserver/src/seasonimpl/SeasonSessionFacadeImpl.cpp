#include "stdafx.h"
#include "SeasonSessionFacadeImpl.h"
#include "SeasonSession.h"
#include "SessionException.h"
#include <string/TextUtils.h>
#include <boost/foreach.hpp>

SeasonSessionFacadeImpl::SeasonSessionFacadeImpl(ClientSeasonSession* pClient,SeasonSession* pDelegate) : m_pClient(pClient),m_pDelegate(pDelegate)
{
}


void SeasonSessionFacadeImpl::LockHent(uint32_t hentId)
{
	m_pDelegate->LockHent(m_pClient, hentId);
}

void SeasonSessionFacadeImpl::UnlockHent(uint32_t hentId)
{
	m_pDelegate->UnlockHent(m_pClient, hentId);
}


uint32_t SeasonSessionFacadeImpl::InsertHent(IHent* hent)
{
	return m_pDelegate->InsertHent(m_pClient, hent);
}

void SeasonSessionFacadeImpl::UpdateHent(IHent* hent)
{
	m_pDelegate->UpdateHent(m_pClient, hent);
}

void SeasonSessionFacadeImpl::DeleteHent(uint32_t hentId)
{
	m_pDelegate->DeleteHent(m_pClient, hentId);
}

IHent* SeasonSessionFacadeImpl::FindHent(uint32_t hentId)
{
	HentServerObject* pHentSO = m_pDelegate->AcquireHent(hentId);
	return pHentSO->Object();
}

void SeasonSessionFacadeImpl::LockCow(uint32_t cowId)
{
	m_pDelegate->LockCow(m_pClient,cowId);
}

void SeasonSessionFacadeImpl::UnlockCow(uint32_t cowId)
{
	m_pDelegate->UnlockCow(m_pClient,cowId);
}

void SeasonSessionFacadeImpl::UpdateCow(uint32_t cowId,const CowDetails& details)
{
	m_pDelegate->UpdateCow(m_pClient,cowId,details);
}

bool SeasonSessionFacadeImpl::IsInDocLocked(uint32_t docId)
{
	InDocServerObject* pInDocSO = m_pDelegate->AcquireInDoc(docId);
	return pInDocSO->OwnsLock(m_pClient);
}

void SeasonSessionFacadeImpl::LockInDocWithCows(uint32_t docId)
{
	return m_pDelegate->LockInDocWithCows(m_pClient, docId);
}

void SeasonSessionFacadeImpl::LockInDoc(uint32_t docId)
{
	return m_pDelegate->LockInDoc(m_pClient,docId);
}

void SeasonSessionFacadeImpl::UnlockInDoc(uint32_t docId)
{
	m_pDelegate->UnlockInDoc(m_pClient,docId);
}

void SeasonSessionFacadeImpl::UpdateInDoc(const OwnDocSnapshot& shapshot)
{
	m_pDelegate->UpdateInDoc(m_pClient,shapshot);
}

void SeasonSessionFacadeImpl::InsertInDoc(const OwnDocSnapshot& shapshot)
{
	m_pDelegate->InsertInDoc(m_pClient,shapshot);
}

void SeasonSessionFacadeImpl::DeleteInDoc(uint32_t docId)
{
	m_pDelegate->DeleteInDoc(m_pClient,docId);
}

bool SeasonSessionFacadeImpl::IsBuyDocLocked(uint32_t docId)
{
	BuyDocServerObject* pDocSO = m_pDelegate->AcquireBuyDoc(docId);
	return pDocSO->OwnsLock(m_pClient);
}

void SeasonSessionFacadeImpl::LockBuyDoc(uint32_t docId)
{
	m_pDelegate->LockBuyDoc(m_pClient,docId);
}

void SeasonSessionFacadeImpl::UnlockBuyDoc(uint32_t docId)
{
	m_pDelegate->UnlockBuyDoc(m_pClient,docId);
}

void SeasonSessionFacadeImpl::UpdateBuyDoc(const HentsDocSnapshot& shapshot)
{
	m_pDelegate->UpdateBuyDoc(m_pClient,shapshot);
}

void SeasonSessionFacadeImpl::InsertBuyDoc(const HentsDocSnapshot& shapshot)
{
	m_pDelegate->InsertBuyDoc(m_pClient,shapshot);
}

void SeasonSessionFacadeImpl::DeleteBuyDoc(uint32_t docId)
{
	m_pDelegate->DeleteBuyDoc(m_pClient,docId);
}

bool SeasonSessionFacadeImpl::IsMoveDocLocked(uint32_t docId)
{
	MoveDocServerObject* pDocSO = m_pDelegate->AcquireMoveDoc(docId);
	return pDocSO->OwnsLock(m_pClient);
}

void SeasonSessionFacadeImpl::LockMoveDoc(uint32_t docId)
{
	m_pDelegate->LockMoveDoc(m_pClient,docId);
}

void SeasonSessionFacadeImpl::UnlockMoveDoc(uint32_t docId)
{
	m_pDelegate->UnlockMoveDoc(m_pClient,docId);
}

void SeasonSessionFacadeImpl::UpdateMoveDoc(const MoveDocSnapshot& shapshot)
{
	m_pDelegate->UpdateMoveDoc(m_pClient,shapshot);
}

void SeasonSessionFacadeImpl::InsertMoveDoc(const MoveDocSnapshot& shapshot)
{
	m_pDelegate->InsertMoveDoc(m_pClient,shapshot);
}

void SeasonSessionFacadeImpl::DeleteMoveDoc(uint32_t docId)
{
	m_pDelegate->DeleteMoveDoc(m_pClient,docId);
}

IAgent* SeasonSessionFacadeImpl::FindAgent(uint32_t agentId)
{
	return m_pDelegate->FindAgent(agentId);
}

ISeason* SeasonSessionFacadeImpl::GetSeason()
{
	return m_pDelegate->GetSeason();
}

ICow* SeasonSessionFacadeImpl::FindCow(uint32_t cowId)
{
	CowServerObject* pCowSO = m_pDelegate->FindCow(cowId);
	if(pCowSO != NULL)
		return pCowSO->Object();
	return NULL;
}

bool SeasonSessionFacadeImpl::IsSellDocLocked(uint32_t docId)
{
	SellDocServerObject* pDocSO = m_pDelegate->AcquireSellDoc(docId);
	return pDocSO->OwnsLock(m_pClient);
}

void SeasonSessionFacadeImpl::LockSellDoc(uint32_t docId)
{
	m_pDelegate->LockSellDoc(m_pClient,docId);
}

void SeasonSessionFacadeImpl::UnlockSellDoc(uint32_t docId)
{
	m_pDelegate->UnlockSellDoc(m_pClient,docId);
}

void SeasonSessionFacadeImpl::UpdateSellDoc(const HentsDocSnapshot& shapshot)
{
	m_pDelegate->UpdateSellDoc(m_pClient,shapshot);
}

void SeasonSessionFacadeImpl::InsertSellDoc(const HentsDocSnapshot& snapshot)
{
	m_pDelegate->InsertSellDoc(m_pClient,snapshot);
}

void SeasonSessionFacadeImpl::DeleteSellDoc(uint32_t docId)
{
	m_pDelegate->DeleteSellDoc(m_pClient,docId);
}

bool SeasonSessionFacadeImpl::IsOutDocLocked(uint32_t docId)
{
	OutDocServerObject* pDocSO = m_pDelegate->AcquireOutDoc(docId);
	return pDocSO->OwnsLock(m_pClient);
}

void SeasonSessionFacadeImpl::LockOutDoc(uint32_t docId)
{
	m_pDelegate->LockOutDoc(m_pClient,docId);	
}

void SeasonSessionFacadeImpl::UnlockOutDoc(uint32_t docId)
{
	m_pDelegate->UnlockOutDoc(m_pClient,docId);
}

void SeasonSessionFacadeImpl::UpdateOutDoc(const OwnDocSnapshot& snapshot)
{
	m_pDelegate->UpdateOutDoc(m_pClient,snapshot);
}

void SeasonSessionFacadeImpl::InsertOutDoc(const OwnDocSnapshot& shapshot)
{
	m_pDelegate->InsertOutDoc(m_pClient,shapshot);
}

void SeasonSessionFacadeImpl::DeleteOutDoc(uint32_t docId)
{
	m_pDelegate->DeleteOutDoc(m_pClient,docId);
}

IInDoc* SeasonSessionFacadeImpl::FindInDoc(uint32_t docId)
{
	return SafeFind(docId,&SeasonSession::FindInDoc);
}

IBuyDoc* SeasonSessionFacadeImpl::FindBuyDoc(uint32_t docId)
{
	return SafeFind(docId, &SeasonSession::FindBuyDoc);
}

IMoveDoc* SeasonSessionFacadeImpl::FindMoveDoc(uint32_t docId)
{
	return SafeFind(docId, &SeasonSession::FindMoveDoc);
}

ISellDoc* SeasonSessionFacadeImpl::FindSellDoc(uint32_t docId)
{
	return SafeFind(docId, &SeasonSession::FindSellDoc);
}

IOutDoc* SeasonSessionFacadeImpl::FindOutDoc(uint32_t docId)
{
	return SafeFind(docId, &SeasonSession::FindOutDoc);
}

ICowTrail* SeasonSessionFacadeImpl::GetCowTrail(uint32_t cowId)
{
	return m_pDelegate->m_Tracker.GetTrail(cowId);
}

ICowPresence*  SeasonSessionFacadeImpl::GetCowPresence(uint32_t herdId,const CowPresenceKey& key)
{
	HerdRegistry* pRegistry = m_pDelegate->m_Registers.at(herdId);
	return pRegistry->GetCowPresence(key);
}

CowPresenceKeys SeasonSessionFacadeImpl::GetCowPresences(uint32_t herdId, const CowNo& cowNo)
{
	HerdRegistry* pHerdRegistry = m_pDelegate->m_Registers.at(herdId);
	CowIds cowIds = m_pDelegate->GetCowOccurences(cowNo);
	CowPresenceKeys result;

	BOOST_FOREACH(uint32_t cowId, cowIds)
	{
		PtrEnumerator<ICowPresence> presecnceEnum = pHerdRegistry->GetCowPresences(cowId);
		while (presecnceEnum.hasNext())
		{
			result.add(presecnceEnum->GetPresenceKey());
			presecnceEnum.advance();
		}
	}
	return std::move(result);
}

void SeasonSessionFacadeImpl::InsertBuyDocsInvoice(const InvoiceSnapshot& snapshot)
{
	m_pDelegate->InsertBuyDocsInvoice(m_pClient,snapshot);
}

void SeasonSessionFacadeImpl::UpdateBuyDocsInvoice(const InvoiceSnapshot& snapshot)
{
	m_pDelegate->UpdateBuyDocsInvoice(m_pClient,snapshot);
}

void SeasonSessionFacadeImpl::DeleteBuyInvoice(uint32_t invoiceId)
{
	m_pDelegate->DeleteBuyInvoice(m_pClient,invoiceId);
}

void SeasonSessionFacadeImpl::InsertBuyCowsInvoice(const InvoiceSnapshot& snapshot)
{
	m_pDelegate->InsertBuyCowsInvoice(m_pClient,snapshot);
}

void SeasonSessionFacadeImpl::UpdateBuyCowsInvoice(const InvoiceSnapshot& snapshot)
{
	m_pDelegate->UpdateBuyCowsInvoice(m_pClient,snapshot);
}

IBuyInvoice* SeasonSessionFacadeImpl::FindBuyInvoice(uint32_t invoiceId)
{
	return SafeFind(invoiceId, &SeasonSession::FindBuyInvoice);
}

bool SeasonSessionFacadeImpl::IsBuyInvoiceLocked(uint32_t invoiceId)
{
	BuyInvoiceServerObject* pInvoiceSO = m_pDelegate->AcquireBuyInvoice(invoiceId);
	return pInvoiceSO->OwnsLock(m_pClient);
}

void SeasonSessionFacadeImpl::LockBuyInvoice(uint32_t invoiceId)
{
	m_pDelegate->LockBuyInvoice(m_pClient,invoiceId);
}

void SeasonSessionFacadeImpl::UnlockBuyInvoice(uint32_t invoiceId)
{
	m_pDelegate->UnlockBuyInvoice(m_pClient,invoiceId);
}

ISellInvoice* SeasonSessionFacadeImpl::FindSellInvoice(uint32_t invoiceId)
{
	return SafeFind(invoiceId, &SeasonSession::FindSellInvoice);
}

bool SeasonSessionFacadeImpl::IsSellInvoiceLocked(uint32_t invoiceId)
{
	SellInvoiceServerObject* pInvoiceSO = m_pDelegate->AcquireSellInvoice(invoiceId);
	return pInvoiceSO->OwnsLock(m_pClient);
}

void SeasonSessionFacadeImpl::LockSellInvoice(uint32_t invoiceId)
{
	m_pDelegate->LockSellInvoice(m_pClient,invoiceId);
}

void SeasonSessionFacadeImpl::UnlockSellInvoice(uint32_t invoiceId)
{
	m_pDelegate->UnlockSellInvoice(m_pClient,invoiceId);
}

void SeasonSessionFacadeImpl::InsertSellDocsInvoice(const InvoiceSnapshot& snapshot)
{
	m_pDelegate->InsertSellDocsInvoice(m_pClient,snapshot);
}

void SeasonSessionFacadeImpl::UpdateSellDocsInvoice(const InvoiceSnapshot& snapshot)
{
	m_pDelegate->UpdateSellDocsInvoice(m_pClient,snapshot);
}

void SeasonSessionFacadeImpl::InsertSellCowsInvoice(const InvoiceSnapshot& snapshot)
{
	m_pDelegate->InsertSellCowsInvoice(m_pClient,snapshot);
}

void SeasonSessionFacadeImpl::UpdateSellCowsInvoice(const InvoiceSnapshot& snapshot)
{
	m_pDelegate->UpdateSellCowsInvoice(m_pClient,snapshot);
}

void SeasonSessionFacadeImpl::DeleteSellInvoice(uint32_t invoiceId)
{
	m_pDelegate->DeleteSellInvoice(m_pClient,invoiceId);
}

IClass* SeasonSessionFacadeImpl::FindClass(const std::wstring& classCd)
{
	return m_pDelegate->FindClass(classCd);
}

IStock* SeasonSessionFacadeImpl::FindStock(const std::wstring& stockCd)
{
	return m_pDelegate->FindStock(stockCd);
}

IHerd* SeasonSessionFacadeImpl::FindHerdByIndex(int herdIndex)
{
	return m_pDelegate->FindHerdByIndex(herdIndex);
}

IHerd* SeasonSessionFacadeImpl::FindHerdById(uint32_t herdId)
{
	return m_pDelegate->FindHerdById(herdId);
}

IAgent* SeasonSessionFacadeImpl::FindAgent(const std::wstring&  agentCd)
{
	return m_pDelegate->FindAgent(agentCd);
}

IHent* SeasonSessionFacadeImpl::FindHent(const HentNo& hentNo)
{
	HentServerObject* hentSO = m_pDelegate->FindHent(hentNo);
	if (hentSO != NULL)
		return hentSO->Object();

	return NULL;
}


void SeasonSessionFacadeImpl::InsertPurchase(IPurchase* purchase, const DateTime& docDt)
{
	m_pDelegate->InsertPurchase(purchase, docDt);
}


bool SeasonSessionFacadeImpl::IsCowInStock(uint32_t herdId, const CowNo& cowNo)
{
	CowPresenceKeys keys(std::move(GetCowPresences(herdId, cowNo)));
	BOOST_FOREACH(CowPresenceKey key, keys)
	{
		ICowPresence* presence = GetCowPresence(herdId, key);
		if (presence->IsInStock())
		{
			return true;
		}
	}
	return false;
}