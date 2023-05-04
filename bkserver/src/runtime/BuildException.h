#ifndef __BUILDEXCEPTION_H__
#define __BUILDEXCEPTION_H__

#include <exception/TagException.h>

struct tag_BuildException;

typedef TagException<tag_BuildException> BuildException;

#endif