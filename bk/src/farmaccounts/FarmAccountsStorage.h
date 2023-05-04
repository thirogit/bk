#pragma once

#include "Farms.h"
#include "../localstorage/LocalStorage.h"

class FarmAccountsStorage : LocalStorage
{
public:
    FarmAccounts Load();
    void Save(const FarmAccounts& accounts);	
};