#include "stdafx.h"
#include "LockableObject.h"
#include <cstddef>

LockableObject::LockableObject() : m_pClient(NULL), m_lockType(LockType_None)
{
}

bool LockableObject::Lock(ClientSeasonSession* pClient, LockType lockType)
{
	if(m_pClient == NULL && pClient != NULL)
	{
		m_pClient = pClient;
		m_lockType = lockType;
		return true;
	}
	return false;	
}

LockType LockableObject::GetLockType() const
{
	return m_lockType;
}

bool LockableObject::Unlock(ClientSeasonSession* pClient)
{	
	if(pClient == m_pClient)
	{
		m_pClient = NULL;
		m_lockType = LockType_None;
		return true;
	}
	return false;
}

bool LockableObject::OwnsLock(ClientSeasonSession* pClient)
{
	return pClient == m_pClient;	
}

bool LockableObject::IsLocked()
{
	return m_pClient != NULL;
}

ClientSeasonSession* LockableObject::GetLockHolder() const
{
	return m_pClient;
}