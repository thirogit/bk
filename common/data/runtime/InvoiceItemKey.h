#pragma once

#include "ItemId.h"
#include "../DocKey.h"

class InvoiceItemKey
{
public:
	InvoiceItemKey();
	InvoiceItemKey(const DocKey& key,const ItemId& item);
	InvoiceItemKey(const InvoiceItemKey& src);
	InvoiceItemKey& operator=(const InvoiceItemKey& right);
	
	const ItemId& GetItemId() const;
	const DocKey& GetDocKey() const;
	
private:
	DocKey m_dockey;
	ItemId m_itemid;
	
	friend bool operator>(const InvoiceItemKey& left,const InvoiceItemKey& right);
	friend bool operator<(const InvoiceItemKey& left,const InvoiceItemKey& right);
};
