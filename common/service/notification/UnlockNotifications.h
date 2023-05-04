#pragma once

#include "UnlockNotification.h"


typedef UnlockNotification<&NotificationRelay::CowUnlock> CowUnlockNotification;
typedef UnlockNotification<&NotificationRelay::InDocUnlock> InDocUnlockNotification;
typedef UnlockNotification<&NotificationRelay::HentUnlock> HentUnlockNotification;
typedef UnlockNotification<&NotificationRelay::BuyDocUnlock> BuyDocUnlockNotification;
typedef UnlockNotification<&NotificationRelay::MoveDocUnlock> MoveDocUnlockNotification;
typedef UnlockNotification<&NotificationRelay::SellDocUnlock> SellDocUnlockNotification;
typedef UnlockNotification<&NotificationRelay::OutDocUnlock> OutDocUnlockNotification;
typedef UnlockNotification<&NotificationRelay::BuyInvoiceUnlock> BuyInvoiceUnlockNotification;
typedef UnlockNotification<&NotificationRelay::SellInvoiceUnlock> SellInvoiceUnlockNotification;
