#pragma once

#include "ItemId.h"

class IItem
{
public:
	virtual ItemId GetId() const = 0;
};

