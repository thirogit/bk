#pragma once

#include "InvoiceNotification.h"
#include "../../data/impl/serialization/SerializableBuyInvoice.h"
#include "../../data/impl/serialization/SerializableSellInvoice.h"
#include <typestring/typestring.hh>

typedef irqus::typestring<'i', 'n', 's', 'e', 'r', 't', '_', 'b', 'u', 'y', 'i', 'n', 'v'> buyinvoiceinsert_nofitifcation_type;
typedef irqus::typestring<'u', 'p', 'd', 'a', 't', 'e', '_', 'b', 'u', 'y', 'i', 'n', 'v'> buyinvoiceupdate_nofitifcation_type;
typedef irqus::typestring<'i', 'n', 's', 'e', 'r', 't', '_', 's', 'e', 'l', 'l', 'i', 'n', 'v'> sellinvoiceinsert_nofitifcation_type;
typedef irqus::typestring<'u', 'p', 'd', 'a', 't', 'e', '_', 's', 'e', 'l', 'l', 'i', 'n', 'v'> sellinvoiceupdate_nofitifcation_type;



typedef InvoiceNotification<BuyInvoice,SerializableBuyInvoice,&NotificationRelay::BuyInvoiceInserted, buyinvoiceinsert_nofitifcation_type> BuyInvoiceInsertNotification;
typedef InvoiceNotification<BuyInvoice,SerializableBuyInvoice,&NotificationRelay::BuyInvoiceUpdated, buyinvoiceupdate_nofitifcation_type> BuyInvoiceUpdateNotification;
typedef InvoiceNotification<SellInvoice,SerializableSellInvoice,&NotificationRelay::SellInvoiceInserted,sellinvoiceinsert_nofitifcation_type> SellInvoiceInsertNotification;
typedef InvoiceNotification<SellInvoice,SerializableSellInvoice,&NotificationRelay::SellInvoiceUpdated, sellinvoiceupdate_nofitifcation_type> SellInvoiceUpdateNotification;