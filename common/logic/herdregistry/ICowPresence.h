#pragma once

#include <data/ICow.h>
#include <data/ICowEntry.h>
#include <types/NullDecimal.h>
#include <types/DateTime.h>
#include <types/NullInt.h>
#include <string>
#include <data/types/CowNo.h>
#include <data/types/CowSex.h>
#include <data/IClass.h>
#include <data/IHent.h>
#include <data/IStock.h>
#include <data/IDoc.h>
#include "CowPresenceKey.h"

class ICowPresence
{
public:	
	virtual const CowPresenceKey& GetPresenceKey() const = 0;
	virtual uint32_t			GetCowId() const = 0;
	virtual bool				IsInStock() const = 0;	
	virtual const CowNo&		GetCowNo() const = 0;
	virtual const std::wstring&	GetPassNo() const = 0;
	virtual uint32_t			GetFirstOwnerId() const = 0;
	virtual const DateTime&		GetBrithDate() const = 0;
	virtual const Decimal&		GetOwnWeight() const = 0;
	virtual const CowSex&		GetSex() const = 0;
	virtual uint32_t			GetOwnStockId() const = 0;
	virtual uint32_t			GetOwnClassId() const = 0;
	virtual const std::wstring&	GetExtras() const = 0;	

	virtual const IDoc*			GetHerdInDoc() const = 0;
	virtual const DateTime&		GetHerdInDate() const = 0;
	virtual const NullInt&		GetHerdInGrp() const = 0;
	virtual uint32_t			GetHerdInStockId() const = 0;
	virtual uint32_t			GetHerdInClassId() const = 0;
	virtual const NullDecimal&	GetHerdInWeight() const = 0;	

	virtual const IDoc*			GetHerdOutDoc() const = 0;
	virtual DateTime			GetHerdOutDate() const = 0;
	virtual NullInt				GetHerdOutGrp() const = 0;
	virtual uint32_t			GetHerdOutStockId() const = 0;
	virtual uint32_t			GetHerdOutClassId() const = 0;
	virtual NullDecimal			GetHerdOutWeight() const = 0;
	
	virtual uint32_t			GetTermBuyClassId() const = 0;
	virtual uint32_t			GetTermSellClassId() const = 0;

	virtual const NullDecimal&	GetTermBuyWeight() const = 0;
	virtual const NullDecimal&	GetTermSellWeight() const = 0;
	virtual const NullDecimal&	GetTermBuyPrice() const = 0;
	virtual const NullDecimal&	GetTermSellPrice() const = 0;
};
