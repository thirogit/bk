#ifndef __SELLCALCULATIONVIEWCREATOR_H__
#define __SELLCALCULATIONVIEWCREATOR_H__

#include "CalculationViewCreator.h"

class SellCalculationViewCreator : public CalculationViewCreator
{
public:
	SellCalculationViewCreator(SeasonSession* facade);
protected:
	virtual NullDecimal GetOtherPrice(const CalculationElement* pElement);
	virtual uint32_t GetOtherHent(const CalculationElement* pElement);
	virtual NullDecimal GetProfit(const CalculationElement* pElement);

	IBuyInvoice* GetBuyInvoice(const CalculationElement* pElement);
};

#endif