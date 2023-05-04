#include "stdafx.h"
#include "InvoiceItemKey.h"

InvoiceItemKey::InvoiceItemKey()
{
}

InvoiceItemKey::InvoiceItemKey(const DocKey& key,const ItemId& item) : m_itemid(item),m_dockey(key)
{
}

InvoiceItemKey::InvoiceItemKey(const InvoiceItemKey& src)
{
	operator=(src);	
}

InvoiceItemKey& InvoiceItemKey::operator=(const InvoiceItemKey& right)
{
	 m_itemid = right.m_itemid;
	 m_dockey = right.m_dockey;
	 return *this;
}
	
const ItemId& InvoiceItemKey::GetItemId() const
{
	return m_itemid;
}

const DocKey& InvoiceItemKey::GetDocKey() const
{
	return m_dockey;
}
	

bool operator>(const InvoiceItemKey& left,const InvoiceItemKey& right)
{
	if(right.m_dockey == left.m_dockey)
		return right.m_itemid > left.m_itemid;

	return right.m_dockey > left.m_dockey;
}

bool operator<(const InvoiceItemKey& left,const InvoiceItemKey& right)
{
	return right > left;
}