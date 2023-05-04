#ifndef __COWPRESENCEVIEWTEXT_H__
#define __COWPRESENCEVIEWTEXT_H__

#include "../view/CowPresenceView.h"

class CowPresenceViewText 
{
public:
	virtual std::wstring Text(const CowPresenceView *pView,uint32_t memberId);
};

#endif