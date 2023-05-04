#pragma once 

#include <boost/shared_ptr.hpp>
#include <SF/Archive.hpp>
#include <arrays/NewPtrFlushArray.h>
#include <data/repo/PurchaseHeader.h>
#include <arrays/Enumerator.h>
#include "../../CommonTypes.h"

class FetchPurchaseHeadersResponse
{
private:
	NewPtrFlushArray<PurchaseHeader> m_PurchaseHeaders;	
public:
	FetchPurchaseHeadersResponse();
	~FetchPurchaseHeadersResponse();
	
	PurchaseHeader* Add(const std::wstring& purchaseId);
		
	PtrEnumerator<IPurchaseHeader> Enum();
	count_t GetCount() const;
	
	void serialize(SF::Archive &archive);
};

typedef boost::shared_ptr<FetchPurchaseHeadersResponse> FetchPurchaseHeadersResponseObj;
