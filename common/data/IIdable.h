#ifndef __IIDABLE_H__
#define __IIDABLE_H__

#include <stdint.h>

#define NULL_ID ((uint32_t)0)

class IIdable
{
public:
	virtual uint32_t GetId() const = 0;	
};

#endif