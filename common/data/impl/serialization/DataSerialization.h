#pragma once 

#include <SF/Archive.hpp>
#include "../CowDetails.h"
#include "../DocDetails.h"
#include "../CowEntryDetails.h"
#include "../CowInvoiceEntryDetails.h"
#include "../InvoiceDetails.h"
#include "../../DocKey.h"
#include "../InvoiceHent.h"
#include "../InvoiceDeduction.h"

void serialize(SF::Archive &archive,CowDetails& details);
void serialize(SF::Archive &archive,CowEntryDetails& details);
void serialize(SF::Archive &archive,DocDetails& details);
void serialize(SF::Archive &archive,CowInvoiceEntryDetails& details);
void serialize(SF::Archive &archive,InvoiceDetails& details);
void serialize(SF::Archive &archive,DocKey& key);
void serialize(SF::Archive &archive,InvoiceHent& hent);
void serialize(SF::Archive &archive, InvoiceDeduction& deduction);



