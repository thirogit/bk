#include "stdafx.h"
#include "PurchaseHeaderSerializer.h"
#include "../../types/serialization/TypesSerialization.h"

PurchaseHeaderSerializer::PurchaseHeaderSerializer(PurchaseHeader* header) : m_header(header)
{
}

std::wstring& PurchaseHeaderSerializer::Id()
{
	return m_header->m_PuracheId;
}

std::wstring& PurchaseHeaderSerializer::AgentCode()
{
	return m_header->m_AgentCode;
}

std::wstring& PurchaseHeaderSerializer::PlateNo()
{
	return m_header->m_PlateNo;
}

DateTime& PurchaseHeaderSerializer::StartDate()
{
	return m_header->m_StartDate;
}

DateTime& PurchaseHeaderSerializer::EndDate()
{
	return m_header->m_EndDate;
}

bool&  PurchaseHeaderSerializer::WasDownloaded()
{
	return m_header->m_Downloaded;
}


int& PurchaseHeaderSerializer::HerdNumber()
{
	return m_header->m_Herd;
}

count_t& PurchaseHeaderSerializer::CowCount()
{
	return m_header->m_CowCount;
}
	
void PurchaseHeaderSerializer::serialize(SF::Archive &archive)
{
	archive & Id();
	archive & AgentCode();
	archive & PlateNo();
	archive & StartDate();
	archive & EndDate();
	archive & WasDownloaded();
	archive & HerdNumber();
	archive & CowCount();
	
}