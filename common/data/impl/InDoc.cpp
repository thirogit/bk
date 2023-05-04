#include "stdafx.h"
#include "InDoc.h"


InDoc::InDoc(uint32_t docId) : m_DocId(docId),m_herdId(NULL_ID)
{
}


uint32_t InDoc::GetId() const
{
	return m_DocId;
}	

DocType InDoc::GetDocType() const
{
	return DocType_In;
}

void InDoc::CopyFrom(IInDoc* pSrc)
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

void InDoc::SetDetails(IDoc* pSrc)
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


void InDoc::SetLoadDate(const DateTime& loadDate)
{
	m_Details.SetLoadDate(loadDate);
}

const DateTime& InDoc::GetLoadDate() const
{
	return m_Details.GetLoadDate();
}


const DateTime& InDoc::GetDocDate() const
{
	return m_Details.GetDocDate();
}

const std::wstring& InDoc::GetExtras() const
{
	return m_Details.GetExtras();
}

const std::wstring& InDoc::GetPlateNo() const
{
	return m_Details.GetPlateNo();
}

Motive InDoc::GetMotive() const
{
	return m_Details.GetMotive();
}

const Time& InDoc::GetLoadStartDtTm() const
{
	return m_Details.GetLoadStartDtTm();
}

const Time& InDoc::GetLoadEndDtTm() const
{
	return m_Details.GetLoadEndDtTm();
}

uint32_t InDoc::GetAgentId() const
{
	return m_Details.GetAgentId();
}

int InDoc::GetCowCount() const
{
	return m_Content.GetCowCount();
}

PtrEnumerator<ICowEntry> InDoc::EnumCows() const
{
	return m_Content.EnumCows();
}

ICowEntry* InDoc::GetCow(uint32_t cowId) const
{
	return m_Content.GetCow(cowId);
}

ICowEntry*	InDoc::GetCowAt(int index) const
{
	return m_Content.GetCowAt(index);
}

uint32_t InDoc::GetHerdId() const
{
	return m_herdId;
}

void InDoc::SetDocDate(const DateTime& docDate)
{
	m_Details.SetDocDate(docDate);
}

void InDoc::SetExtras(const std::wstring& sExtras)
{
	m_Details.SetExtras(sExtras);
}

void InDoc::SetPlateNo(const std::wstring& sPlateNo)
{
	m_Details.SetPlateNo(sPlateNo);
}

void InDoc::SetMotive(Motive motive)
{
	m_Details.SetMotive(motive);
}

void InDoc::SetLoadStartDtTm(const Time& loadStartTm)
{
	m_Details.SetLoadStartDtTm(loadStartTm);
}

void InDoc::SetLoadEndDtTm(const Time& loadEndTm)
{
	m_Details.SetLoadEndDtTm(loadEndTm);
}

void InDoc::SetAgentId(uint32_t agentId)
{
	m_Details.SetAgentId(agentId);
}

void InDoc::SetHerdId(uint32_t herdId)
{
	m_herdId = herdId;
}

void InDoc::RemoveAt(int index)
{
	m_Content.RemoveAt(index);
}

void InDoc::RemoveAll()
{
	m_Content.RemoveAll();
}

CowEntry* InDoc::AddCow(uint32_t cowId)
{
	return m_Content.AddCow(cowId);
}

void InDoc::AddCow(CowEntry* pCow)
{
	m_Content.AddCow(pCow);
}

const DocDetails& InDoc::GetDetails() const
{
	return m_Details;
}
	
void InDoc::SetDetails(const DocDetails& details)
{
	m_Details.CopyFrom(details);
}

void InDoc::RemoveEntry(uint32_t cowId)
{
	m_Content.RemoveCow(cowId);
}

CowEntry* InDoc::GetEntry(uint32_t cowId)
{
	return m_Content.GetCow(cowId);
}