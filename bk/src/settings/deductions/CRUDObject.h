#pragma once
		
#include "CRUDFlag.h"
#include <data\runtime\IItem.h>
		
class CRUDObject : public IItem
{
public:
	CRUDObject(ItemId id);
	ItemId GetId() const;
	
	CRUDFlag GetFlag() const;
	void ChangeFlag(CRUDFlag flag);

private:
	ItemId m_id;
	CRUDFlag m_flag;		
};

