#ifndef __DOCITEM_H__
#define __DOCITEM_H__

#include <data\runtime\ItemId.h>
#include <data\impl\CowEntryDetails.h>
#include <data\impl\CowDetails.h>

class DocItem
{
public:
	DocItem(const ItemId& itemId);
	
	ItemId GetItemId() const;	
	uint32_t GetCowId() const;
	const CowDetails& GetCowDetails() const;
	const CowEntryDetails& GetCowEntry() const;
		
	void SetCowDetails(const CowDetails& cow);
	void SetCowEntry(const CowEntryDetails& entry);	
	void SetCowId(uint32_t cowId);

private:
	ItemId m_itemId;
	uint32_t m_cowId;
	CowDetails m_cowDetails;
	CowEntryDetails m_cowEntry;
};

#endif