#ifndef __HENTTEXT_H__
#define __HENTTEXT_H__

#include "../view/HentView.h"

class HentText 
{
public:
	virtual std::wstring Text(const HentView *pHent,uint32_t memberId);
};

#endif