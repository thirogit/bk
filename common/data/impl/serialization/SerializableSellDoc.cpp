#include "stdafx.h"
#include "SerializableSellDoc.h"
#include "../../types/serialization/TypesSerialization.h"
#include "SerializableDocDetails.h"
#include "SerializableDocContent.h"

SerializableSellDoc::SerializableSellDoc(SellDoc* pDoc) : m_pDoc(pDoc)
{
}

uint32_t& SerializableSellDoc::Id()
{
	return m_pDoc->m_DocId;
}

uint32_t& SerializableSellDoc::HerdId()
{
	return m_pDoc->m_herdId;
}

uint32_t& SerializableSellDoc::HentId()
{
	return m_pDoc->m_hentId;
}

uint32_t& SerializableSellDoc::InvoiceId()
{
	return m_pDoc->m_invoiceId;
}
	
void SerializableSellDoc::serialize(SF::Archive &archive)
{
	archive & Id();
	archive & HerdId();
	archive & HentId();
	archive & InvoiceId();
	SerializableDocDetails(&m_pDoc->m_Details).serialize(archive);
	SerializableDocContent(&m_pDoc->m_Content).serialize(archive);
}


