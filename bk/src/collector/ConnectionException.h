#pragma once

#include <exception\TagException.h>

struct tag_ConnectionFailedException {};
struct tag_ConnectionAbortedException {};

typedef TagException<tag_ConnectionFailedException> ConnectionFailedException;
typedef TagException<tag_ConnectionAbortedException> ConnectionAbortedException;