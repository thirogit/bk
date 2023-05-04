#include "stdafx.h"
#include "SerializableBuyDoc.h"
#include "../../types/serialization/TypesSerialization.h"
#include "SerializableDocDetails.h"
#include "SerializableDocContent.h"

SerializableBuyDoc::SerializableBuyDoc(BuyDoc* pDoc) : m_pDoc(pDoc)
{
}

uint32_t& SerializableBuyDoc::Id()
{
	return m_pDoc->m_DocId;
}

uint32_t& SerializableBuyDoc::HerdId()
{
	return m_pDoc->m_herdId;
}

uint32_t& SerializableBuyDoc::HentId()
{
	return m_pDoc->m_hentId;
}

uint32_t& SerializableBuyDoc::InvoiceId()
{
	return m_pDoc->m_invoiceId;
}
	
void SerializableBuyDoc::serialize(SF::Archive &archive)
{
	archive & Id();
	archive & HerdId();
	archive & HentId();
	archive & InvoiceId();
	SerializableDocDetails(&m_pDoc->m_Details).serialize(archive);
	SerializableDocContent(&m_pDoc->m_Content).serialize(archive);
}


