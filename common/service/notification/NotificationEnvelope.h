#pragma once

#include <SF/Archive.hpp>
#include <arrays/Enumerator.h>
#include "Notification.h"

class NotificationEnvelope : public Notification
{
public:
	NotificationEnvelope();
	~NotificationEnvelope();

	virtual Notification* Clone();
	virtual void Relay(NotificationRelay* pRelay) const;

	virtual void serialize(SF::Archive &archive);
	void Insert(Notification* pNotifcation);
	PtrEnumerator<Notification> EnumNotifications();
	void Empty();
	bool IsEmpty() const;

	virtual json::value ToJson() const;
private:
	std::vector<Notification*> m_Notifications;
};
