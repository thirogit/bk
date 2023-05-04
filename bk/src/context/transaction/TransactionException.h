#pragma once

#include <exception\TagException.h>

struct tag_TransactionException {};

typedef TagException<tag_TransactionException> TransactionException;