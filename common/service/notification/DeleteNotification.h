#pragma once

#include <SF/Archive.hpp>
#include "Notification.h"

template<class TAG,void (NotificationRelay::*relayFn)(uint32_t)>
class DeleteNotification : public Notification
{
public:
	DeleteNotification() : m_Id(NULL_ID)
	{
	}
	
	DeleteNotification(uint32_t id) : m_Id(id)
	{
	}
	
	DeleteNotification& WithId(uint32_t id)
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
		return new DeleteNotification<TAG,relayFn>(m_Id);
	}
	
	virtual json::value ToJson() const
	{
		return json::value::null();
	}

private:
	uint32_t m_Id;	
};

