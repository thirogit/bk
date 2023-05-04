#include "stdafx.h"
#include "CowRequest.h"
#include <data/impl/serialization/SerializableCowDetails.h>

CowRequest::CowRequest(uint32_t cowId/* = NULL_ID*/) : m_cowId(cowId)
{
}

uint32_t CowRequest::GetCowId() const
{
	return m_cowId;
}

CowRequest& CowRequest::WithDetails(const CowDetails& details)
{
	m_details.CopyFrom(details);
	return *this;
}

void CowRequest::serialize(SF::Archive &archive)
{
	archive & m_cowId;
	SerializableCowDetails(m_details).serialize(archive);
}
	
const CowDetails& CowRequest::GetDetails() const
{
	return m_details;
}
