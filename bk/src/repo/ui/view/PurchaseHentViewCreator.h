#ifndef __PURCHASEHENTVIEWCREATOR_H__
#define __PURCHASEHENTVIEWCREATOR_H__

#include <data\repo\IPurchaseHent.h>
#include "PurchaseHentView.h"

class PurchaseHentViewCreator  
{
public:
	PurchaseHentViewCreator();
	virtual void RefreshView(PurchaseHentView* pView, const IPurchaseHent* hent);
	virtual PurchaseHentView* CreateView(const IPurchaseHent* hent);
};


#endif