#include "stdafx.h"
#include "FetchPurchasesResponse.h"
#include "../../data/repo/serialization/SerializablePurchase.h"

FetchPurchasesResponse::FetchPurchasesResponse()
{
}

FetchPurchasesResponse::~FetchPurchasesResponse()
{
	m_Purchases.FreeAndFlush();
}	

Purchase* FetchPurchasesResponse::Add(const std::wstring& purchaseId)
{
	Purchase* pNew = new Purchase(purchaseId);
	m_Purchases.Add(pNew);
	return pNew;
}
		
PtrEnumerator<IPurchase> FetchPurchasesResponse::Enum()
{
	return PtrEnumerator<IPurchase>(m_Purchases);
}

count_t FetchPurchasesResponse::GetCount() const
{
	return m_Purchases.size();
}

void FetchPurchasesResponse::serialize(SF::Archive &archive)
{
	count_t count;

	if(archive.isRead())
	{
		archive & count;
		for (count_t i = 0; i < count; i++)
		{
			SerializablePurchase(Add(NULL_ID)).serialize(archive);
		}
	}	
	else
	{
		count = m_Purchases.size();
		archive & count;
		for (count_t i = 0; i < count; i++)
		{
			SerializablePurchase(m_Purchases[i]).serialize(archive);
		}
	}
}
