#ifndef __BUYCALCULATIONVIEWCREATOR_H__
#define __BUYCALCULATIONVIEWCREATOR_H__

#include "CalculationViewCreator.h"

class BuyCalculationViewCreator : public CalculationViewCreator
{
public:
	BuyCalculationViewCreator(SeasonSession* facade);
protected:
	virtual NullDecimal GetOtherPrice(const CalculationElement* pElement);
	virtual uint32_t GetOtherHent(const CalculationElement* pElement);
	virtual NullDecimal GetProfit(const CalculationElement* pElement);

	ISellInvoice* GetSellInvoice(const CalculationElement* pElement);
};

#endif