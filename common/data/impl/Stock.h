#ifndef __STOCK_H__
#define __STOCK_H__

#include "../IStock.h"

class SerializableStock;

class Stock : public IStock
{
public:
	explicit Stock(uint32_t Id = NULL_ID);
	Stock(const IStock* pSrc);
	
	void CopyFrom(const IStock* pSrc);
	
	uint32_t GetId() const;
	virtual const std::wstring& 	GetStockName() const;	
	virtual const std::wstring& 	GetStockCode() const;	
	
		
	void	SetStockName(const std::wstring& stockName);
	void	SetStockCode(const std::wstring& stockCode);
	
	
protected:
	std::wstring	m_stockname;
	std::wstring	m_stockcode;
	uint32_t	m_stockid;

	friend class SerializableStock;
	
};


#endif