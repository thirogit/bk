#pragma once 

#include "Purchase.h"
#include <arrays/Enumerator.h>

class Purchases
{
public:

	typedef std::vector<Purchase*> purchase_vector;
	typedef purchase_vector::const_iterator const_iterator;
	typedef purchase_vector::iterator iterator;
	
	Purchases();
	Purchases(const Purchases& purchases);
	Purchases(Purchases&& purchases);
	~Purchases();
	
	Purchase* Add(const std::wstring& id);
	void Add(const IPurchase* purchase);
	count_t Count() const;

	PtrEnumerator<IPurchase> Enum();

	const_iterator begin() const;
	const_iterator end() const;

	iterator begin();
	iterator end();

	
	
private:
	purchase_vector m_purchases;
};





