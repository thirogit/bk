#pragma once

#include <exception\TagException.h>

struct tag_ContextException {};

typedef TagException<tag_ContextException> ContextException;