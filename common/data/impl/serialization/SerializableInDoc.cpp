#include "stdafx.h"
#include "SerializableInDoc.h"
#include "../../types/serialization/TypesSerialization.h"
#include "SerializableDocDetails.h"
#include "SerializableDocContent.h"

SerializableInDoc::SerializableInDoc(InDoc* pInDoc) : m_pInDoc(pInDoc)
{
}

uint32_t& SerializableInDoc::Id()
{
	return m_pInDoc->m_DocId;
}

uint32_t& SerializableInDoc::HerdId()
{
	return m_pInDoc->m_herdId;
}
	
void SerializableInDoc::serialize(SF::Archive &archive)
{
	archive & Id();
	archive & HerdId();
	SerializableDocDetails(&m_pInDoc->m_Details).serialize(archive);
	SerializableDocContent(&m_pInDoc->m_Content).serialize(archive);
}


