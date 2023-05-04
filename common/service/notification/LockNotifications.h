#pragma once

#include "LockNotification.h"

typedef LockNotification<&NotificationRelay::CowLock> CowLockNotification;
typedef LockNotification<&NotificationRelay::HentLock> HentLockNotification;
typedef LockNotification<&NotificationRelay::InDocLock> InDocLockNotification;
typedef LockNotification<&NotificationRelay::BuyDocLock> BuyDocLockNotification;
typedef LockNotification<&NotificationRelay::MoveDocLock> MoveDocLockNotification;
typedef LockNotification<&NotificationRelay::SellDocLock> SellDocLockNotification;
typedef LockNotification<&NotificationRelay::OutDocLock> OutDocLockNotification;
typedef LockNotification<&NotificationRelay::BuyInvoiceLock> BuyInvoiceLockNotification;
typedef LockNotification<&NotificationRelay::SellInvoiceLock> SellInvoiceLockNotification;
