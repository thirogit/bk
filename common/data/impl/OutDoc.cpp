#include "stdafx.h"
#include "OutDoc.h"

OutDoc::OutDoc(uint32_t docId) : m_DocId(docId),m_herdId(NULL_ID)
{
}


uint32_t OutDoc::GetId() const
{
	return m_DocId;
}	

DocType OutDoc::GetDocType() const
{
	return DocType_Out;
}

void OutDoc::CopyFrom(IOutDoc* pSrc)
{
	m_herdId = pSrc->GetHerdId();		
	
	SetDetails(pSrc);

	m_Content.RemoveAll();
	for(int i = 0,count = pSrc->GetCowCount();i < count;i++)
	{
		ICowEntry* pSrcEntry = pSrc->GetCowAt(i);
		m_Content.AddCow(pSrcEntry->GetCowId())->CopyFrom(pSrcEntry);
	}
	
}

void OutDoc::SetDetails(IDoc* pSrc)
{
	m_Details.SetDocDate(pSrc->GetDocDate());
	m_Details.SetExtras(pSrc->GetExtras());
	m_Details.SetPlateNo(pSrc->GetPlateNo());
	m_Details.SetMotive(pSrc->GetMotive());	
	m_Details.SetLoadStartDtTm(pSrc->GetLoadStartDtTm());
	m_Details.SetLoadEndDtTm(pSrc->GetLoadEndDtTm());	
	m_Details.SetAgentId(pSrc->GetAgentId());
	m_Details.SetLoadDate(pSrc->GetLoadDate());
}

void OutDoc::SetLoadDate(const DateTime& loadDate)
{
	m_Details.SetLoadDate(loadDate);
}

const DateTime& OutDoc::GetLoadDate() const
{
	return m_Details.GetLoadDate();
}


const DateTime& OutDoc::GetDocDate() const
{
	return m_Details.GetDocDate();
}

const std::wstring& OutDoc::GetExtras() const
{
	return m_Details.GetExtras();
}

const std::wstring& OutDoc::GetPlateNo() const
{
	return m_Details.GetPlateNo();
}

Motive OutDoc::GetMotive() const
{
	return m_Details.GetMotive();
}

const Time& OutDoc::GetLoadStartDtTm() const
{
	return m_Details.GetLoadStartDtTm();
}

const Time& OutDoc::GetLoadEndDtTm() const
{
	return m_Details.GetLoadEndDtTm();
}

uint32_t OutDoc::GetAgentId() const
{
	return m_Details.GetAgentId();
}

int OutDoc::GetCowCount() const
{
	return m_Content.GetCowCount();
}

PtrEnumerator<ICowEntry> OutDoc::EnumCows() const
{
	return m_Content.EnumCows();
}

ICowEntry* OutDoc::GetCow(uint32_t cowId) const
{
	return m_Content.GetCow(cowId);
}

ICowEntry*	OutDoc::GetCowAt(int index) const
{
	return m_Content.GetCowAt(index);
}

uint32_t OutDoc::GetHerdId() const
{
	return m_herdId;
}


void OutDoc::SetDocDate(const DateTime& docDate)
{
	m_Details.SetDocDate(docDate);
}

void OutDoc::SetExtras(const std::wstring& sExtras)
{
	m_Details.SetExtras(sExtras);
}

void OutDoc::SetPlateNo(const std::wstring& sPlateNo)
{
	m_Details.SetPlateNo(sPlateNo);
}

void OutDoc::SetMotive(Motive motive)
{
	m_Details.SetMotive(motive);
}

void OutDoc::SetLoadStartDtTm(const Time& loadStartTm)
{
	m_Details.SetLoadStartDtTm(loadStartTm);
}

void OutDoc::SetLoadEndDtTm(const Time& loadEndTm)
{
	m_Details.SetLoadEndDtTm(loadEndTm);
}

void OutDoc::SetAgentId(uint32_t agentId)
{
	m_Details.SetAgentId(agentId);
}

void OutDoc::SetHerdId(uint32_t	herdId)
{
	m_herdId = herdId;
}

void OutDoc::RemoveAt(int index)
{
	m_Content.RemoveAt(index);
}

void OutDoc::RemoveAll()
{
	m_Content.RemoveAll();
}

CowEntry* OutDoc::AddCow(uint32_t cowId)
{
	return m_Content.AddCow(cowId);
}

void OutDoc::AddCow(CowEntry* pCow)
{
	m_Content.AddCow(pCow);
}

void OutDoc::SetDetails(const DocDetails& details)
{
	m_Details.CopyFrom(details);
}

void OutDoc::RemoveEntry(uint32_t cowId)
{
	m_Content.RemoveCow(cowId);
}

CowEntry* OutDoc::GetEntry(uint32_t cowId)
{
	return m_Content.GetCow(cowId);
}