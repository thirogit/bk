#include "stdafx.h"
#include "DocView.h"

DocView::DocView(uint32_t docId) : m_DocId(docId),m_Motive(Motive_None)
{
}

uint32_t  DocView::GetDocId() const
{
	return m_DocId;
}

const std::wstring& DocView::GetDocNo() const
{
	return m_DocNo;
}

const DateTime& DocView::GetDocDate() const
{
	return m_DocDate;
}

const DateTime& DocView::GetLoadDate() const
{
	return m_LoadDate;
}

const std::wstring& DocView::GetExtras() const
{
	return m_Extras;
}

const std::wstring& DocView::GetPlateNo() const
{
	return m_PlateNo;
}

Motive DocView::GetMotive() const
{
	return m_Motive;
}
	
const Time& DocView::GetLoadStartDtTm() const
{
	return m_LoadStartDtTm;
}

const Time& DocView::GetLoadEndDtTm() const
{
	return m_LoadEndDtTm;
}
	
const std::wstring& DocView::GetAgentCd() const
{
	return m_AgentCd;
}

//const std::string& DocView::GetHerdCd() const
//{
//	return m_HerdCd;
//}

size_t DocView::GetCowCount() const
{
	return m_CowCount;
}	
		
void DocView::SetDocDate(const DateTime& docDate)
{
	m_DocDate = docDate;
}

void DocView::SetLoadDate(const DateTime& loadDate)
{
	m_LoadDate = loadDate;
}

void DocView::SetExtras(const std::wstring& sExtras)
{
	m_Extras = sExtras;
}

void DocView::SetPlateNo(const std::wstring& sPlateNo)
{
	m_PlateNo = sPlateNo;
}

void DocView::SetMotive(Motive motive)
{
	m_Motive = motive;
}

void DocView::SetDocNo(const std::wstring& sDocNo)
{
	m_DocNo = sDocNo;
}
	
void DocView::SetLoadStartDtTm(const Time& loadStartTm)
{
	m_LoadStartDtTm = loadStartTm;
}

void DocView::SetLoadEndDtTm(const Time& loadEndTm)
{
	m_LoadEndDtTm = loadEndTm;
}
	
void DocView::SetAgentCd(const std::wstring& sAgentCd)
{
	m_AgentCd = sAgentCd;
}
	
//void DocView::SetHerdCd(const std::string& sHerdCd)
//{
//	m_HerdCd = sHerdCd;
//}

void DocView::SetCowCount(size_t cowCount)
{
	m_CowCount = cowCount;
}

