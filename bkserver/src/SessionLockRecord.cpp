#include "stdafx.h"
#include "SessionLockRecord.h"
#include "ClientSeasonSession.h"
#include <arrays/DeepDelete.h>

ObjectLock::ObjectLock() : m_type(LockObject_None), m_id(NULL_ID)
{
}

ObjectLock::ObjectLock(LockObject type,uint32_t id) : m_type(type), m_id(id)
{
}

ObjectLock::ObjectLock(const ObjectLock& src) : m_type(src.m_type), m_id(src.m_id)
{
}

LockObject ObjectLock::GetType() const
{
	return m_type;
}

uint32_t ObjectLock::GetId() const
{
	return m_id;
}

//-------------------------------------------------------------------------------------------------

SessionLockRecord::ObjectLockRegistry::~ObjectLockRegistry()
{
	DeepDelete(m_Registry);
}

void SessionLockRecord::ObjectLockRegistry::Remember(LockObject objectType,uint32_t objectId)
{
	m_Registry.insert(new ObjectLock(objectType,objectId));
}

const ObjectLock* SessionLockRecord::ObjectLockRegistry::FirstLock()
{
	auto lockIt = m_Registry.begin();
	if(lockIt != m_Registry.end())
	{
		return *lockIt;
	}

	return NULL;
}

void SessionLockRecord::ObjectLockRegistry::Forget(LockObject objectType,uint32_t objectId)
{
	boost::multi_index::index<_Registry,index_by_type>::type& indexByType = m_Registry.get<index_by_type>();

	auto lockRange = indexByType.equal_range(objectType);
	ObjectLock*  lock;
	for(auto lockIt = lockRange.first;lockIt != lockRange.second;lockIt++)
	{
		lock = *lockIt;
		if(lock->GetId() == objectId)
		{
			m_Registry.erase(lockIt);
			delete lock;
			break;
		}
	}
}

size_t SessionLockRecord::ObjectLockRegistry::LockCount() const
{
	return m_Registry.size();
}

//-------------------------------------------------------------------------------------------------

SessionLockRecord::SessionLockRecord()
{
}

SessionLockRecord::~SessionLockRecord()
{
	DeepDelete(m_SessionLocks);
}
	
void SessionLockRecord::RememberLock(ClientSeasonSession* pSession, HentServerObject* pSO)
{
	RememberLock(pSession,LockObject_Hent,pSO->Object()->GetId());
}

void SessionLockRecord::RememberLock(ClientSeasonSession* pSession, CowServerObject* pSO)
{
	RememberLock(pSession,LockObject_Cow,pSO->Object()->GetId());
}

void SessionLockRecord::RememberLock(ClientSeasonSession* pSession, InDocServerObject* pSO)
{
	RememberLock(pSession,LockObject_InDoc,pSO->Object()->GetId());
}

void SessionLockRecord::RememberLock(ClientSeasonSession* pSession, BuyDocServerObject* pSO)
{
	RememberLock(pSession,LockObject_BuyDoc,pSO->Object()->GetId());
}

void SessionLockRecord::RememberLock(ClientSeasonSession* pSession, MoveDocServerObject* pSO)
{
	RememberLock(pSession,LockObject_MoveDoc,pSO->Object()->GetId());
}

void SessionLockRecord::RememberLock(ClientSeasonSession* pSession, SellDocServerObject* pSO)
{
	RememberLock(pSession,LockObject_SellDoc,pSO->Object()->GetId());
}

void SessionLockRecord::RememberLock(ClientSeasonSession* pSession, OutDocServerObject* pSO)
{
	RememberLock(pSession,LockObject_OutDoc,pSO->Object()->GetId());
}

void SessionLockRecord::RememberLock(ClientSeasonSession* pSession, BuyInvoiceServerObject* pSO)
{
	RememberLock(pSession,LockObject_BuyInvoice,pSO->Object()->GetId());
}

void SessionLockRecord::RememberLock(ClientSeasonSession* pSession, SellInvoiceServerObject* pSO)
{
	RememberLock(pSession,LockObject_SellInvoice,pSO->Object()->GetId());
}

void SessionLockRecord::ForgetLock(ClientSeasonSession* pSession, HentServerObject* pSO)
{
	ForgetLock(pSession,LockObject_Hent,pSO->Object()->GetId());
}

void SessionLockRecord::ForgetLock(ClientSeasonSession* pSession, CowServerObject* pSO)
{
	ForgetLock(pSession,LockObject_Cow,pSO->Object()->GetId());
}

void SessionLockRecord::ForgetLock(ClientSeasonSession* pSession, InDocServerObject* pSO)
{
	ForgetLock(pSession,LockObject_InDoc,pSO->Object()->GetId());
}

void SessionLockRecord::ForgetLock(ClientSeasonSession* pSession, BuyDocServerObject* pSO)
{
	ForgetLock(pSession,LockObject_BuyDoc,pSO->Object()->GetId());
}

void SessionLockRecord::ForgetLock(ClientSeasonSession* pSession, MoveDocServerObject* pSO)
{
	ForgetLock(pSession,LockObject_MoveDoc,pSO->Object()->GetId());
}

void SessionLockRecord::ForgetLock(ClientSeasonSession* pSession, SellDocServerObject* pSO)
{
	ForgetLock(pSession,LockObject_SellDoc,pSO->Object()->GetId());
}

void SessionLockRecord::ForgetLock(ClientSeasonSession* pSession, OutDocServerObject* pSO)
{
	ForgetLock(pSession,LockObject_OutDoc,pSO->Object()->GetId());
}

void SessionLockRecord::ForgetLock(ClientSeasonSession* pSession, BuyInvoiceServerObject* pSO)
{
	ForgetLock(pSession,LockObject_BuyInvoice,pSO->Object()->GetId());
}

void SessionLockRecord::ForgetLock(ClientSeasonSession* pSession, SellInvoiceServerObject* pSO)
{
	ForgetLock(pSession,LockObject_SellInvoice,pSO->Object()->GetId());
}

void SessionLockRecord::RememberLock(ClientSeasonSession* pSession, LockObject objectType,uint32_t objectId)
{
	auto lockRegistryIt = m_SessionLocks.find(pSession->GetSessionToken());

	ObjectLockRegistry* pLockRegistry;
	if(lockRegistryIt != m_SessionLocks.end())
	{
		pLockRegistry = lockRegistryIt->second;
	}
	else
	{
		pLockRegistry = new ObjectLockRegistry();
		m_SessionLocks[pSession->GetSessionToken()] = pLockRegistry;
	}
	pLockRegistry->Remember(objectType,objectId);
}

void SessionLockRecord::ForgetLock(ClientSeasonSession* pSession, LockObject objectType,uint32_t objectId)
{
	auto lockRegistryIt = m_SessionLocks.find(pSession->GetSessionToken());
		
	if(lockRegistryIt != m_SessionLocks.end())
	{
		ObjectLockRegistry*  pLockRegistry = lockRegistryIt->second;
		pLockRegistry->Forget(objectType,objectId);
		if(pLockRegistry->LockCount() == 0)
		{
			delete pLockRegistry;
			m_SessionLocks.erase(lockRegistryIt);
		}
	}	
}
	
	
ObjectLock SessionLockRecord::PopLock(ClientSeasonSession* pSession)
{
	auto lockRegistryIt = m_SessionLocks.find(pSession->GetSessionToken());
	if(lockRegistryIt != m_SessionLocks.end())
	{
		ObjectLockRegistry* registry = lockRegistryIt->second;
		const ObjectLock* lock = registry->FirstLock();
		if(lock != NULL)
		{
			return *lock;
		}		
	}
	return ObjectLock();
}