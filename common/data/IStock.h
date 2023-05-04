#ifndef __ISTOCK_H__
#define __ISTOCK_H__

#include "IIdable.h"
#include <string>
#include "types/CowSex.h"

class IStock : public IIdable
{
public:	
	virtual const std::wstring& 	GetStockName() const = 0;	
	virtual const std::wstring& 	GetStockCode() const = 0;	
};


#endif
