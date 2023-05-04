#include "stdafx.h"
#include "Stock.h"

Stock::Stock(uint32_t Id /*= NULL_ID*/) : m_stockid(Id)
{	

}

Stock::Stock(const IStock* pSrc)
{
	m_stockid = pSrc->GetId();
	CopyFrom(pSrc);
}

uint32_t Stock::GetId() const
{
	return m_stockid;
}


const std::wstring&	Stock::GetStockName() const
{
	return m_stockname;
}

void Stock::SetStockName(const std::wstring& stockName)
{
	m_stockname = stockName;
}

const std::wstring&	Stock::GetStockCode() const
{
	return m_stockcode;
}

void Stock::SetStockCode(const std::wstring& stockCode)
{
	m_stockcode = stockCode;
}


void Stock::CopyFrom(const IStock* pSrc)
{
	SetStockName(pSrc->GetStockName()); 
	SetStockCode(pSrc->GetStockCode());	
}
