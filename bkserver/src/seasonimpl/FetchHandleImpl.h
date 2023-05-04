#pragma once

#include "IFetchHandle.h"
#include "../index/HentsIndex.h"
#include "../index/ClassesIndex.h"
#include "../index/StocksIndex.h"
#include "../index/IdableIndexes.h"

class SeasonSession;

class FetchHandleImpl : public IFetchHandle
{
public:
	FetchHandleImpl(SeasonSession* pSession);
	virtual ~FetchHandleImpl();

	virtual HentServerObject* NextHentObject();
	virtual size_t HentObjectsCount();

	virtual CowServerObject* NextCowObject();
	virtual size_t CowObjectsCount();

	virtual InDocServerObject* NextInDocObject();
	virtual size_t InDocObjectsCount();

	virtual SellDocServerObject* NextSellDocObject();
	virtual size_t SellDocObjectsCount();

	virtual MoveDocServerObject* NextMoveDocObject();
	virtual size_t MoveDocObjectsCount();

	virtual BuyDocServerObject* NextBuyDocObject();
	virtual size_t BuyDocObjectsCount();

	virtual OutDocServerObject* NextOutDocObject();
	virtual size_t OutDocObjectsCount();

	virtual BuyInvoiceServerObject* NextBuyInvoiceObject();
	virtual size_t BuyInvoiceObjectsCount();

	virtual SellInvoiceServerObject* NextSellInvoiceObject();
	virtual size_t SellInvoiceObjectsCount();

private:
	SeasonSession* m_pSession;
	HentsSOIndex::iterator m_HentIt;			
	CowsSOIndex::iterator m_CowIt;
	InDocsIndex::iterator m_InDocIt;
	OutDocsIndex::iterator m_OutDocIt;
	BuyDocsIndex::iterator m_BuyDocIt;
	SellDocsIndex::iterator m_SellDocIt;
	MoveDocsIndex::iterator m_MoveDocIt;
	BuyInvoicesIndex::iterator m_BuyInvoiceIt;
	SellInvoicesIndex::iterator m_SellInvoiceIt;
};
