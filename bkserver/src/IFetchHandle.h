#pragma once

#include <boost/range/any_range.hpp>
#include "object/HentServerObject.h"
#include "object/CowServerObject.h"
#include "object/InDocServerObject.h"
#include "object/BuyDocServerObject.h"
#include "object/MoveDocServerObject.h"
#include "object/SellDocServerObject.h"
#include "object/OutDocServerObject.h"
#include "object/BuyInvoiceServerObject.h"
#include "object/SellInvoiceServerObject.h"


class IFetchHandle
{
public:
	virtual HentServerObject* NextHentObject() = 0;
	virtual size_t HentObjectsCount() = 0;
	
	virtual CowServerObject* NextCowObject() = 0;
	virtual size_t CowObjectsCount() = 0;
	
	virtual InDocServerObject* NextInDocObject() = 0;
	virtual size_t InDocObjectsCount() = 0;
	
	virtual SellDocServerObject* NextSellDocObject() = 0;
	virtual size_t SellDocObjectsCount() = 0;
	
	virtual MoveDocServerObject* NextMoveDocObject() = 0;
	virtual size_t MoveDocObjectsCount() = 0;
	
	virtual BuyDocServerObject* NextBuyDocObject() = 0;
	virtual size_t BuyDocObjectsCount() = 0;
	
	virtual OutDocServerObject* NextOutDocObject() = 0;
	virtual size_t OutDocObjectsCount() = 0;
	
	virtual BuyInvoiceServerObject* NextBuyInvoiceObject() = 0;
	virtual size_t BuyInvoiceObjectsCount() = 0;
	
	virtual SellInvoiceServerObject* NextSellInvoiceObject() = 0;
	virtual size_t SellInvoiceObjectsCount() = 0;
	
	virtual ~IFetchHandle() = 0;
};

typedef boost::shared_ptr<IFetchHandle> FetchHandle;

