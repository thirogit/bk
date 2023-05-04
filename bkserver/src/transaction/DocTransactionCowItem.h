#pragma once

#include "../snapshot/DocCowItem.h"
#include <data/runtime/IItem.h>

class DocTransactionCowItem : public IItem,public DocCowItem
{
public:
	DocTransactionCowItem(ItemId itemId,uint32_t cowId);
	virtual ItemId GetId() const;
private:
	ItemId m_ItemId;
};
