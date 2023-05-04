#ifndef __IMOVEDOC_H__
#define __IMOVEDOC_H__

#include "IDoc.h"

class IMoveDoc : public IDoc
{
public:
	virtual uint32_t 				GetSrcHerdId() const = 0;
	virtual uint32_t				GetDstHerdId() const = 0;
};

#endif