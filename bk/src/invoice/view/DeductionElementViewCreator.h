#ifndef __DEDUCTIONELEMENTVIEWCREATOR_H__
#define __DEDUCTIONELEMENTVIEWCREATOR_H__

#include <types\NullDecimal.h>
#include <stdint.h>
#include "DeductionElementView.h"
#include "../calculation/DeductionElement.h"
#include "../../context/SeasonSession.h"

class DeductionElementViewCreator 
{
public:
	DeductionElementViewCreator(SeasonSession* facade);
	DeductionElementView* CreateView(const DeductionElement* pElement);
	void RefreshView(const DeductionElement* pElement,DeductionElementView* pView);	
protected:
	SeasonSession* m_pFacade;
};


#endif