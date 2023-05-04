#include "stdafx.h"
#include "NotificationSubscriber.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

NotificationSubsriber::NotificationSubsriber() : m_server(RCF::TcpEndpoint(-1)),m_pRelay(NULL)
{
	m_bridge.Create();
}

NotificationSubsriber::~NotificationSubsriber()
{
	m_bridge.Destroy();
}

void NotificationSubsriber::Subsrcibe(RCF::TcpEndpoint& endpoint,NotificationRelay* pRelay)
{
	m_server.start();
	m_subscription = m_server.createSubscription<I_NotificationPublish,NotificationSubsriber>(*this,endpoint);
	m_pRelay = pRelay;	
}

void NotificationSubsriber::Unsubscribe()
{	
	m_subscription->close();
	m_server.stop();
	m_pRelay = NULL;
	
}

void NotificationSubsriber::Notify(NotificationObj notificaion)
{
	if(m_pRelay)
	{
		m_bridge.InvokeAny(std::bind(&Notification::Relay,notificaion,m_pRelay));
		//notificaion->Relay(m_pRelay);
	}
}

