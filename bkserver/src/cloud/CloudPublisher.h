#pragma once

#include "GoogleServiceAccount.h"
#include "FirebaseTopicPublisher.h"
#include <service/notification/Notification.h>
#include <service/notification/NopNotificationRelay.h>

class CloudPublisher
{
	class HentDetectingNotificationRelay : public NopNotificationRelay
	{
	public:
		HentDetectingNotificationRelay();
		virtual void HentInserted(IHent* pHent);
		virtual void HentUpdated(IHent* pHent);
		virtual void HentDeleted(uint32_t hentId);
		
		bool IsGood() const;
	private:
		bool m_good;
	};

	class BuyInvoiceDetectingNotificationRelay : public NopNotificationRelay
	{
	public:
		BuyInvoiceDetectingNotificationRelay();
		virtual void BuyInvoiceInserted(IBuyInvoice* pInvoice);
		virtual void BuyInvoiceUpdated(IBuyInvoice* pInvoice);
		virtual void BuyInvoiceDeleted(uint32_t invoiceId);
	

		bool IsGood() const;
	private:
		bool m_good;
	};
public:
	CloudPublisher(const gcp::GoogleServiceAccount& account);	
	void Publish(const FarmNo& farmNo,uint32_t seasonId,const Notification* notification);
	
private:
	FirebaseTopicPublisher m_fcmPublisher;
};

