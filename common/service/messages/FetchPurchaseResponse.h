#pragma once 

#include <boost/shared_ptr.hpp>
#include <SF/Archive.hpp>
#include <data/repo/Purchase.h>
#include "../../CommonTypes.h"

class FetchPurchaseResponse
{
private:
	Purchase* m_Purchase;
public:
	FetchPurchaseResponse();
	~FetchPurchaseResponse();
	
	void Set(IPurchase* purchase);
	IPurchase* Get();
	
	void serialize(SF::Archive &archive);
};

typedef boost::shared_ptr<FetchPurchaseResponse> FetchPurchaseResponseObj;
