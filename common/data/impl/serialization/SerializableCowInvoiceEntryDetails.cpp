#include "stdafx.h"
#include "SerializableCowInvoiceEntryDetails.h"
#include "../../types/serialization/TypesSerialization.h"

SerializableCowInvoiceEntryDetails::SerializableCowInvoiceEntryDetails(CowInvoiceEntryDetails& invoiceEntryDetails) : m_InvoiceEntryDetails(invoiceEntryDetails)
{
}

Decimal& SerializableCowInvoiceEntryDetails::Weight()
{
	return m_InvoiceEntryDetails.m_Weight;
}


uint32_t& SerializableCowInvoiceEntryDetails::ClassId()
{
	return m_InvoiceEntryDetails.m_classId;
}

Decimal& SerializableCowInvoiceEntryDetails::Price()
{
	return m_InvoiceEntryDetails.m_Price;
}
	
void SerializableCowInvoiceEntryDetails::serialize(SF::Archive &archive)
{
	archive & Weight();
	archive & ClassId();	
	archive & Price();	
}