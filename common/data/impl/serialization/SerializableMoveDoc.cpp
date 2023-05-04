#include "stdafx.h"
#include "SerializableMoveDoc.h"
#include "../../types/serialization/TypesSerialization.h"
#include "SerializableDocDetails.h"
#include "SerializableDocContent.h"

SerializableMoveDoc::SerializableMoveDoc(MoveDoc* pDoc) : m_pDoc(pDoc)
{
}

uint32_t& SerializableMoveDoc::Id()
{
	return m_pDoc->m_DocId;
}

uint32_t& SerializableMoveDoc::HerdFromId()
{
	return m_pDoc->m_herdFromId;
}

uint32_t& SerializableMoveDoc::HerdToId()
{
	return m_pDoc->m_herdToId;
}
	
void SerializableMoveDoc::serialize(SF::Archive &archive)
{
	archive & Id();
	archive & HerdToId();
	archive & HerdFromId();
	SerializableDocDetails(&m_pDoc->m_Details).serialize(archive);
	SerializableDocContent(&m_pDoc->m_Content).serialize(archive);
}


