#include "stdafx.h"
#include "InvoiceDeductionsSerializer.h"
#include "../../types/serialization/TypesSerialization.h"
#include "InvoiceDeductionSerializer.h"

InvoiceDeductionsSerializer::InvoiceDeductionsSerializer(InvoiceDeductions& deductions) : m_Deductions(deductions)
{
}

void InvoiceDeductionsSerializer::serialize(SF::Archive &archive)
{
	count_t count;

	if(archive.isRead())
	{
		archive & count;
		for(int i = 0;i < count;i++)
		{
			InvoiceDeduction* deduction = new InvoiceDeduction();
			InvoiceDeductionSerializer(*deduction).serialize(archive);
			m_Deductions.m_Deductions.push_back(deduction);
		}
	}
	else
	{
		count = m_Deductions.m_Deductions.size();
		archive & count;
		for(int i = 0;i < count;i++)
		{
			InvoiceDeductionSerializer(*m_Deductions.m_Deductions.at(i)).serialize(archive);
		}
	}
}