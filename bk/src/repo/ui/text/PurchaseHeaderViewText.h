#ifndef __PURCHASEHEADERVIEWTEXT_H__
#define __PURCHASEHEADERVIEWTEXT_H__

#include "../view/PurchaseHeaderView.h"

class PurchaseHeaderViewText 
{
public:
	virtual std::wstring Text(const PurchaseHeaderView *pView,uint32_t memberId);

};

#endif