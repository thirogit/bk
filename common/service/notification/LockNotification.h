#pragma once

#include <SF/Archive.hpp>
#include "Notification.h"
#include <stdint.h>

template<void (NotificationRelay::*relayFn)(uint32_t)>
class LockNotification : public Notification
{
public:
	LockNotification() : m_Id(NULL_ID),m_UserId(NULL_ID)
	{
	}
	
	LockNotification(uint32_t id,uint32_t userId) : m_Id(id), m_UserId(userId)
	{
	}
	
	LockNotification& WithId(uint32_t id)
	{
		m_Id = id;
		return *this;
	}
	
	LockNotification& WithUserId(uint32_t userId)
	{
		m_UserId = userId;
		return *this;
	}
	
	uint32_t GetId() const
	{
		return m_Id;
	}
	
	uint32_t GetUserId() const
	{
		return m_UserId;
	}
	
	void serialize(SF::Archive &archive)
	{
		archive & m_Id;
		archive & m_UserId;
	}

	virtual void Relay(NotificationRelay* pRelay) const
	{
		(pRelay->*relayFn)(m_Id);
	}

	virtual Notification* Clone()
	{
		return new LockNotification<relayFn>(m_Id,m_UserId);
	}

	virtual json::value ToJson() const
	{
		return json::value::null();
	}


	
private:
	uint32_t m_Id;
	uint32_t m_UserId;
};

