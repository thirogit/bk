#pragma once

#include <stdint.h>
#include "../../data/IDoc.h"
#include "../../data/ICowEntry.h"

class IHerdFootprint
{
public:
	virtual IHerdFootprint* PrevStep() = 0;
	virtual IHerdFootprint* NextStep() = 0;
	virtual uint32_t GetHerdId() const = 0;
	virtual IDoc* GetStepDoc() const = 0;
	virtual ICowEntry* GetStepEntry() const = 0;
};

