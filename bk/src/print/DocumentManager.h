#ifndef __DOCUMENTMANAGER_H__
#define __DOCUMENTMANAGER_H__

#include "DocumentSelection.h"

class DocumentManager
{
public:
	virtual DocumentSelection FindDocuments(uint32_t inputTypeMask) = 0;
};


#endif
