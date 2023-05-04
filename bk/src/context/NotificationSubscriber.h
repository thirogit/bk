#ifndef __NOTIFICATIONSUBSCRIBER_H__
#define __NOTIFICATIONSUBSCRIBER_H__

#include <RCF\RCF.hpp>
#include <service/notification/NotificationRelay.h>
#include <service/INotificationPublish.h>
#include "UIBridge.h"

class NotificationSubsriber
{
public:
	NotificationSubsriber();
	~NotificationSubsriber();
	void Subsrcibe(RCF::TcpEndpoint& endpoint,NotificationRelay* pRelay);
	void Unsubscribe();
	void Notify(NotificationObj notificaion);	
private:		

	RCF::RcfServer m_server;
	RCF::SubscriptionServicePtr m_service;
	RCF::SubscriptionPtr m_subscription;
	NotificationRelay* m_pRelay;
	UIBridge m_bridge;
};


#endif