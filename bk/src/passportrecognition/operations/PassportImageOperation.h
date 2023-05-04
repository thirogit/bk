#ifndef __PASSPORTIMAGEOPERATION_H__
#define __PASSPORTIMAGEOPERATION_H__

#include "../PassportImage.h"
#include <exception/TagException.h>

struct tag_PassportImageOperationException {};
typedef TagException<tag_PassportImageOperationException> PassportImageOperationException;

class PassportImageOperation
{
public:
	virtual void DoOperation(PassportImageObject* passportImgObj) = 0;
};



#endif