#ifndef __HENTSDOCVIEWTEXT_H__
#define __HENTSDOCVIEWTEXT_H__

#include "DocViewText.h"
#include "../view/HentsDocView.h"

class HentsDocViewText : DocViewText
{
public:
	virtual std::wstring Text(const HentsDocView *pView,uint32_t memberId);

};

#endif