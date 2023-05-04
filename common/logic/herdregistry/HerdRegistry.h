#pragma once

#include "ICowPresence.h"
#include <arrays/Enumerator.h>
#include <arrays/NewPtrFlushArray.h>
#include <data/IInDoc.h>
#include <data/IBuyDoc.h>
#include <data/IOutDoc.h>
#include <data/ISellDoc.h>
#include <data/IMoveDoc.h>
#include <arrays/IdableIndex.h>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/mem_fun.hpp>

class HerdRegistry
{
	class CowPresence : public ICowPresence
	{
	public:
		CowPresence(ICow* pCow);
		virtual const CowPresenceKey& GetPresenceKey() const;
		virtual uint32_t			GetCowId() const;
		virtual bool				IsInStock() const;	
		virtual const CowNo&		GetCowNo() const;
		virtual const std::wstring&	GetPassNo() const;
		virtual uint32_t			GetFirstOwnerId() const;
		virtual const DateTime&		GetBrithDate() const;
		virtual const Decimal&		GetOwnWeight() const;
		virtual const CowSex&		GetSex() const;
		virtual uint32_t			GetOwnStockId() const;
		virtual uint32_t			GetOwnClassId() const;
		virtual const std::wstring&	GetExtras() const;
	
		
		virtual const IDoc*			GetHerdInDoc() const;
		virtual const DateTime&		GetHerdInDate() const;
		virtual const NullInt&		GetHerdInGrp() const;
		virtual uint32_t			GetHerdInStockId() const;
		virtual uint32_t			GetHerdInClassId() const;
		virtual const NullDecimal&	GetHerdInWeight() const;

		virtual const IDoc*			GetHerdOutDoc() const;
		virtual DateTime			GetHerdOutDate() const;
		virtual NullInt				GetHerdOutGrp() const;
		virtual uint32_t			GetHerdOutStockId() const;
		virtual uint32_t			GetHerdOutClassId() const;
		virtual NullDecimal			GetHerdOutWeight() const;
	
		virtual uint32_t			GetTermBuyClassId() const;
		virtual uint32_t			GetTermSellClassId() const;

		virtual const NullDecimal&	GetTermBuyWeight() const;
		virtual const NullDecimal&	GetTermSellWeight() const;
		virtual const NullDecimal&	GetTermBuyPrice() const;
		virtual const NullDecimal&	GetTermSellPrice() const;
	private:
		ICow* m_pCow;
		ICowEntry* m_pEnteringEntry;
		ICowEntry* m_pLeavingEntry;
		IDoc* m_pEnteringDoc;
		IDoc* m_pLeavingDoc;
		CowPresenceKey m_Key;
		friend class HerdRegistry;
	};

public:
	HerdRegistry(uint32_t herdId);	
	virtual ~HerdRegistry();
	void AddInDoc(IInDoc* pInDoc);
	void AddBuyDoc(IBuyDoc* pBuyDoc);
	void AddSellDoc(ISellDoc* pSellDoc);
	void AddOutDoc(IOutDoc* pOutDoc);
	void AddMoveDoc(IMoveDoc* pMoveDoc);

	void AddEnteringEvent(ICow* pCow,IDoc* pViaDoc,ICowEntry* pEvent);
	void RemoveEnteringEvent(ICow* pCow,IDoc* pViaDoc);

	void AddLeavingEvent(ICow* pCow,IDoc* pViaDoc,ICowEntry* pEvent);
	void RemoveLeavingEvent(ICow* pCow,IDoc* pViaDoc);
	
	PtrEnumerator<ICowPresence> EnumCows();

	PtrEnumerator<IInDoc> EnumInDocs();
	void RemoveInDoc(uint32_t docId);
	
	PtrEnumerator<IBuyDoc> EnumBuyDocs();
	void RemoveBuyDoc(uint32_t docId);

	PtrEnumerator<IOutDoc> EnumOutDocs();
	void RemoveOutDoc(uint32_t docId);

	PtrEnumerator<ISellDoc> EnumSellDocs();
	void RemoveSellDoc(uint32_t docId);

	PtrEnumerator<IMoveDoc> EnumMoveDocs();
	void RemoveMoveDoc(uint32_t docId);

	ICowPresence* GetCowPresence(const CowPresenceKey& key);
	PtrEnumerator<ICowPresence> GetCowPresences(uint32_t cowId);

	size_t GetCountOfAllCows();
	size_t GetCountOfInStockCows();

private:

	struct index_tag_id {};

	typedef boost::multi_index_container
		<
		CowPresence*,
		boost::multi_index::indexed_by<
		boost::multi_index::ordered_non_unique < boost::multi_index::tag<index_tag_id>, boost::multi_index::const_mem_fun<CowPresence, uint32_t, &CowPresence::GetCowId> >
		>
		> PresenceJournal;

	
	PresenceJournal m_journal;

	IdableIndex<IInDoc> m_indocs;
	IdableIndex<IBuyDoc> m_buydocs;
	IdableIndex<IMoveDoc> m_movedocs;
	IdableIndex<IOutDoc> m_outdocs;
	IdableIndex<ISellDoc> m_selldocs;

	size_t m_InStockCount;
	size_t m_AllCount;

	uint32_t m_herdId;
};
