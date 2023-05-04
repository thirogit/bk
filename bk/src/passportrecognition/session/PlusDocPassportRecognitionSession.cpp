#include "stdafx.h"
#include "PlusDocPassportRecognitionSession.h"
#include <arrays\DeepDelete.h>
#include <boost/foreach.hpp>

#define PASSPORT_RESOLUTION_DPI_FOR_STORING 100

PlusDocPassportRecognitionSession::RecognitionItem::RecognitionItem(ItemId id, PlusDocPassportRecognitionSession* owner) : m_id(id), m_owner(owner)
{
}

ItemId PlusDocPassportRecognitionSession::RecognitionItem::GetId() const
{
	return m_id;
}

CowDetails PlusDocPassportRecognitionSession::RecognitionItem::GetCow() const
{
	return m_owner->m_transaction->GetItem(m_id)->GetCowDetails();
}

bool PlusDocPassportRecognitionSession::RecognitionItem::HasImage() const
{
	return m_owner->m_cache->IsCached(m_id);
}

fipWinImageEx PlusDocPassportRecognitionSession::RecognitionItem::GetImage() const
{
	return std::move(m_owner->m_cache->GetPassportImage(m_id));
}

//-------------------------------------------------------------------------------------------------

PlusDocPassportRecognitionSession::PlusDocPassportRecognitionSession(PassportImageDiskCache* cache, PlusDocTransactionFacade* transaction) : m_transaction(transaction), m_cache(cache)
{	
	for (count_t item = 0, count = transaction->GetItemCount(); item < count; item++)
	{
		m_items.push_back(new RecognitionItem(transaction->GetItemAt(item)->GetItemId(), this));
	}
}

PlusDocPassportRecognitionSession::~PlusDocPassportRecognitionSession()
{
	DeepDelete(m_items);
}

fipWinImageEx PlusDocPassportRecognitionSession::GetImageForStorage(PassportImagePtr image)
{	
	int saveResolution = PASSPORT_RESOLUTION_DPI_FOR_STORING;
	int curResolution = image->GetResolution().ToInt();

	fipWinImageEx imageToStore;
	if (image->GetPreparedPassportImage().isValid())
	{
		imageToStore = image->GetPreparedPassportImage();
	}
	else
	{
		imageToStore = image->GetRawPassportImage();
	}

	int newWidth = (imageToStore.getWidth()*saveResolution) / curResolution;
	int newHeight = (imageToStore.getHeight()*saveResolution) / curResolution;	
	imageToStore.rescale(newWidth, newHeight, FILTER_BOX);

	return std::move(imageToStore);
}

ItemId PlusDocPassportRecognitionSession::AddCow(const CowDetails& details, PassportImagePtr image)
{
	ItemId itemid = m_transaction->PlusCow(details);
	m_cache->CachePassportImage(itemid, GetImageForStorage(image));
	return itemid;
}

void PlusDocPassportRecognitionSession::UpdateCow(ItemId id, const CowDetails& details, PassportImagePtr image)
{
	m_transaction->UpdateCow(id, details);
	m_cache->CachePassportImage(id, GetImageForStorage(image));
}

void PlusDocPassportRecognitionSession::UpdateCow(ItemId id, const CowDetails& details)
{
	m_transaction->UpdateCow(id, details);
}

IRecognitionItem* PlusDocPassportRecognitionSession::FindItem(const CowNo& cowNo)
{
	const DocItem* transactionItem;
	BOOST_FOREACH(RecognitionItem* item, m_items)
	{
		transactionItem = m_transaction->GetItem(item->GetId());
		if (transactionItem->GetCowDetails().GetCowNo() == cowNo)
		{
			return item;
		}		
	}
	return NULL;
}

IRecognitionItem* PlusDocPassportRecognitionSession::GetItem(ItemId id) const
{
	const boost::multi_index::index<ItemIndex, index_tag_itemId>::type& indexItemId = boost::multi_index::get<index_tag_itemId>(m_items);

	auto itemIt = indexItemId.find(id);
	if (itemIt != indexItemId.end())
		return *itemIt;

	return NULL;
}

IRecognitionItem* PlusDocPassportRecognitionSession::GetItemAt(int index) const
{
	return *(m_items.begin() + index);
}

IRecognitionItem* PlusDocPassportRecognitionSession::GetItem(CowNo& cowno) const
{
	BOOST_FOREACH(RecognitionItem* item, m_items)
	{
		if (cowno == item->GetCow().GetCowNo())
		{
			return item;
		}
	}

	return NULL;
}

count_t PlusDocPassportRecognitionSession::GetItemCount() const
{
	return m_items.size();
}

