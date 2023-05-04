#include "stdafx.h"
#include "MoveDoc.h"

MoveDoc::MoveDoc(uint32_t docId) : m_DocId(docId),m_herdFromId(NULL_ID),m_herdToId(NULL_ID)
{
}


uint32_t MoveDoc::GetId() const
{
	return m_DocId;
}	

DocType MoveDoc::GetDocType() const
{
	return DocType_Move;
}

void MoveDoc::CopyFrom(IMoveDoc* pSrc)
{
	m_herdFromId = pSrc->GetSrcHerdId();
	m_herdToId = pSrc->GetDstHerdId();

	SetDetails(pSrc);

	m_Content.RemoveAll();
	for(int i = 0,count = pSrc->GetCowCount();i < count;i++)
	{
		ICowEntry* pSrcEntry = pSrc->GetCowAt(i);
		m_Content.AddCow(pSrcEntry->GetCowId())->CopyFrom(pSrcEntry);
	}	
}

void MoveDoc::SetDetails(IDoc* pSrc)
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

const DocDetails& MoveDoc::GetDetails() const
{
	return m_Details;
}

void MoveDoc::SetDetails(const DocDetails& details)
{
	m_Details.CopyFrom(details);
}

void MoveDoc::SetLoadDate(const DateTime& loadDate)
{
	m_Details.SetLoadDate(loadDate);
}

const DateTime& MoveDoc::GetLoadDate() const
{
	return m_Details.GetLoadDate();
}

const DateTime& MoveDoc::GetDocDate() const
{
	return m_Details.GetDocDate();
}

const std::wstring& MoveDoc::GetExtras() const
{
	return m_Details.GetExtras();
}

const std::wstring& MoveDoc::GetPlateNo() const
{
	return m_Details.GetPlateNo();
}

Motive MoveDoc::GetMotive() const
{
	return m_Details.GetMotive();
}

const Time& MoveDoc::GetLoadStartDtTm() const
{
	return m_Details.GetLoadStartDtTm();
}

const Time& MoveDoc::GetLoadEndDtTm() const
{
	return m_Details.GetLoadEndDtTm();
}

uint32_t MoveDoc::GetAgentId() const
{
	return m_Details.GetAgentId();
}

int MoveDoc::GetCowCount() const
{
	return m_Content.GetCowCount();
}

PtrEnumerator<ICowEntry> MoveDoc::EnumCows() const
{
	return m_Content.EnumCows();
}

ICowEntry* MoveDoc::GetCow(uint32_t cowId) const
{
	return m_Content.GetCow(cowId);
}

ICowEntry*	MoveDoc::GetCowAt(int index) const
{
	return m_Content.GetCowAt(index);
}

uint32_t MoveDoc::GetSrcHerdId() const
{
	return m_herdFromId;
}

uint32_t MoveDoc::GetDstHerdId() const
{
	return m_herdToId;
}

void MoveDoc::SetSrcHerdId(uint32_t herdFromId)
{
	m_herdFromId = herdFromId;
}

void MoveDoc::SetDstHerdId(uint32_t herdtoId)
{
	m_herdToId = herdtoId;
}

void MoveDoc::SetDocDate(const DateTime& docDate)
{
	m_Details.SetDocDate(docDate);
}

void MoveDoc::SetExtras(const std::wstring& sExtras)
{
	m_Details.SetExtras(sExtras);
}

void MoveDoc::SetPlateNo(const std::wstring& sPlateNo)
{
	m_Details.SetPlateNo(sPlateNo);
}

//void MoveDoc::SetMotive(Motive motive)
//{
//	m_Details.SetMotive(motive);
//}

void MoveDoc::SetLoadStartDtTm(const Time& loadStartTm)
{
	m_Details.SetLoadStartDtTm(loadStartTm);
}

void MoveDoc::SetLoadEndDtTm(const Time& loadEndTm)
{
	m_Details.SetLoadEndDtTm(loadEndTm);
}

void MoveDoc::SetAgentId(uint32_t agentId)
{
	m_Details.SetAgentId(agentId);
}

void MoveDoc::RemoveAt(int index)
{
	m_Content.RemoveAt(index);
}

void MoveDoc::RemoveAll()
{
	m_Content.RemoveAll();
}

CowEntry* MoveDoc::AddCow(uint32_t cowId)
{
	return m_Content.AddCow(cowId);
}

void MoveDoc::AddCow(CowEntry* pCow)
{
	m_Content.AddCow(pCow);
}

void MoveDoc::RemoveEntry(uint32_t cowId)
{
	m_Content.RemoveCow(cowId);
}

CowEntry* MoveDoc::GetEntry(uint32_t cowId)
{
	return m_Content.GetCow(cowId);
}