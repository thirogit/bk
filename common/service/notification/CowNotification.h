#pragma once

#include "Notification.h"
#include <data/impl/Cow.h>
#include "../../data/impl/serialization/SerializableCow.h"

template<void (NotificationRelay::*relayFn)(ICow*)>
class CowNotification : public Notification
{
public:
	CowNotification(const ICow* pCow = NULL) : m_pCow(NULL)
	{
		if(pCow)
			CreateCow(pCow->GetId())->CopyFrom(pCow);
	}

	~CowNotification()
	{
		if(m_pCow)
			delete m_pCow;
	}

	virtual Notification* Clone()
	{
		CowNotification<relayFn>* pClone = new CowNotification<relayFn>();
		const ICow* pCow = GetCow();
		if(pCow)
		{
			Cow* pClonedCow = pClone->CreateCow(pCow->GetId());
			pClonedCow->CopyFrom(pCow);		
		}
		return pClone;	
	}

	virtual json::value ToJson() const
	{
		return json::value::null();
	}

	virtual void Relay(NotificationRelay* pRelay) const
	{
		if(m_pCow)
			(pRelay->*relayFn)(m_pCow);
	}

	
	Cow* CreateCow(uint32_t id)
	{
		if(!m_pCow)
			m_pCow = new Cow(id);
		return m_pCow;
	}

	const ICow* GetCow() const
	{
		return m_pCow;
	}

	virtual void serialize(SF::Archive &archive)
	{
		if(archive.isRead())
			CreateCow(NULL_ID);

		SerializableCow(m_pCow).serialize(archive);
	}

private:
	Cow* m_pCow;
};
