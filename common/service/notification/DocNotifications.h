#pragma once

#include "DocNotification.h"
#include "../../data/impl/serialization/SerializableInDoc.h"
#include "../../data/impl/serialization/SerializableMoveDoc.h"
#include "../../data/impl/serialization/SerializableSellDoc.h"
#include "../../data/impl/serialization/SerializableOutDoc.h"
#include "../../data/impl/serialization/SerializableBuyDoc.h"

typedef DocNotification<InDoc,SerializableInDoc,&NotificationRelay::InDocInserted> InDocInsertNotification;
typedef DocNotification<InDoc,SerializableInDoc,&NotificationRelay::InDocUpdated> InDocUpdateNotification;
typedef DocNotification<MoveDoc,SerializableMoveDoc,&NotificationRelay::MoveDocInserted> MoveDocInsertNotification;
typedef DocNotification<MoveDoc,SerializableMoveDoc,&NotificationRelay::MoveDocUpdated> MoveDocUpdateNotification;
typedef DocNotification<SellDoc,SerializableSellDoc,&NotificationRelay::SellDocInserted> SellDocInsertNotification;
typedef DocNotification<SellDoc,SerializableSellDoc,&NotificationRelay::SellDocUpdated> SellDocUpdateNotification;
typedef DocNotification<OutDoc,SerializableOutDoc,&NotificationRelay::OutDocInserted> OutDocInsertNotification;
typedef DocNotification<OutDoc,SerializableOutDoc,&NotificationRelay::OutDocUpdated> OutDocUpdateNotification;
typedef DocNotification<BuyDoc,SerializableBuyDoc,&NotificationRelay::BuyDocInserted> BuyDocInsertNotification;
typedef DocNotification<BuyDoc,SerializableBuyDoc,&NotificationRelay::BuyDocUpdated> BuyDocUpdateNotification;