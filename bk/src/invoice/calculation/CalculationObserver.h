#ifndef __CALCULATIONOBSERVER_H__
#define __CALCULATIONOBSERVER_H__

#include "CalculationElement.h"
#include "DeductionElement.h"
#include <data\runtime\ItemId.h>

class CalculationObserver
{
public:
	virtual void ElementChanged(const CalculationElement* pElement) = 0;
	virtual void ElementDeleted(ItemId id) = 0;
	virtual void ElementAdded(const CalculationElement* pElement) = 0;

	virtual void DeductionChanged(const DeductionElement* pDeduction) = 0;
	virtual void DeductionDeleted(ItemId id) = 0;
	virtual void DeductionAdded(const DeductionElement* pDeduction) = 0;

};




#endif