#ifndef __ICLASS_H__
#define __ICLASS_H__

#include "IIdable.h"
#include <string>
#include "types/CowSex.h"
#include "../types/NullDecimal.h"

class IClass : public IIdable
{
public:	
	virtual const std::wstring& 	GetClassNm() const = 0;	
	virtual const std::wstring& 	GetClassCode() const = 0;	
	virtual const NullDecimal&	GetMyPerKgPrice() const = 0;	
	virtual const CowSex&	    GetPredefSex() const = 0;	
	virtual uint32_t			GetRuleId() const = 0;

};


#endif
