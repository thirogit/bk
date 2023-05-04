#ifndef __ICOWENTRY_H__
#define __ICOWENTRY_H__


#include "../types/NullInt.h"
#include "../types/NullDecimal.h"

class ICowEntry  
{
public:		
	virtual const NullDecimal&	GetWeight() const = 0;	
	virtual uint32_t			GetStockId() const = 0;	
	virtual uint32_t			GetClassId() const = 0;			
	virtual const NullInt&		GetGroup() const = 0;			
	virtual uint32_t			GetCowId() const = 0;
};



#endif