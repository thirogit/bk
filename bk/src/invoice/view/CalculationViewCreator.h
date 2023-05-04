#ifndef __CALCULATIONVIEWCREATOR_H__
#define __CALCULATIONVIEWCREATOR_H__

#include <types\NullDecimal.h>
#include <stdint.h>
#include "CalculationElementView.h"
#include "../calculation/CalculationElement.h"
#include "../../context/SeasonSession.h"

class CalculationViewCreator 
{
public:
	CalculationViewCreator(SeasonSession* facade);
	CalculationElementView* CreateView(const CalculationElement* pElement);
	void RefreshView(const CalculationElement* pElement,CalculationElementView* pView);	
protected:
	ICow* GetCow(const CalculationElement* pElement);

	virtual NullDecimal GetOtherPrice(const CalculationElement* pElement) = 0;
	virtual uint32_t GetOtherHent(const CalculationElement* pElement) = 0;
	virtual NullDecimal GetProfit(const CalculationElement* pElement) = 0;
protected:
	SeasonSession* m_pFacade;
};


#endif