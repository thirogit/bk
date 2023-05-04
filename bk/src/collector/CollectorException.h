#pragma once

#include <exception\TagException.h>

struct tag_CollectorException {};
struct tag_CollectorNotFoundException {};

typedef TagException<tag_CollectorException> CollectorException;

typedef TagException<tag_CollectorNotFoundException> CollectorNotFoundException;