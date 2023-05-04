#include "stdafx.h"
#include "FetchStocksResponse.h"
#include <data/impl/serialization/SerializableStock.h>

FetchStocksResponse::FetchStocksResponse() 
{
}

FetchStocksResponse::~FetchStocksResponse()
{
		m_Stocks.FreeAndFlush();
}	

Stock* FetchStocksResponse::AddStock(uint32_t stockId)
{
	Stock* pNewStock = new Stock(stockId);
	m_Stocks.Add(pNewStock);
	return pNewStock;
}
		
PtrEnumerator<IStock> FetchStocksResponse::GetStocks()
{
	return PtrEnumerator<IStock>(m_Stocks);
}

count_t FetchStocksResponse::GetStocksCount() const
{
	return m_Stocks.size();
}

void FetchStocksResponse::serialize(SF::Archive &archive)
{
	count_t count;

	if(archive.isRead())
	{
		archive & count;
		for (count_t i = 0; i < count; i++)
		{
			SerializableStock(AddStock(NULL_ID)).serialize(archive);
		}
	}	
	else
	{
		count = m_Stocks.size();
		archive & count;
		for (count_t i = 0; i < count; i++)
		{
			SerializableStock(m_Stocks[i]).serialize(archive);
		}
	}
}
