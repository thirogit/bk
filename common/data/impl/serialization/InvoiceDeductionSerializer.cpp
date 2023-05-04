#include "stdafx.h"
#include "InvoiceDeductionSerializer.h"
#include <CommonTypes.h>
#include "../../../configuration/serialization/SerializableDeductionDetails.h"

InvoiceDeductionSerializer::InvoiceDeductionSerializer(InvoiceDeduction& deduction) : m_Deduction(deduction)
{
}

void InvoiceDeductionSerializer::serialize(SF::Archive &archive)
{
	SerializableDeductionDetails(m_Deduction.m_details).serialize(archive);
	archive & m_Deduction.m_enabled;	
}