#include "stdafx.h"
#include "SerializableDeductionDetails.h"
#include "../../data/types/serialization/TypesSerialization.h"

SerializableDeductionDetails::SerializableDeductionDetails(DeductionDetails& details) : m_details(details)
{
}

void SerializableDeductionDetails::serialize(SF::Archive &archive)
{
	if(archive.isRead())
	{
		std::wstring code;
		std::wstring reason;
		Decimal	fraction;

		archive & code & reason & fraction;
		m_details.SetCode(code);
		m_details.SetReason(reason);
		m_details.SetFraction(fraction);		
	} else {
		archive & m_details.GetCode() & m_details.GetReason() & m_details.GetFraction();
	}
		
}
