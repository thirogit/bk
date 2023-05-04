#ifndef __PURCHASECOWVIEWTEXT_H__
#define __PURCHASECOWVIEWTEXT_H__

#include "../view/PurchaseCowView.h"

class PurchaseCowViewText 
{
public:
	virtual std::wstring Text(const PurchaseCowView *pView,uint32_t memberId);
};

#endif