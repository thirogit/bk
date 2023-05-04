#include "stdafx.h"
#include "CloudPublisher.h"

#include "jwt/jwt_all.h"
#include "jwt/rsavalidator.h"
#include "private/clock.h"
#include <openssl/pem.h>
#include <private/ssl_compat.h>
#include <string/TextUtils.h>

CloudPublisher::HentDetectingNotificationRelay::HentDetectingNotificationRelay() : m_good(false)
{
	
}

void CloudPublisher::HentDetectingNotificationRelay::HentInserted(IHent* pHent)
{
	m_good = true;
}

void CloudPublisher::HentDetectingNotificationRelay::HentUpdated(IHent* pHent)
{
	m_good = true;
}
void CloudPublisher::HentDetectingNotificationRelay::HentDeleted(uint32_t hentId)
{
	m_good = true;
}

bool CloudPublisher::HentDetectingNotificationRelay::IsGood() const
{
	return m_good;
}


CloudPublisher::BuyInvoiceDetectingNotificationRelay::BuyInvoiceDetectingNotificationRelay() : m_good(false)
{

}

void CloudPublisher::BuyInvoiceDetectingNotificationRelay::BuyInvoiceInserted(IBuyInvoice* pInvoice)
{
	m_good = true;
}

void CloudPublisher::BuyInvoiceDetectingNotificationRelay::BuyInvoiceUpdated(IBuyInvoice* pInvoice)
{
	m_good = true;
}

void CloudPublisher::BuyInvoiceDetectingNotificationRelay::BuyInvoiceDeleted(uint32_t invoiceId)
{
	m_good = true;
}


bool CloudPublisher::BuyInvoiceDetectingNotificationRelay::IsGood() const
{
	return m_good;
}

//#################################################################################################

CloudPublisher::CloudPublisher(const gcp::GoogleServiceAccount& account) : m_fcmPublisher(account)
{
}

void CloudPublisher::Publish(const FarmNo& farmNo, uint32_t seasonId, const Notification* notification)
{
	static const wchar_t* seasonid_attribute_name = L"season_id";
	static const wchar_t* farmno_attribute_name = L"farm_no";
	static const wchar_t* notification_attribute_name = L"notification";

	HentDetectingNotificationRelay hentdetectingrelay;
	notification->Relay(&hentdetectingrelay);

	if (hentdetectingrelay.IsGood())
	{
		json::value message = notification->ToJson();		
		message[farmno_attribute_name] = json::value(farmNo.ToString());
		message[seasonid_attribute_name] = json::value(seasonId);
		message[notification_attribute_name] = notification->ToJson();
		m_fcmPublisher.PublishToTopic(TextUtils::ToUTF8(message.serialize()), "bkskup_hents");
	}

	BuyInvoiceDetectingNotificationRelay invoidedetectingrelay;
	notification->Relay(&invoidedetectingrelay);

	if (invoidedetectingrelay.IsGood())
	{
		json::value message = notification->ToJson();
		message[seasonid_attribute_name] = json::value(seasonId);
		message[notification_attribute_name] = notification->ToJson();
//		m_fcmPublisher.PublishToTopic(TextUtils::ToUTF8(message.serialize()), u(farmNo.ToString()) + ".buy_invoices");
	}



}

