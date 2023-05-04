#include "stdafx.h"
#include "FetchPurchaseHeadersResponse.h"
#include "../../data/repo/serialization/PurchaseHeaderSerializer.h"

FetchPurchaseHeadersResponse::FetchPurchaseHeadersResponse()
{
}

FetchPurchaseHeadersResponse::~FetchPurchaseHeadersResponse()
{
	m_PurchaseHeaders.FreeAndFlush();
}	

PurchaseHeader* FetchPurchaseHeadersResponse::Add(const std::wstring& purchaseId)
{
	PurchaseHeader* pNew = new PurchaseHeader(purchaseId);
	m_PurchaseHeaders.Add(pNew);
	return pNew;
}
		
PtrEnumerator<IPurchaseHeader> FetchPurchaseHeadersResponse::Enum()
{
	return PtrEnumerator<IPurchaseHeader>(m_PurchaseHeaders);
}

count_t FetchPurchaseHeadersResponse::GetCount() const
{
	return m_PurchaseHeaders.size();
}

void FetchPurchaseHeadersResponse::serialize(SF::Archive &archive)
{
	count_t count;

	if(archive.isRead())
	{
		archive & count;
		for (count_t i = 0; i < count; i++)
		{
			PurchaseHeaderSerializer(Add(L"")).serialize(archive);
		}
	}	
	else
	{
		count = m_PurchaseHeaders.size();
		archive & count;
		for (count_t i = 0; i < count; i++)
		{
			PurchaseHeaderSerializer(m_PurchaseHeaders[i]).serialize(archive);
		}
	}
}
