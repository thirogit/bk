#ifndef __DEDUCTIONDEFINITIONVIEWCREATOR_H__
#define __DEDUCTIONDEFINITIONVIEWCREATOR_H__

#include "DeductionDefinitionView.h"
#include <configuration/deductions/DeductionDefinition.h>

class DeductionDefinitionViewCreator 
{
public:
	DeductionDefinitionView* CreateView(ItemId id,const DeductionDefinition* definition);
	void RefreshView(const DeductionDefinition* definition, DeductionDefinitionView* pView);
};


#endif