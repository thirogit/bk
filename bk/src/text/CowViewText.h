#ifndef __COWVIEWTEXT_H__
#define __COWVIEWTEXT_H__

#include "../view/CowView.h"

class CowViewText 
{
public:
	virtual std::wstring Text(const CowView *pView,uint32_t memberId);
};

#endif