#pragma once

#include <boost/tuple/tuple.hpp>
#include "InvoiceItemKey.h"

typedef boost::tuple<InvoiceItemKey,uint32_t,CowDetails,CowInvoiceEntryDetails> InvoiceTransactionItemTuple;


