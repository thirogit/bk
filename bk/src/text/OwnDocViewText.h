#ifndef __OWNDOCVIEWTEXT_H__
#define __OWNDOCVIEWTEXT_H__

#include "DocViewText.h"
#include "../view/OwnDocView.h"

class OwnDocViewText : DocViewText
{
public:
	virtual std::wstring Text(const OwnDocView *pView,uint32_t memberId);

};

#endif