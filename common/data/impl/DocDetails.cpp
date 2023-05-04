#include "stdafx.h"
#include "DocDetails.h"
#include "../IIdable.h"

DocDetails::DocDetails() : m_Motive(Motive_None),m_AgentId(NULL_ID)
{
}

void DocDetails::CopyFrom(const DocDetails& src)
{
	m_LoadDate = src.m_LoadDate;
	m_DocDate = src.m_DocDate;
	m_Extras = src.m_Extras;
	m_PlateNo = src.m_PlateNo;
	m_Motive = src.m_Motive;	
	m_LoadStartDtTm = src.m_LoadStartDtTm;
	m_LoadEndDtTm = src.m_LoadEndDtTm;	
	m_AgentId = src.m_AgentId;	
}

void DocDetails::CopyFrom(const IDoc* pSrc)
{
	m_LoadDate = pSrc->GetLoadDate();
	m_DocDate = pSrc->GetDocDate();
	m_Extras = pSrc->GetExtras();
	m_PlateNo = pSrc->GetPlateNo();
	m_Motive = pSrc->GetMotive();	
	m_LoadStartDtTm = pSrc->GetLoadStartDtTm();
	m_LoadEndDtTm = pSrc->GetLoadEndDtTm();	
	m_AgentId = pSrc->GetAgentId();	
}

const DateTime& DocDetails::GetLoadDate() const
{
	return m_LoadDate;
}

void DocDetails::SetLoadDate(const DateTime& loadDate)
{
	m_LoadDate = loadDate;
}

const DateTime& DocDetails::GetDocDate() const
{
	return m_DocDate;
}

const std::wstring& DocDetails::GetExtras() const
{
	return m_Extras;
}

const std::wstring& DocDetails::GetPlateNo() const
{
	return m_PlateNo;
}

Motive DocDetails::GetMotive() const
{
	return m_Motive;
}

const Time& DocDetails::GetLoadStartDtTm() const
{
	return m_LoadStartDtTm;
}

const Time& DocDetails::GetLoadEndDtTm() const
{
	return m_LoadEndDtTm;
}

uint32_t DocDetails::GetAgentId() const
{
	return m_AgentId;
}

void DocDetails::SetDocDate(const DateTime& docDate)
{
	m_DocDate = docDate;
}

void DocDetails::SetExtras(const std::wstring& sExtras)
{
	m_Extras = sExtras;
}

void DocDetails::SetPlateNo(const std::wstring& sPlateNo)
{
	m_PlateNo = sPlateNo;
}

void DocDetails::SetMotive(Motive motive)
{
	m_Motive = motive;
}

void DocDetails::SetLoadStartDtTm(const Time& loadStartDtTm)
{
	m_LoadStartDtTm = loadStartDtTm;
}

void DocDetails::SetLoadEndDtTm(const Time& loadEndDtTm)
{
	m_LoadEndDtTm = loadEndDtTm;
}

void DocDetails::SetAgentId(uint32_t agentId)
{
	m_AgentId = agentId;
}
