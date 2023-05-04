#pragma once

#include <boost/tuple/tuple.hpp>
#include "../impl/InvoiceDeduction.h"
#include "ItemId.h"

typedef boost::tuple<ItemId, InvoiceDeduction> DeductionItemTuple;


