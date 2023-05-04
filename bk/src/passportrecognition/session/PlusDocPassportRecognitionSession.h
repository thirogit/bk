#ifndef __PLUSDOCPASSPORTRECOGNITIONSESSION_H__
#define __PLUSDOCPASSPORTRECOGNITIONSESSION_H__

#include "PassportRecognitionSession.h"
#include "../../context/transaction/PlusDocTransactionFacade.h"
#include "../cache/PassportImageDiskCache.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/random_access_index.hpp>

class PlusDocPassportRecognitionSession : public PassportRecognitionSession
{
	struct index_tag_itemId{};
	//struct index_tag_cowNo{};

	class RecognitionItem : public IRecognitionItem
	{
	public:

		RecognitionItem(ItemId id, PlusDocPassportRecognitionSession* owner);
		
		virtual ItemId GetId() const;
		virtual CowDetails GetCow() const;
		virtual bool HasImage() const;
		virtual fipWinImageEx GetImage() const;

	private:
	
		ItemId m_id;
		PlusDocPassportRecognitionSession* m_owner;
	};

	typedef boost::multi_index_container
	<
	RecognitionItem*,
	boost::multi_index::indexed_by<
	boost::multi_index::random_access<>,
	boost::multi_index::ordered_unique < boost::multi_index::tag<index_tag_itemId>, boost::multi_index::const_mem_fun<RecognitionItem, ItemId, &RecognitionItem::GetId> >//,
	//boost::multi_index::ordered_unique < boost::multi_index::tag<index_tag_cowNo>, boost::multi_index::const_mem_fun<RecognitionItem, CowNo, &RecognitionItem::GetCowNo> >
	>
	> ItemIndex;


public:
	PlusDocPassportRecognitionSession(PassportImageDiskCache* cache,PlusDocTransactionFacade* transaction);
	~PlusDocPassportRecognitionSession();

	virtual ItemId AddCow(const CowDetails& details, PassportImagePtr image);

	virtual void UpdateCow(ItemId id, const CowDetails& details, PassportImagePtr image);
	virtual void UpdateCow(ItemId id, const CowDetails& details);
	virtual IRecognitionItem* FindItem(const CowNo& cowNo);
	
	virtual IRecognitionItem* GetItem(ItemId id) const;
	virtual IRecognitionItem* GetItem(CowNo& cowno) const;
	virtual IRecognitionItem* GetItemAt(int index) const;
	virtual count_t GetItemCount() const;
private:
	fipWinImageEx GetImageForStorage(PassportImagePtr image);
private:
	PlusDocTransactionFacade* m_transaction;
	PassportImageDiskCache* m_cache;
	ItemIndex m_items;

	friend class RecognitionItem;
};

#endif