#ifndef __ICOW_H__
#define __ICOW_H__

#include "IIdable.h"
#include <string>
#include "types/CowSex.h"
#include "../types/Longitude.h"
#include "../types/Latitude.h"
#include "../types/DateTime.h"
#include "../types/NullDecimal.h"
#include "../types/Decimal.h"

#include "types/CowNo.h"

class ICow  : public IIdable
{
public:
		
	virtual const Decimal&		GetWeight() const = 0;	
	virtual uint32_t			GetStockId() const = 0;	
	virtual uint32_t			GetClassId() const = 0;		
	virtual const CowNo&		GetCowNo() const = 0;	
	virtual const CowNo&		GetMotherNo() const = 0;
	virtual const DateTime&		GetBirthDate() const = 0;		
	virtual const std::wstring& GetPassNo() const = 0;
	virtual const std::wstring& GetHealthCertNo() const = 0;
	virtual const DateTime&		GetPassDate() const = 0;	
	virtual const std::wstring& GetBirthPlace() const = 0;
	virtual const std::wstring& GetExtras() const = 0;	
	virtual const CowSex&		GetSex() const = 0;
	virtual const NullDecimal&	GetTermBuyPrice() const = 0;
	virtual const NullDecimal&	GetTermSellPrice() const = 0;
	virtual const NullDecimal&	GetTermBuyWeight() const = 0;
	virtual const NullDecimal&	GetTermSellWeight() const = 0;
	virtual uint32_t			GetTermBuyClassId() const = 0;
	virtual uint32_t			GetTermSellClassId() const = 0;
	virtual const std::wstring& GetPassImgLabel() const = 0;
		
	virtual uint32_t			GetSellInvoiceId() const = 0;
	virtual uint32_t			GetBuyInvoiceId() const = 0;
	virtual uint32_t			GetFirstOwnerId() const = 0;

};



#endif
