#ifndef __SEASONTEXT_H__
#define __SEASONTEXT_H__

#include <data\ISeason.h>

class SeasonText 
{
public:
	virtual std::wstring Text(const ISeason *pSeason,uint32_t memberId);
};

#endif