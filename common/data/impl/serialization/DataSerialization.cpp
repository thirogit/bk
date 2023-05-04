#include "stdafx.h"
#include "DataSerialization.h"
#include "SerializableDocDetails.h"
#include "SerializableCowDetails.h"
#include "SerializableCowEntryDetails.h"
#include "SerializableCowInvoiceEntryDetails.h"
#include "SerializableInvoiceDetails.h"
#include "SerializableInvoiceHent.h"
#include "InvoiceDeductionSerializer.h"

void serialize(SF::Archive &archive,CowDetails& details)
{
	SerializableCowDetails(details).serialize(archive);
}

void serialize(SF::Archive &archive,DocDetails& details)
{
	SerializableDocDetails(&details).serialize(archive);	
}

void serialize(SF::Archive &archive,CowEntryDetails& details)
{
	SerializableCowEntryDetails(details).serialize(archive);
}

void serialize(SF::Archive &archive,CowInvoiceEntryDetails& details)
{
	SerializableCowInvoiceEntryDetails(details).serialize(archive);
}

void serialize(SF::Archive &archive,InvoiceDetails& details)
{
	SerializableInvoiceDetails(details).serialize(archive);
}

void serialize(SF::Archive &archive,DocKey& key)
{
	if(archive.isWrite())
	{
		archive & key.GetDocType();
		archive & key.GetDocNo();
	}
	else
	{
		DocType docType;
		uint32_t docId;

		archive & docType;
		archive & docId;

		key = DocKey(docType,docId);
	}
}

void serialize(SF::Archive &archive,InvoiceHent& hent)
{
	SerializableInvoiceHent(hent).serialize(archive);
}

void serialize(SF::Archive &archive, InvoiceDeduction& deduction)
{
	InvoiceDeductionSerializer(deduction).serialize(archive);
}