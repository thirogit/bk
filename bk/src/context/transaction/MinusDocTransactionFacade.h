#ifndef __MINUSDOCTRANSACTIONFACADE_H__
#define __MINUSDOCTRANSACTIONFACADE_H__

#include "DocTransactionFacade.h"

class MinusDocTransactionFacade : public DocTransactionFacade
{
public:
	virtual ItemId MinusCow(uint32_t cowId) = 0;	
};

#endif