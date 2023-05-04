#ifndef __DOCUMENTTERRITORY_H__
#define __DOCUMENTTERRITORY_H__

#include <data/IFarm.h>
#include <data/IHerd.h>

class DocumentTerritory
{
public:
	virtual ~DocumentTerritory() = 0;
	virtual const IFarm* GetFarm() const = 0;
	virtual const IHerd* GetHerd() const = 0;	
};



#endif
