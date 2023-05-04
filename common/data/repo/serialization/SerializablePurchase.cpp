#include "stdafx.h"
#include "SerializablePurchase.h"
#include "../../types/serialization/TypesSerialization.h"
#include "SerializablePurchaseHent.h"
#include "SerializablePurchaseInvoice.h"


SerializablePurchase::SerializablePurchase(Purchase* purchase) : m_purchase(purchase)
{
}

std::wstring& SerializablePurchase::Id()
{
	return m_purchase->m_PuracheId;
}

std::wstring& SerializablePurchase::AgentCode()
{
	return m_purchase->m_AgentCode;
}

std::wstring& SerializablePurchase::PlateNo()
{
	return m_purchase->m_PlateNo;
}

DateTime& SerializablePurchase::StartDate()
{
	return m_purchase->m_StartDate;
}

DateTime& SerializablePurchase::EndDate()
{
	return m_purchase->m_EndDate;
}

bool&  SerializablePurchase::WasDownloaded()
{
	return m_purchase->m_Downloaded;
}


int& SerializablePurchase::HerdNumber()
{
	return m_purchase->m_Herd;
}
	
void SerializablePurchase::serialize(SF::Archive &archive)
{
	archive & Id();
	archive & AgentCode();
	archive & PlateNo();
	archive & StartDate();
	archive & EndDate();
	archive & WasDownloaded();
	archive & HerdNumber();

	count_t hentsCount, invoiceCount;
	if (archive.isRead())
	{
		archive & hentsCount;

		for (count_t i = 0; i < hentsCount; i++)
		{
			PurchaseHent* hent = new PurchaseHent(m_purchase->ObtainNewHentId());
			SerializablePurchaseHent(hent).serialize(archive);
			m_purchase->m_Hents.push_back(hent);
		}

		archive & invoiceCount;

		for (count_t i = 0; i < invoiceCount; i++)
		{
			PurchaseInvoice* invoice = new PurchaseInvoice(m_purchase->ObtainNewInvoiceId());
			SerializablePurchaseInvoice(invoice).serialize(archive);
			m_purchase->m_Invoices.push_back(invoice);
		}
		
	}
	else
	{
		hentsCount = m_purchase->m_Hents.size();
		archive & hentsCount;

		for (count_t i = 0; i < hentsCount; i++)
		{
			SerializablePurchaseHent(m_purchase->m_Hents[i]).serialize(archive);
		}
		
		invoiceCount = m_purchase->m_Invoices.size();
		archive & invoiceCount;
		for (count_t i = 0; i < invoiceCount; i++)
		{
			SerializablePurchaseInvoice(m_purchase->m_Invoices[i]).serialize(archive);
		}
	}
	
}