#ifndef __IOWNDOC_H__
#define __IOWNDOC_H__

#include "IDoc.h"

class IOwnDoc : public IDoc
{
public:
	virtual uint32_t				GetHerdId() const = 0;
};

#endif