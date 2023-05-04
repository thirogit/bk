#ifndef __FEEDEXCEPTION_H__
#define __FEEDEXCEPTION_H__

#include <exception/TagException.h>

struct tag_FeedException {};

typedef TagException<tag_FeedException> FeedException;

#endif
