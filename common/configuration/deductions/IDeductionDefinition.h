#ifndef __IDEDUCTIONDEFINITION_H__
#define __IDEDUCTIONDEFINITION_H__

#include "../../data/IIdable.h"
#include <string>
#include "../../types/Decimal.h"

class IDeductionDefinition : public IIdable
{
public:	
	virtual const std::wstring& 	GetCode() const = 0;	
	virtual const std::wstring& 	GetReason() const = 0;	
	virtual const Decimal&	GetFraction() const = 0;	
	virtual bool GetIsEnabledByDefault() const = 0;

};


#endif
