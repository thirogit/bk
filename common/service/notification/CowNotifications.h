#pragma once

#include "CowNotification.h"

typedef CowNotification<&NotificationRelay::CowInserted> CowInsertNotification;
typedef CowNotification<&NotificationRelay::CowUpdated> CowUpdateNotification;