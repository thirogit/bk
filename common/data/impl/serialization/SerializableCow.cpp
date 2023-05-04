#include "stdafx.h"
#include "SerializableCow.h"
#include "../../types/serialization/TypesSerialization.h"
#include "SerializableCowDetails.h"

SerializableCow::SerializableCow(Cow* pCow) : m_pCow(pCow)
{
}

uint32_t& SerializableCow::Id()
{
	return m_pCow->m_cowId;
}


uint32_t& SerializableCow::BuyInvoiceId()
{
	return m_pCow->m_buyInvoice;
}

uint32_t& SerializableCow::SellInvoiceId()
{
	return m_pCow->m_sellInvoice;
}


std::wstring& SerializableCow::PassImgLabel()
{
	return m_pCow->m_passimglabel;
}

CowDetails& SerializableCow::Details()
{
	return m_pCow->m_details;
}

	
void SerializableCow::serialize(SF::Archive &archive)
{
	archive & Id();
	archive & BuyInvoiceId();		
	archive & SellInvoiceId();		
	archive & PassImgLabel();
	SerializableCowDetails(Details()).serialize(archive);
	
}