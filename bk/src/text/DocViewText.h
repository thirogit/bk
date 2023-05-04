#ifndef __DOCVIEWTEXT_H__
#define __DOCVIEWTEXT_H__

#include "../view/DocView.h"

class DocViewText 
{
public:
	virtual std::wstring Text(const DocView *pView,uint32_t memberId);

};

#endif