#include "stdafx.h"
#include "HentRequest.h"
#include <data/impl/serialization/SerializableHent.h>

HentRequest::HentRequest() : m_pHent(NULL)
{
}

HentRequest::~HentRequest()
{
	if(m_pHent)
		delete m_pHent;
}

IHent* HentRequest::GetHent() const
{
	return m_pHent;
}

Hent* HentRequest::CreateHent(uint32_t hentId)
{
	if(!m_pHent)
		m_pHent = new Hent(hentId);
	return m_pHent;
}

void HentRequest::serialize(SF::Archive &archive)
{
	if(archive.isRead())
	{
		CreateHent(NULL_ID);
		SerializableHent(m_pHent).serialize(archive);
	}
	else
	{
		SerializableHent(m_pHent).serialize(archive);
	}
}
	
