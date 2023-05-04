#pragma once

#include "HentNotification.h"

#include <typestring/typestring.hh>

typedef irqus::typestring<'i', 'n', 's', 'e', 'r', 't', '_', 'h', 'e', 'n', 't'> hentinsert_nofitifcation_type;
typedef irqus::typestring<'u', 'p', 'd', 'a', 't', 'e', '_', 'h', 'e', 'n', 't'> hentupdate_nofitifcation_type;

typedef HentNotification<&NotificationRelay::HentUpdated, hentinsert_nofitifcation_type> HentUpdateNotification;
typedef HentNotification<&NotificationRelay::HentInserted, hentupdate_nofitifcation_type> HentInsertNotification;

