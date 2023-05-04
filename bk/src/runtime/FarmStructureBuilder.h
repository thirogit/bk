#ifndef __FARMSTRUCTUREBUILDER_H__
#define __FARMSTRUCTUREBUILDER_H__

#include "FarmStructure.h"
#include "../context/SeasonSession.h"

class FarmStructureBuilder
{
public:
	FarmStructureBuilder(SeasonSession* session);	
	FarmStructure* Build();
private:
};


#endif