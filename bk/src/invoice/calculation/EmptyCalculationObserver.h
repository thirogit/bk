#ifndef __EMPTYCALCULATIONOBSERVER_H__
#define __EMPTYCALCULATIONOBSERVER_H__

#include "CalculationObserver.h"

class EmptyCalculationObserver : public CalculationObserver
{
public:
	virtual void ElementChanged(const CalculationElement* pElement) {};
	virtual void ElementDeleted(ItemId id) {};
	virtual void ElementAdded(const CalculationElement* pElement) {};

	virtual void DeductionChanged(const DeductionElement* pDeduction) {};
	virtual void DeductionDeleted(ItemId id) {};
	virtual void DeductionAdded(const DeductionElement* pDeduction) {};

};




#endif