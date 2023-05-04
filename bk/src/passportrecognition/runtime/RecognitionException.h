#ifndef __RECOGNITIONEXCEPTION_H__
#define __RECOGNITIONEXCEPTION_H__

#include <exception\TagException.h>

struct tag_RecognitionException{};

typedef TagException<tag_RecognitionException> RecognitionException;

#endif