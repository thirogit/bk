#pragma once 

#include <boost/shared_ptr.hpp>
#include <SF/Archive.hpp>
#include <arrays/NewPtrFlushArray.h>
#include <data/repo/Purchase.h>
#include <arrays/Enumerator.h>
#include "../../CommonTypes.h"

class FetchPurchasesResponse
{
private:
	NewPtrFlushArray<Purchase> m_Purchases;
	bool m_bEOF;
public:
	FetchPurchasesResponse();
	~FetchPurchasesResponse();
	
	Purchase* Add(const std::wstring& purchaseId);
		
	PtrEnumerator<IPurchase> Enum();
	count_t GetCount() const;
	
	void serialize(SF::Archive &archive);
};

typedef boost::shared_ptr<FetchPurchasesResponse> FetchPurchasesResponseObj;
