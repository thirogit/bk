#ifndef __COWVIEWTEXT_H__
#define __COWVIEWTEXT_H__

#include "../view/RecognitionItemView.h"

class RecognitionItemViewText 
{
public:
	virtual std::wstring Text(const RecognitionItemView *pView, uint32_t memberId);
};

#endif