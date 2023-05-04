#include "stdafx.h"
#include "HentWireObject.h"
#include <data/impl/serialization/SerializableHent.h>
 
HentWireObject::HentWireObject(uint32_t id) : Hent(id),m_Locked(false)
{
}

bool HentWireObject::GetIsLocked() const
{
	return m_Locked;
}

void HentWireObject::SetIsLocked(bool bLocked)
{
	m_Locked = bLocked;
}

void HentWireObject::serialize(SF::Archive &archive)
{
	archive & m_Locked;
	SerializableHent(this).serialize(archive);
}
