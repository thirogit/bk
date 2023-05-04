#ifndef __PURCHASEHEADERVIEWCREATOR_H__
#define __PURCHASEHEADERVIEWCREATOR_H__

#include <data\repo\IPurchaseHeader.h>
#include "PurchaseHeaderView.h"

class PurchaseHeaderViewCreator  
{
public:
	PurchaseHeaderViewCreator();
	virtual void RefreshView(PurchaseHeaderView* pView,const IPurchaseHeader* pPurchaseHeader);
	virtual PurchaseHeaderView* CreateView(const IPurchaseHeader* pPurchaseHeader);
};


#endif