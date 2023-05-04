#include "stdafx.h"
#include "PurchaseHeaders.h"
#include "../../arrays/DeepDelete.h"
#include "../../CommonTypes.h"
#include <boost/foreach.hpp>

PurchaseHeaders::PurchaseHeaders()
{
}

PurchaseHeaders::PurchaseHeaders(PurchaseHeaders&& purchaseheaders)
{
	std::copy(purchaseheaders.m_headers.begin(), purchaseheaders.m_headers.end(), std::back_inserter(m_headers));
	purchaseheaders.m_headers.clear();
}

PurchaseHeaders::PurchaseHeaders(const PurchaseHeaders& purchaseheaders)
{
	BOOST_FOREACH(PurchaseHeader* header, purchaseheaders)
	{
		Add(header->GetId())->CopyFrom(*header);
	}
}

PurchaseHeaders::~PurchaseHeaders()
{
	DeepDelete(m_headers);
	m_headers.clear();
}


PurchaseHeader* PurchaseHeaders::Add(const std::wstring& id)
{
	PurchaseHeader* purchase = new PurchaseHeader(id);
	m_headers.push_back(purchase);
	return purchase;
}

void PurchaseHeaders::Add(const IPurchaseHeader* purchaseHeader)
{
	m_headers.push_back(new PurchaseHeader(purchaseHeader));
}

PtrEnumerator<IPurchaseHeader> PurchaseHeaders::Enum()
{
	return PtrEnumerator<IPurchaseHeader>(m_headers);
}

PurchaseHeaders::const_iterator PurchaseHeaders::begin() const
{
	return m_headers.begin();
}

PurchaseHeaders::const_iterator PurchaseHeaders::end() const
{
	return m_headers.end();
}

PurchaseHeaders::iterator PurchaseHeaders::begin()
{
	return m_headers.begin();
}

PurchaseHeaders::iterator PurchaseHeaders::end()
{
	return m_headers.end();
}

count_t PurchaseHeaders::Count() const
{
	return m_headers.size();
}