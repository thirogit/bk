#include "stdafx.h"
#include "HerdRegistry.h"
#include "../runtime/LogicException.h"
#include <boost/iterator/transform_iterator.hpp>
#include <boost/function.hpp>
#include <string/TextUtils.h>

HerdRegistry::CowPresence::CowPresence(ICow* pCow) : m_Key(NULL_ID,DocType_None,NULL_ID)
{
	m_pCow = pCow;
	m_pEnteringEntry = NULL;
	m_pLeavingEntry = NULL;
	m_pEnteringDoc = NULL;
	m_pLeavingDoc = NULL;
}

const CowPresenceKey& HerdRegistry::CowPresence::GetPresenceKey() const
{
	return m_Key;
}

uint32_t HerdRegistry::CowPresence::GetCowId() const
{
	return m_pCow->GetId();
}

bool HerdRegistry::CowPresence::IsInStock() const
{
	return m_pLeavingDoc == NULL;
}
	
const CowNo& HerdRegistry::CowPresence::GetCowNo() const
{
	return m_pCow->GetCowNo();
}

const std::wstring&	HerdRegistry::CowPresence::GetPassNo() const
{
	return m_pCow->GetPassNo();
}

uint32_t HerdRegistry::CowPresence::GetFirstOwnerId() const
{
	return m_pCow->GetFirstOwnerId();
}

const DateTime&	HerdRegistry::CowPresence::GetBrithDate() const
{
	return m_pCow->GetBirthDate();
}

const Decimal& HerdRegistry::CowPresence::GetOwnWeight() const
{
	return m_pCow->GetWeight();
}

const CowSex& HerdRegistry::CowPresence::GetSex() const
{
	return m_pCow->GetSex();
}

uint32_t HerdRegistry::CowPresence::GetOwnStockId() const
{
	return m_pCow->GetStockId();
}

uint32_t HerdRegistry::CowPresence::GetOwnClassId() const
{
	return m_pCow->GetClassId();
}

const std::wstring& HerdRegistry::CowPresence::GetExtras() const
{
	return m_pCow->GetExtras();
}
	
//uint32_t HerdRegistry::CowPresence::GetBuyFromHentId() const
//{
//}
//
//uint32_t HerdRegistry::CowPresence::GetBuyStockId() const
//{
//}
//
//const NullDecimal& HerdRegistry::CowPresence::GetBuyPrice() const
//{
//}
//
//const NullDecimal& HerdRegistry::CowPresence::GetBuyWeight() const
//{
//}
//
//uint32_t HerdRegistry::CowPresence::GetBuyInvoiceId() const
//{
//}
//
const IDoc* HerdRegistry::CowPresence::GetHerdInDoc() const
{
	return m_pEnteringDoc;
}

const DateTime&	HerdRegistry::CowPresence::GetHerdInDate() const
{
	return m_pEnteringDoc->GetLoadDate();
}

const NullInt&	HerdRegistry::CowPresence::GetHerdInGrp() const
{
	return m_pEnteringEntry->GetGroup();
}

uint32_t HerdRegistry::CowPresence::GetHerdInStockId() const
{
	return m_pEnteringEntry->GetStockId();
}

uint32_t HerdRegistry::CowPresence::GetHerdInClassId() const
{
	return m_pEnteringEntry->GetClassId();
}

const NullDecimal& HerdRegistry::CowPresence::GetHerdInWeight() const
{
	return m_pEnteringEntry->GetWeight();
}
/*
uint32_t HerdRegistry::CowPresence::GetSellToHentId() const
{
}

uint32_t HerdRegistry::CowPresence::GetSellStockId() const
{
}

const NullDecimal& HerdRegistry::CowPresence::GetSellPrice() const
{
}

const NullDecimal& HerdRegistry::CowPresence::GetSellWeight() const
{
}

uint32_t HerdRegistry::CowPresence::GetSellInvoiceId() const
{
}*/

const IDoc*	HerdRegistry::CowPresence::GetHerdOutDoc() const
{
	return m_pLeavingDoc; 
}

DateTime	HerdRegistry::CowPresence::GetHerdOutDate() const
{
	if(m_pLeavingDoc)
		return m_pLeavingDoc->GetLoadDate();

	return DateTime();
}

NullInt HerdRegistry::CowPresence::GetHerdOutGrp() const
{
	if(m_pLeavingDoc)
		return m_pLeavingEntry->GetGroup();

	return NullInt();
}

uint32_t HerdRegistry::CowPresence::GetHerdOutStockId() const
{
	if(m_pLeavingDoc)
		return m_pLeavingEntry->GetStockId();

	return NULL_ID;
}

uint32_t HerdRegistry::CowPresence::GetHerdOutClassId() const
{
	if(m_pLeavingDoc)
		return m_pLeavingEntry->GetClassId();

	return NULL_ID;
}

NullDecimal HerdRegistry::CowPresence::GetHerdOutWeight() const
{
	if(m_pLeavingDoc)
		return m_pLeavingEntry->GetWeight();

	return NullDecimal();
}

	
uint32_t HerdRegistry::CowPresence::GetTermBuyClassId() const
{
	return m_pCow->GetTermBuyClassId();
}

uint32_t HerdRegistry::CowPresence::GetTermSellClassId() const
{
	return m_pCow->GetTermSellClassId();
}

const NullDecimal& HerdRegistry::CowPresence::GetTermBuyWeight() const
{
	return m_pCow->GetTermBuyWeight();
}

const NullDecimal& HerdRegistry::CowPresence::GetTermSellWeight() const
{
	return m_pCow->GetTermSellWeight();
}

const NullDecimal& HerdRegistry::CowPresence::GetTermBuyPrice() const
{
	return m_pCow->GetTermBuyPrice();
}

const NullDecimal& HerdRegistry::CowPresence::GetTermSellPrice() const
{
	return m_pCow->GetTermSellPrice();
}

//-------------------------------------------------------------------------------------------------

HerdRegistry::HerdRegistry(uint32_t herdId) : m_herdId(herdId)
{
	m_InStockCount = 0;
	m_AllCount = 0;
}

HerdRegistry::~HerdRegistry()
{
	std::for_each(m_journal.begin(),m_journal.end(),[](PresenceJournal::value_type journalEntry)
													{
														delete journalEntry;
													});
}

void HerdRegistry::AddInDoc(IInDoc* pInDoc)
{
	m_indocs.add(pInDoc);
}

void HerdRegistry::RemoveInDoc(uint32_t docId)
{
	m_indocs.remove(docId);
}

void HerdRegistry::AddBuyDoc(IBuyDoc* pBuyDoc)
{
	m_buydocs.add(pBuyDoc);
}

void HerdRegistry::AddSellDoc(ISellDoc* pSellDoc)
{
	m_selldocs.add(pSellDoc);
}

void HerdRegistry::AddOutDoc(IOutDoc* pOutDoc)
{
	m_outdocs.add(pOutDoc);
}

void HerdRegistry::AddMoveDoc(IMoveDoc* pMoveDoc)
{
	m_movedocs.add(pMoveDoc);
}


void HerdRegistry::AddLeavingEvent(ICow* pCow,IDoc* pViaDoc,ICowEntry* pEvent)
{
	if(m_journal.find(pCow->GetId()) != m_journal.end())
	{
		auto presenceTrail = m_journal.equal_range(pCow->GetId());
		PresenceJournal::iterator presenceIt = presenceTrail.second;
		presenceIt--;
		
		CowPresence* pLastPresence = *presenceIt;
		if(!pLastPresence->IsInStock())
		{
			BOOST_THROW_EXCEPTION(LogicException(TextUtils::FormatA(L"add-: cow(id=%d) already left herd(id=%d) via doc %c%d",
				pCow->GetId(),pLastPresence->m_pLeavingDoc->GetDocType(),pLastPresence->m_pLeavingDoc->GetId())));
		}
		else
		{
			pLastPresence->m_pLeavingDoc = pViaDoc;
			pLastPresence->m_pLeavingEntry = pEvent;

			m_InStockCount--;			
		}
	}
	else
	{
		BOOST_THROW_EXCEPTION(LogicException(TextUtils::FormatA(L"add-: no cow(id=%d) to record LEAVE event from herd(id=%d)",pCow->GetId(),m_herdId)));
	}
}

void HerdRegistry::RemoveLeavingEvent(ICow* pCow,IDoc* pViaDoc)
{
	if(m_journal.find(pCow->GetId()) != m_journal.end())
	{
		auto presenceTrail = m_journal.equal_range(pCow->GetId());
		PresenceJournal::iterator presenceIt = presenceTrail.second;
		presenceIt--;
		
		CowPresence* pLastPresence = *presenceIt;
		IDoc* pLeavingDoc = pLastPresence->m_pLeavingDoc;

		if(pLeavingDoc->GetDocType() == pViaDoc->GetDocType() && 
			pLeavingDoc->GetId() == pViaDoc->GetId())
		{
			pLastPresence->m_pLeavingDoc = NULL;
			pLastPresence->m_pLeavingEntry = NULL;
			m_InStockCount++;			
		}
		else
		{
			BOOST_THROW_EXCEPTION(LogicException(TextUtils::FormatA(L"remove-: cow(id=%d) didnt left herd(id=%d) via doc %c%d",
				pCow->GetId(),m_herdId,pViaDoc->GetDocType(),pViaDoc->GetId())));
		}		
	}
	else
	{
		BOOST_THROW_EXCEPTION(LogicException(TextUtils::FormatA(L"remove-: no cow(id=%d) to remove LEAVE event from herd(id=%d)",pCow->GetId(),m_herdId)));
	}
}

void HerdRegistry::AddEnteringEvent(ICow* pCow,IDoc* pViaDoc,ICowEntry* pEvent)
{
	if(m_journal.find(pCow->GetId()) != m_journal.end())
	{
		auto presenceTrail = m_journal.equal_range(pCow->GetId());
		PresenceJournal::iterator presenceIt = presenceTrail.second;
		presenceIt--;
		
		CowPresence* pLastPresence = *presenceIt;
		if(pLastPresence->IsInStock())
		{
			BOOST_THROW_EXCEPTION(LogicException(TextUtils::FormatA(L"add+: cow(id=%d) is still present in herd(id=%d), entered via doc %c%d",
				pCow->GetId(),m_herdId,pLastPresence->m_pEnteringDoc->GetDocType(),pLastPresence->m_pEnteringDoc->GetId())));
		}
	}
	
	CowPresence* pPresence = new CowPresence(pCow);	
	pPresence->m_pEnteringDoc = pViaDoc;
	pPresence->m_pEnteringEntry = pEvent;
	pPresence->m_Key = CowPresenceKey(pCow->GetId(),pViaDoc->GetDocType(),pViaDoc->GetId());
	m_journal.insert(pPresence);

	m_InStockCount++;
	m_AllCount++;
}

void HerdRegistry::RemoveEnteringEvent(ICow* pCow,IDoc* pViaDoc)
{
	if(m_journal.find(pCow->GetId()) != m_journal.end())
	{
		auto presenceTrail = m_journal.equal_range(pCow->GetId());

		PresenceJournal::iterator presenceIt = std::find_if(presenceTrail.first,presenceTrail.second,
																				[pViaDoc](CowPresence* presence) -> bool
																				{
																					const IDoc* pEnteringDoc = presence->GetHerdInDoc();
																					return (pViaDoc->GetId() == pEnteringDoc->GetId()) && (pViaDoc->GetDocType() == pEnteringDoc->GetDocType());
																				});

		if(presenceIt != presenceTrail.second)
		{
			CowPresence* pPresence = *presenceIt;
			const IDoc* pExitDoc = pPresence->GetHerdOutDoc();
			if(!pPresence->IsInStock())
				BOOST_THROW_EXCEPTION(LogicException(TextUtils::FormatA(L"rm+: cow(id=%d) was moved from herd(id=%d) via doc %c%d", pCow->GetId(),m_herdId,pExitDoc->GetDocType(),pExitDoc->GetId())));

			delete pPresence;
			m_journal.erase(presenceIt);

			m_InStockCount--;
			m_AllCount--;

			return;
		}
		else
		{
			BOOST_THROW_EXCEPTION(LogicException(TextUtils::FormatA(L"rm+: cow(id=%d) was present in herd(id=%d), but did not entered it through doc %c%d", pCow->GetId(),m_herdId,pViaDoc->GetDocType(),pViaDoc->GetId())));
		}
	}
	BOOST_THROW_EXCEPTION(LogicException(TextUtils::FormatA(L"rm+: cow(id=%d) was never present in herd(id=%d)", pCow->GetId(),m_herdId)));
}

PtrEnumerator<ICowPresence> HerdRegistry::EnumCows()
{	
	return PtrEnumerator<ICowPresence>(m_journal);
}

PtrEnumerator<IInDoc> HerdRegistry::EnumInDocs()
{
	return PtrEnumerator<IInDoc>(m_indocs);
}

PtrEnumerator<IBuyDoc> HerdRegistry::EnumBuyDocs()
{
	return PtrEnumerator<IBuyDoc>(m_buydocs);
}

PtrEnumerator<IOutDoc> HerdRegistry::EnumOutDocs()
{
	return PtrEnumerator<IOutDoc>(m_outdocs);
}

PtrEnumerator<ISellDoc> HerdRegistry::EnumSellDocs()
{
	return PtrEnumerator<ISellDoc>(m_selldocs);
}

PtrEnumerator<IMoveDoc> HerdRegistry::EnumMoveDocs()
{
	return PtrEnumerator<IMoveDoc>(m_movedocs);
}

ICowPresence* HerdRegistry::GetCowPresence(const CowPresenceKey& key)
{
	auto presenceRange = m_journal.equal_range(key.GetCowId());
	auto presenceIt = presenceRange.first;
	CowPresence* presence = NULL;
	while(presenceIt != presenceRange.second)
	{
		presence = *presenceIt;
		if(presence->GetPresenceKey() == key)
			return presence;
		presenceIt++;
	}
	return NULL;
}

PtrEnumerator<ICowPresence> HerdRegistry::GetCowPresences(uint32_t cowId)
{
	auto presenceRange = m_journal.equal_range(cowId);
	return PtrEnumerator<ICowPresence>(presenceRange.first, presenceRange.second);
}


void HerdRegistry::RemoveSellDoc(uint32_t docId)
{
	m_selldocs.remove(docId);
}

void HerdRegistry::RemoveMoveDoc(uint32_t docId)
{
	m_movedocs.remove(docId);
}

void HerdRegistry::RemoveBuyDoc(uint32_t docId)
{
	m_buydocs.remove(docId);
}

void HerdRegistry::RemoveOutDoc(uint32_t docId)
{
	m_outdocs.remove(docId);
}

size_t HerdRegistry::GetCountOfAllCows()
{
	return m_AllCount;
}

size_t HerdRegistry::GetCountOfInStockCows()
{
	return m_InStockCount;
}