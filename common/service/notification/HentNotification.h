#pragma once

#include "Notification.h"
#include "../../data/impl/serialization/SerializableHent.h"

namespace HentNotificationJsonMembers
{
	static const wchar_t* hent_attribute_name = L"hent";
}


template<void (NotificationRelay::*relayFn)(IHent*), class ntfnType>
class HentNotification : public Notification
{
public:
	HentNotification() : m_pHent(NULL)
	{
		m_type = ntfnType::data();
	}
	
	HentNotification(const IHent* pHent/* = NULL*/) : HentNotification()
	{
		if(pHent)
			CreateHent(pHent->GetId())->CopyFrom(pHent);
	}
	
	virtual ~HentNotification()
	{
		if(m_pHent)
			delete m_pHent;
	}
	
	Hent* CreateHent(uint32_t hentId)
	{
		if(!m_pHent)
		{
			m_pHent = new Hent(hentId);
			return m_pHent;
		}
		return NULL;		
	}
	
	const IHent* GetHent() const
	{
		return m_pHent;
	}
	virtual void serialize(SF::Archive &archive)
	{
		if(archive.isRead())
		{
			CreateHent(NULL_ID);
		}
		SerializableHent(m_pHent).serialize(archive);
	}
	
	
	virtual Notification* Clone()
	{
		HentNotification<relayFn, ntfnType>* pClone = new HentNotification<relayFn, ntfnType>();
		const IHent* pSrcHent = GetHent();
		if(pSrcHent)
		{
			Hent* pClonedHent = pClone->CreateHent(pSrcHent->GetId());
			pClonedHent->CopyFrom(pSrcHent);		
		}
		return pClone;	
	}

	virtual void Relay(NotificationRelay* pRelay) const
	{
		if(m_pHent)
			(pRelay->*relayFn)(m_pHent);
	}

	virtual json::value ToJson() const
	{
		json::value result;
		result[NotificationJsonMembers::_type_attribute_name] = json::value(m_type);
		result[HentNotificationJsonMembers::hent_attribute_name] = m_pHent->ToJson();
		return result;
	}



	const std::wstring& GetType()
	{
		return m_type;
	}

private:
	std::wstring m_type;
	Hent* m_pHent;
};

