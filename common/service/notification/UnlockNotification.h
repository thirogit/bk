#pragma once

#include <SF/Archive.hpp>
#include "Notification.h"
#include "../../data/IIdable.h"

template<void (NotificationRelay::*relayFn)(uint32_t)>
class UnlockNotification : public Notification
{
public:
	
	UnlockNotification() : m_Id(NULL_ID)
	{
	}

	UnlockNotification(uint32_t id) : m_Id(id)
	{
	}

	UnlockNotification& WithId(uint32_t id)
	{
		m_Id = id;
		return *this;
	}

	uint32_t GetId() const
	{
		return m_Id;
	}

	void serialize(SF::Archive &archive)
	{
		archive & m_Id;	
	}
	
	virtual void Relay(NotificationRelay* pRelay) const
	{
		(pRelay->*relayFn)(m_Id);
	}

	virtual Notification* Clone()
	{
		return new UnlockNotification<relayFn>(m_Id);
	}


	virtual json::value ToJson() const
	{
		return json::value::null();
	}

private:
	uint32_t m_Id;	
};

