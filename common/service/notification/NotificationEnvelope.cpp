#include "stdafx.h"
#include "NotificationEnvelope.h"
#include <SF/vector.hpp>
#include <boost/foreach.hpp>

NotificationEnvelope::NotificationEnvelope()
{
}

NotificationEnvelope::~NotificationEnvelope()
{
	Empty();
}

void NotificationEnvelope::serialize(SF::Archive &archive)
{
	archive & m_Notifications;
}

void NotificationEnvelope::Insert(Notification* pNotifcation)
{
	m_Notifications.push_back(pNotifcation->Clone());
}

PtrEnumerator<Notification> NotificationEnvelope::EnumNotifications()
{
	return PtrEnumerator<Notification>(m_Notifications);
}

void NotificationEnvelope::Empty()
{
	std::for_each(m_Notifications.begin(),m_Notifications.end(),[](Notification* pNotifcation)
																{
																	delete pNotifcation;
																});
};

Notification* NotificationEnvelope::Clone()
{
	NotificationEnvelope* pClone = new NotificationEnvelope();
	PtrEnumerator<Notification> ntfnEnum = EnumNotifications();
	while(ntfnEnum.hasNext())
	{
		pClone->Insert(*ntfnEnum);
		ntfnEnum.advance();
	}
	return pClone;
}

void NotificationEnvelope::Relay(NotificationRelay* pRelay) const
{
	std::for_each(m_Notifications.begin(),m_Notifications.end(),[pRelay](Notification* pNotifcation)
																{
																	pNotifcation->Relay(pRelay);
																});
}

bool NotificationEnvelope::IsEmpty() const
{
	return m_Notifications.empty();
}


json::value NotificationEnvelope::ToJson() const
{
	std::vector<json::value> jsonNotifications;

	BOOST_FOREACH(Notification* notification, m_Notifications)
	{
		jsonNotifications.push_back(notification->ToJson());
	}

	return json::value::array(jsonNotifications);
}