#include "stdafx.h"
#include "FetchPurchaseResponse.h"
#include "../../data/repo/serialization/SerializablePurchase.h"

FetchPurchaseResponse::FetchPurchaseResponse() : m_Purchase(NULL)
{
}

FetchPurchaseResponse::~FetchPurchaseResponse()
{
	delete m_Purchase;
}	

void FetchPurchaseResponse::Set(IPurchase* purchase)
{
	delete m_Purchase;
	m_Purchase = new Purchase(purchase);
	m_Purchase->CopyFrom(purchase);
}
		

IPurchase* FetchPurchaseResponse::Get()
{
	return m_Purchase;
}

void FetchPurchaseResponse::serialize(SF::Archive &archive)
{
	count_t empty;

	if(archive.isRead())
	{		
		archive & empty;
		if (empty != 0)
		{
			if (m_Purchase == NULL)
			{
				m_Purchase = new Purchase();
			}
			SerializablePurchase(m_Purchase).serialize(archive);
		}
	}	
	else
	{
		empty = (m_Purchase == NULL) ? 0 : 1;
		archive & empty;
		if (empty != 0)
		{
			SerializablePurchase(m_Purchase).serialize(archive);
		}
	}
}
