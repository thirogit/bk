#pragma once 

#include <SF/Archive.hpp>
#include "../Purchase.h"
#include "../PurchaseObjPath.h"

void serialize(SF::Archive &archive,Purchase& purchase);
void serialize(SF::Archive &archive, PurchaseObjPath& purchaseObjPath);




