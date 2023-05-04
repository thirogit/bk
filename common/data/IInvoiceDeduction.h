#ifndef __IINVIOCEDEDUCTION_H__
#define __IINVIOCEDEDUCTION_H__

#include <string>
#include "../types/Decimal.h"
#include "../configuration/deductions/DeductionDetails.h"

class IInvoiceDeduction 
{
public:	
	virtual const std::wstring& 	GetCode() const = 0;	
	virtual const std::wstring& 	GetReason() const = 0;	
	virtual const Decimal&	GetFraction() const = 0;	
	virtual bool GetIsEnabled() const = 0;
	virtual DeductionDetails GetDetails() const = 0;
};


#endif
