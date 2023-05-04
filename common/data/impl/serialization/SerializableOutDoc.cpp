#include "stdafx.h"
#include "SerializableOutDoc.h"
#include "../../types/serialization/TypesSerialization.h"
#include "SerializableDocDetails.h"
#include "SerializableDocContent.h"

SerializableOutDoc::SerializableOutDoc(OutDoc* pDoc) : m_pDoc(pDoc)
{
}

uint32_t& SerializableOutDoc::Id()
{
	return m_pDoc->m_DocId;
}

uint32_t& SerializableOutDoc::HerdId()
{
	return m_pDoc->m_herdId;
}
	
void SerializableOutDoc::serialize(SF::Archive &archive)
{
	archive & Id();
	archive & HerdId();
	SerializableDocDetails(&m_pDoc->m_Details).serialize(archive);
	SerializableDocContent(&m_pDoc->m_Content).serialize(archive);
}


