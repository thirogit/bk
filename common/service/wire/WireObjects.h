#pragma once 

#include "WireObject.h"
#include <data/impl/serialization/SerializableInDoc.h>
#include <data/impl/serialization/SerializableBuyDoc.h>
#include <data/impl/serialization/SerializableMoveDoc.h>
#include <data/impl/serialization/SerializableOutDoc.h>
#include <data/impl/serialization/SerializableSellDoc.h>

typedef WireObject<InDoc,SerializableInDoc> InDocWireObject;
typedef WireObject<BuyDoc,SerializableBuyDoc> BuyDocWireObject;
typedef WireObject<MoveDoc,SerializableMoveDoc> MoveDocWireObject;
typedef WireObject<OutDoc,SerializableOutDoc> OutDocWireObject;
typedef WireObject<SellDoc,SerializableSellDoc> SellDocWireObject;
