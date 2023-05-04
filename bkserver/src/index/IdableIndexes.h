#pragma once

#include "IdableSOIndex.h"

#include "object/HentServerObject.h"
#include "object/CowServerObject.h"
#include "object/InDocServerObject.h"
#include "object/OutDocServerObject.h"
#include "object/MoveDocServerObject.h"
#include "object/BuyDocServerObject.h"
#include "object/SellDocServerObject.h"
#include "object/BuyInvoiceServerObject.h"
#include "object/SellInvoiceServerObject.h"
#include <data/impl/BankAccount.h>


typedef IdableSOIndex<InDocServerObject> InDocsIndex;

typedef IdableIndex<Cow> CowsIndex;
typedef IdableSOIndex<CowServerObject> CowsSOIndex;

typedef IdableSOIndex<BuyDocServerObject> BuyDocsIndex;
typedef IdableSOIndex<SellDocServerObject> SellDocsIndex;
typedef IdableSOIndex<MoveDocServerObject> MoveDocsIndex;
typedef IdableSOIndex<OutDocServerObject> OutDocsIndex;
typedef IdableSOIndex<BuyInvoiceServerObject> BuyInvoicesIndex;
typedef IdableSOIndex<SellInvoiceServerObject> SellInvoicesIndex;

typedef IdableIndex<BankAccount> BankAccountIndex;

