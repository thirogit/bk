#ifndef __TRANSACTIONEXCEPTION_H__
#define __TRANSACTIONEXCEPTION_H__

#include <exception/TagException.h>

struct tag_TransactionException {};

typedef TagException<tag_TransactionException> TransactionException;


#endif