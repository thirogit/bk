#ifndef __IPURCHASEINVIOCEDEDUCTION_H__
#define __IPURCHASEINVIOCEDEDUCTION_H__

#include <string>
#include "../../types/Decimal.h"
#include "../IIdable.h"

class IPurchaseInvoiceDeduction 
{
public:	
	virtual const std::wstring& GetId() const = 0;
	virtual const std::wstring& 	GetCode() const = 0;	
	virtual const std::wstring& 	GetReason() const = 0;	
	virtual const Decimal&	GetFraction() const = 0;	
	virtual bool GetIsEnabled() const = 0;
};


#endif
