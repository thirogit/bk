#pragma once 

#include "PurchaseHeader.h"
#include <arrays/Enumerator.h>
#include <vector>

class PurchaseHeaders
{
public:

	typedef std::vector<PurchaseHeader*> purchaseheader_vector;
	typedef purchaseheader_vector::const_iterator const_iterator;
	typedef purchaseheader_vector::iterator iterator;
	typedef PurchaseHeader* value_type;
	
	PurchaseHeaders();
	PurchaseHeaders(const PurchaseHeaders& purchaseheaders);
	PurchaseHeaders(PurchaseHeaders&& purchaseheaders);
	~PurchaseHeaders();
	
	PurchaseHeader* Add(const std::wstring& id);
	void Add(const IPurchaseHeader* purchaseHeader);
	count_t Count() const;

	PtrEnumerator<IPurchaseHeader> Enum();

	const_iterator begin() const;
	const_iterator end() const;

	iterator begin();
	iterator end();

	
	
private:
	purchaseheader_vector m_headers;
};





