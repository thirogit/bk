#pragma once

#include "../data/IHent.h"
#include "../data/impl/DocDetails.h"
#include "../data/impl/CowDetails.h"
#include "../data/impl/CowEntryDetails.h"
#include "../configuration/deductions/IDeductionDefinition.h"

std::wstring ToString(const IHent* hent);
std::wstring ToString(const DocDetails& details);
std::wstring ToString(const CowDetails& details);
std::wstring ToString(const CowEntryDetails& details);
std::wstring ToString(const IDeductionDefinition* definition);
 
