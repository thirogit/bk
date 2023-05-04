#pragma once

#include "PurchaseCowView.h"
#include <data\repo\IPurchaseCow.h>

class PurchaseCowViewCreator 
{
public:
	PurchaseCowView* CreateView(IPurchaseCow* pCow);
	void RefreshView(PurchaseCowView* pView, IPurchaseCow* pCow);
};
