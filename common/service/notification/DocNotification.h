#pragma once

#include "Notification.h"

template<class DOCTYPE,class SERIALIZER,void (NotificationRelay::*relayFn)(typename DOCTYPE::interface_type*)>
class DocNotification : public Notification
{
public:
	DocNotification(typename DOCTYPE::interface_type* pDoc = NULL) : m_pDoc(NULL)
	{
		if(pDoc)
			CreateDoc(pDoc->GetId())->CopyFrom(pDoc);
	}
	~DocNotification()
	{
		if(m_pDoc)
			delete m_pDoc;
	}
	
	virtual Notification* Clone()
	{
		auto pClone = new DocNotification<DOCTYPE,SERIALIZER,relayFn>();
		auto pDoc = GetDoc();
		if(pDoc)
		{
			DOCTYPE* pClonedDoc = pClone->CreateDoc(pDoc->GetId());
			pClonedDoc->CopyFrom(pDoc);		
		}
		return pClone;
	}
	
	virtual void Relay(NotificationRelay* pRelay) const
	{
		if(m_pDoc)
			(pRelay->*relayFn)(m_pDoc);
	}
	
	virtual void serialize(SF::Archive &archive)
	{
		if(archive.isRead())
			CreateDoc(NULL_ID);

		SERIALIZER(m_pDoc).serialize(archive);
	}
	
	DOCTYPE* CreateDoc(uint32_t id)
	{
		if(!m_pDoc)
			m_pDoc = new DOCTYPE(id);
		return m_pDoc;
	}	
	typename DOCTYPE::interface_type* GetDoc() const
	{
		return m_pDoc;
	}

	virtual json::value ToJson() const
	{
		return json::value::null();
	}

private:
	DOCTYPE* m_pDoc;
};
