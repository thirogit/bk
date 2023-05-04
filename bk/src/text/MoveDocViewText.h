#ifndef __MOVEDOCVIEWTEXT_H__
#define __MOVEDOCVIEWTEXT_H__

#include "DocViewText.h"
#include "../view/MoveDocView.h"

class MoveDocViewText : DocViewText
{
public:
	virtual std::wstring Text(const MoveDocView *pView,uint32_t memberId);

};

#endif