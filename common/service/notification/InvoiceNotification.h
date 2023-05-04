#pragma once

#include "Notification.h"

namespace InvoiceNotificationJsonMemebers
{
	static const wchar_t* invoice_attribute_name = L"invoice";
}

template<class INVOICETYPE,class SERIALIZER,void (NotificationRelay::*relayFn)(typename INVOICETYPE::interface_type*), class ntfnType>
class InvoiceNotification : public Notification
{
public:
	InvoiceNotification(const typename INVOICETYPE::interface_type* pInvoice = NULL) : m_pInvoice(NULL)
	{
		if(pInvoice)
			CreateInvoice(pInvoice->GetId(),pInvoice->GetInvoiceScope())->CopyFrom(pInvoice);

		m_type = ntfnType::data();
	}
	~InvoiceNotification()
	{
		if(m_pInvoice)
			delete m_pInvoice;
	}
	
	virtual Notification* Clone()
	{
		auto pClone = new InvoiceNotification<INVOICETYPE,SERIALIZER,relayFn, ntfnType>();
		if(m_pInvoice)
		{
			INVOICETYPE* pClonedDoc = pClone->CreateInvoice(m_pInvoice->GetId(),m_pInvoice->GetInvoiceScope());
			pClonedDoc->CopyFrom(m_pInvoice);		
		}
		return pClone;
	}
	
	virtual void Relay(NotificationRelay* pRelay) const
	{
		if(m_pInvoice)
			(pRelay->*relayFn)(m_pInvoice);
	}
	
	virtual void serialize(SF::Archive &archive)
	{
		if(archive.isRead())
			CreateInvoice(NULL_ID,InvoiceScope_None);

		SERIALIZER(m_pInvoice).serialize(archive);
	}
	
	INVOICETYPE* CreateInvoice(uint32_t id,InvoiceScope scope)
	{
		if(!m_pInvoice)
			m_pInvoice = new INVOICETYPE(id,scope);
		return m_pInvoice;
	}	
	const typename INVOICETYPE::interface_type* GetInvoice() const
	{
		return m_pInvoice;
	}

	virtual json::value ToJson() const
	{
		json::value result;
		result[NotificationJsonMembers::_type_attribute_name] = json::value(m_type);
		result[InvoiceNotificationJsonMemebers::invoice_attribute_name] = m_pInvoice->ToJson();
		return result;
	}

private:
	std::wstring m_type;
	INVOICETYPE* m_pInvoice;
};
