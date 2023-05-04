#include "stdafx.h"
#include "SerializableDocDetails.h"
#include "../../types/serialization/TypesSerialization.h"


SerializableDocDetails::SerializableDocDetails(DocDetails* pDocDetails) : m_pDocDetails(pDocDetails)
{
}

DateTime& SerializableDocDetails::DocDate()
{
	return m_pDocDetails->m_DocDate;
}

std::wstring& SerializableDocDetails::Extras()
{
	return m_pDocDetails->m_Extras;
}

std::wstring& SerializableDocDetails::PlateNo()
{
	return m_pDocDetails->m_PlateNo;
}

Motive& SerializableDocDetails::MotiveOfDisplacement()
{
	return m_pDocDetails->m_Motive;
}

Time& SerializableDocDetails::LoadStartDtTm()
{
	return m_pDocDetails->m_LoadStartDtTm;
}

Time& SerializableDocDetails::LoadEndDtTm()
{
	return m_pDocDetails->m_LoadEndDtTm;
}

uint32_t& SerializableDocDetails::AgentId()
{
	return m_pDocDetails->m_AgentId;
}

DateTime& SerializableDocDetails::LoadDate()
{
	return m_pDocDetails->m_LoadDate;
}

void SerializableDocDetails::serialize(SF::Archive &archive)
{
	archive & DocDate();
	archive & LoadDate();
	archive & Extras();
	archive & PlateNo();
	archive & MotiveOfDisplacement();	
	archive & LoadStartDtTm();
	archive & LoadEndDtTm();	
	archive & AgentId();
}


