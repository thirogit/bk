#pragma once 

#include <boost/shared_ptr.hpp>
#include <SF/Archive.hpp>
#include <arrays/NewPtrFlushArray.h>
#include <data/impl/Stock.h>
#include <arrays/Enumerator.h>
#include "../../CommonTypes.h"

class FetchStocksResponse
{
private:
	NewPtrFlushArray<Stock> m_Stocks;
public:
	FetchStocksResponse();
	~FetchStocksResponse();
	
	Stock* AddStock(uint32_t stockId);
		
	PtrEnumerator<IStock> GetStocks();
	count_t GetStocksCount() const;
	
	void serialize(SF::Archive &archive);
};

typedef boost::shared_ptr<FetchStocksResponse> FetchStocksResponseObj;
