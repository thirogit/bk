#include "stdafx.h"
#include "CowWireObject.h"
#include <data/impl/serialization/SerializableCow.h>
 
CowWireObject::CowWireObject(uint32_t id) : Cow(id),m_Locked(false)
{
}

bool CowWireObject::GetIsLocked() const
{
	return m_Locked;
}

void CowWireObject::SetIsLocked(bool bLocked)
{
	m_Locked = bLocked;
}

void CowWireObject::serialize(SF::Archive &archive)
{
	archive & m_Locked;
	SerializableCow(this).serialize(archive);
}
