#ifndef __PURCHASEHENTVIEWTEXT_H__
#define __PURCHASEHENTVIEWTEXT_H__

#include "../view/PurchaseHentView.h"

class PurchaseHentViewText 
{
public:
	virtual std::wstring Text(const PurchaseHentView *pHent,uint32_t memberId);
};

#endif