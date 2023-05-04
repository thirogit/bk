#include "stdafx.h"
#include "PurchaseHeader.h"
#include <algorithm>
#include <arrays/DeepDelete.h>

PurchaseHeader::PurchaseHeader(const std::wstring& id) : m_PuracheId(id), m_Downloaded(false), m_Herd(0), m_CowCount(0)
{
}



PurchaseHeader::~PurchaseHeader()
{	
}

PurchaseHeader::PurchaseHeader(const IPurchase* src)
{
	m_PuracheId = src->GetId();
	CopyFrom(src);
}

PurchaseHeader::PurchaseHeader(const IPurchaseHeader* src)
{
	m_PuracheId = src->GetId();
	CopyFrom(src);
}

PurchaseHeader& PurchaseHeader::operator=(const PurchaseHeader& right)
{
	m_PuracheId = right.m_PuracheId;
	CopyFrom(right);
	return *this;
}

void PurchaseHeader::CopyFrom(const IPurchase* src)
{	
	m_AgentCode = src->GetAgentCode();
	m_PlateNo = src->GetPlateNo();
	m_StartDate = src->GetStartDate();
	m_EndDate = src->GetEndDate();
	m_Herd = src->GetHerd();

	m_CowCount = 0;	
	PtrEnumerator<IPurchaseInvoice> enumInvoices = src->EnumInvoices();
	while (enumInvoices.hasNext())
	{
		m_CowCount += (*enumInvoices)->GetCowCount();
		enumInvoices.advance();
	}
	m_Downloaded = src->GetWasDownloaded();
}

void PurchaseHeader::CopyFrom(const IPurchaseHeader* src)
{
	m_AgentCode = src->GetAgentCode();
	m_PlateNo = src->GetPlateNo();
	m_StartDate = src->GetStartDate();
	m_EndDate = src->GetEndDate();
	m_Herd = src->GetHerd();
	m_CowCount = src->GetCowCount();
	m_Downloaded = src->GetWasDownloaded();
}

PurchaseHeader::PurchaseHeader(const PurchaseHeader& src)
{
	m_PuracheId = src.m_PuracheId;
	CopyFrom(src);
}


void PurchaseHeader::CopyFrom(const PurchaseHeader& src)
{
	CopyFrom(&src);
}

const std::wstring& PurchaseHeader::GetId() const
{
	return m_PuracheId;
}

int PurchaseHeader::GetHerd() const
{
	return m_Herd;
}

const std::wstring& PurchaseHeader::GetAgentCode() const
{
	return m_AgentCode;
}
const std::wstring& PurchaseHeader::GetPlateNo() const
{
	return m_PlateNo;
}

const DateTime& PurchaseHeader::GetStartDate() const
{
	return m_StartDate;
}

const DateTime& PurchaseHeader::GetEndDate() const
{
	return m_EndDate;
}

void PurchaseHeader::SetAgentCode(const std::wstring& sAgentCode)
{
	m_AgentCode = sAgentCode;
}

void PurchaseHeader::SetPlateNo(const std::wstring& sPlateNo)
{
	m_PlateNo = sPlateNo;
}

void PurchaseHeader::SetStartDate(const DateTime& startDt)
{
	m_StartDate = startDt;
}

void PurchaseHeader::SetEndDate(const DateTime& endDt)
{
	m_EndDate = endDt;
}

bool PurchaseHeader::GetWasDownloaded() const
{
	return m_Downloaded;
}

void PurchaseHeader::SetWasDownloaded(bool bDownloaded)
{
	m_Downloaded = bDownloaded;
}

void PurchaseHeader::SetHerd(int herd)
{
	m_Herd = herd;
}

void PurchaseHeader::SetCowCount(count_t cowCount)
{
	m_CowCount = cowCount;
}

count_t PurchaseHeader::GetCowCount() const
{
	return m_CowCount;
}