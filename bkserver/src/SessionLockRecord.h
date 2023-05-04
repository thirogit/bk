#pragma once

#include <stdint.h>
#include <map>
#include "object/HentServerObject.h"
#include "object/CowServerObject.h"
#include "object/InDocServerObject.h"
#include "object/BuyDocServerObject.h"
#include "object/MoveDocServerObject.h"
#include "object/SellDocServerObject.h"
#include "object/OutDocServerObject.h"
#include "object/BuyInvoiceServerObject.h"
#include "object/SellInvoiceServerObject.h"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/random_access_index.hpp>

class ClientSeasonSession;


enum LockObject
{
	LockObject_None = 0,
	LockObject_Hent,
	LockObject_Cow,
	LockObject_InDoc,
	LockObject_BuyDoc,
	LockObject_MoveDoc,
	LockObject_SellDoc,
	LockObject_OutDoc,
	LockObject_BuyInvoice,
	LockObject_SellInvoice
};

class ObjectLock
{
public:
	ObjectLock();
	ObjectLock(LockObject type,uint32_t id);
	ObjectLock(const ObjectLock& src);
	ObjectLock& operator=(const ObjectLock& right);

	LockObject GetType() const;
	uint32_t GetId() const;

private:
	LockObject m_type;
	uint32_t m_id;
};

class SessionLockRecord
{
	class ObjectLockRegistry
	{
		struct index_by_type{};

		typedef boost::multi_index_container  <
														  ObjectLock*,
														  boost::multi_index::indexed_by< 																							
																							boost::multi_index::ordered_non_unique < boost::multi_index::tag<index_by_type>, boost::multi_index::const_mem_fun<ObjectLock,LockObject, &ObjectLock::GetType> >										
																						>
													>
		_Registry;

		typedef boost::multi_index::index<_Registry,index_by_type>::type _IndexByType;


	public:
		~ObjectLockRegistry();
		void Remember(LockObject objectType,uint32_t objectId);
		void Forget(LockObject objectType,uint32_t objectId);
		size_t LockCount() const;
		const ObjectLock* FirstLock();
	private:
		_Registry m_Registry;		
	};

public:
	SessionLockRecord();
	~SessionLockRecord();
	
	void RememberLock(ClientSeasonSession* pSession, HentServerObject* pSO);
	void RememberLock(ClientSeasonSession* pSession, CowServerObject* pSO);
	void RememberLock(ClientSeasonSession* pSession, InDocServerObject* pSO);
	void RememberLock(ClientSeasonSession* pSession, BuyDocServerObject* pSO);
	void RememberLock(ClientSeasonSession* pSession, MoveDocServerObject* pSO);
	void RememberLock(ClientSeasonSession* pSession, SellDocServerObject* pSO);
	void RememberLock(ClientSeasonSession* pSession, OutDocServerObject* pSO);
	void RememberLock(ClientSeasonSession* pSession, BuyInvoiceServerObject* pSO);
	void RememberLock(ClientSeasonSession* pSession, SellInvoiceServerObject* pSO);

	void ForgetLock(ClientSeasonSession* pSession, HentServerObject* pSO);
	void ForgetLock(ClientSeasonSession* pSession, CowServerObject* pSO);
	void ForgetLock(ClientSeasonSession* pSession, InDocServerObject* pSO);
	void ForgetLock(ClientSeasonSession* pSession, BuyDocServerObject* pSO);
	void ForgetLock(ClientSeasonSession* pSession, MoveDocServerObject* pSO);
	void ForgetLock(ClientSeasonSession* pSession, SellDocServerObject* pSO);
	void ForgetLock(ClientSeasonSession* pSession, OutDocServerObject* pSO);
	void ForgetLock(ClientSeasonSession* pSession, BuyInvoiceServerObject* pSO);
	void ForgetLock(ClientSeasonSession* pSession, SellInvoiceServerObject* pSO);

	ObjectLock PopLock(ClientSeasonSession* pSession);

private:
	void RememberLock(ClientSeasonSession* pSession, LockObject objectType,uint32_t objectId);
	void ForgetLock(ClientSeasonSession* pSession, LockObject objectType,uint32_t objectId);
	
	
	
private:
	std::map<uint32_t, ObjectLockRegistry*> m_SessionLocks;

};
