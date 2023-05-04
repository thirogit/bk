#ifndef __SESSIONEXCEPTION_H__
#define __SESSIONEXCEPTION_H__

#include <exception/TagException.h>

struct tag_SessionException {};

typedef TagException<tag_SessionException> SessionException;


#endif