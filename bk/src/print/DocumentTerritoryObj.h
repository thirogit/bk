#ifndef __DOCUMENTTERRITORYOBJ_H__
#define __DOCUMENTTERRITORYOBJ_H__


#include "DocumentTerritory.h"

class DocumentTerritoryObj : public DocumentTerritory
{
public:
	DocumentTerritoryObj(IFarm* farm,IHerd* herd);
	virtual ~DocumentTerritoryObj();
	virtual const IFarm* GetFarm() const;
	virtual const IHerd* GetHerd() const;	
private:
	 IFarm* m_Farm;
	 IHerd* m_Herd;
};



#endif
