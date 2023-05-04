#ifndef __LOGICEXCEPTION_H__
#define __LOGICEXCEPTION_H__

#include <exception/TagException.h>

struct tag_LogicException {};

typedef TagException<tag_LogicException> LogicException;


#endif
