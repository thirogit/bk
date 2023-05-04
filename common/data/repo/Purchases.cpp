#include "stdafx.h"
#include "Purchases.h"
#include "../../arrays/DeepDelete.h"
#include "../../CommonTypes.h"
#include <boost/foreach.hpp>

Purchases::Purchases()
{
}

Purchases::Purchases(Purchases&& purchases)
{
	std::copy(purchases.m_purchases.begin(), purchases.m_purchases.end(), std::back_inserter(m_purchases));
	purchases.m_purchases.clear();
}

Purchases::Purchases(const Purchases& purchases)
{
	BOOST_FOREACH(Purchase* purchase, purchases)
	{
		Add(purchase);
	}
}

Purchases::~Purchases()
{
	DeepDelete(m_purchases);
	m_purchases.clear();
}

void Purchases::Add(const IPurchase* purchase)
{
	m_purchases.push_back(new Purchase(purchase));
}

Purchase* Purchases::Add(const std::wstring& id)
{
	Purchase* purchase = new Purchase(id);
	m_purchases.push_back(purchase);
	return purchase;
}

PtrEnumerator<IPurchase> Purchases::Enum()
{
	return PtrEnumerator<IPurchase>(m_purchases);
}

Purchases::const_iterator Purchases::begin() const
{
	return m_purchases.begin();
}

Purchases::const_iterator Purchases::end() const
{
	return m_purchases.end();
}

Purchases::iterator Purchases::begin()
{
	return m_purchases.begin();
}

Purchases::iterator Purchases::end()
{
	return m_purchases.end();
}

count_t Purchases::Count() const
{
	return m_purchases.size();
}