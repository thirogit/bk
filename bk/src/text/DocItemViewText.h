#ifndef __DOCITEMVIEWTEXT_H__
#define __DOCITEMVIEWTEXT_H__

#include "../view/DocItemView.h"

class DocItemViewText 
{
public:
	virtual std::wstring Text(const DocItemView *pView,uint32_t memberId);

};

#endif