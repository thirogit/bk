#ifndef __ISEASON_H__
#define __ISEASON_H__

#include <string>
#include <types/DateTime.h>
#include "IIdable.h"

class ISeason : public IIdable
{
public:
	virtual const std::wstring& GetSeasonName() const = 0;
	virtual const DateTime& GetStartDate() const = 0;
	virtual const DateTime& GetEndDate() const = 0;	
};

#endif