#include "stdafx.h"
#include "SellDoc.h"


SellDoc::SellDoc(uint32_t docId) : m_DocId(docId),m_herdId(NULL_ID),m_hentId(NULL_ID),m_invoiceId(NULL_ID)
{
}


uint32_t SellDoc::GetId() const
{
	return m_DocId;
}	

DocType SellDoc::GetDocType() const
{
	return DocType_Sell;
}

void SellDoc::SetDetails(const DocDetails& details)
{
	m_Details.CopyFrom(details);
}

void SellDoc::SetDetails(IDoc* pSrc)
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


void SellDoc::CopyFrom(ISellDoc* pSrc)
{
	m_herdId = pSrc->GetHerdId();		
	m_hentId = pSrc->GetHentId();	
	m_invoiceId = pSrc->GetInvoiceId();	
	
	SetDetails(pSrc);

	m_Content.RemoveAll();
	for(int i = 0,count = pSrc->GetCowCount();i < count;i++)
	{
		ICowEntry* pSrcEntry = pSrc->GetCowAt(i);
		m_Content.AddCow(pSrcEntry->GetCowId())->CopyFrom(pSrcEntry);
	}
	
}

const DateTime& SellDoc::GetDocDate() const
{
	return m_Details.GetDocDate();
}

void SellDoc::SetLoadDate(const DateTime& loadDate)
{
	m_Details.SetLoadDate(loadDate);
}

const DateTime& SellDoc::GetLoadDate() const
{
	return m_Details.GetLoadDate();
}

const std::wstring& SellDoc::GetExtras() const
{
	return m_Details.GetExtras();
}

const std::wstring& SellDoc::GetPlateNo() const
{
	return m_Details.GetPlateNo();
}

Motive SellDoc::GetMotive() const
{
	return m_Details.GetMotive();
}

const Time& SellDoc::GetLoadStartDtTm() const
{
	return m_Details.GetLoadStartDtTm();
}

const Time& SellDoc::GetLoadEndDtTm() const
{
	return m_Details.GetLoadEndDtTm();
}

uint32_t SellDoc::GetAgentId() const
{
	return m_Details.GetAgentId();
}

int SellDoc::GetCowCount() const
{
	return m_Content.GetCowCount();
}

PtrEnumerator<ICowEntry> SellDoc::EnumCows() const
{
	return m_Content.EnumCows();
}

ICowEntry* SellDoc::GetCow(uint32_t cowId) const
{
	return m_Content.GetCow(cowId);
}

ICowEntry*	SellDoc::GetCowAt(int index) const
{
	return m_Content.GetCowAt(index);
}

uint32_t SellDoc::GetHerdId() const
{
	return m_herdId;
}

uint32_t SellDoc::GetHentId() const
{
	return m_hentId;
}

uint32_t SellDoc::GetInvoiceId() const
{
	return m_invoiceId;
}
	

void SellDoc::SetDocDate(const DateTime& docDate)
{
	m_Details.SetDocDate(docDate);
}

void SellDoc::SetExtras(const std::wstring& sExtras)
{
	m_Details.SetExtras(sExtras);
}

void SellDoc::SetPlateNo(const std::wstring& sPlateNo)
{
	m_Details.SetPlateNo(sPlateNo);
}

void SellDoc::SetMotive(Motive motive)
{
	m_Details.SetMotive(motive);
}

void SellDoc::SetLoadStartDtTm(const Time& loadStartTm)
{
	m_Details.SetLoadStartDtTm(loadStartTm);
}

void SellDoc::SetLoadEndDtTm(const Time& loadEndTm)
{
	m_Details.SetLoadEndDtTm(loadEndTm);
}

void SellDoc::SetAgentId(uint32_t agentId)
{
	m_Details.SetAgentId(agentId);
}

void SellDoc::RemoveAt(int index)
{
	m_Content.RemoveAt(index);
}

void SellDoc::RemoveAll()
{
	m_Content.RemoveAll();
}

CowEntry* SellDoc::AddCow(uint32_t cowId)
{
	return m_Content.AddCow(cowId);
}

void SellDoc::AddCow(CowEntry* pCow)
{
	m_Content.AddCow(pCow);
}

void SellDoc::SetHerdId(uint32_t herdId)
{
	m_herdId = herdId;
}

void SellDoc::SetHentId(uint32_t hentId)
{
	m_hentId = hentId;
}

void SellDoc::SetInvoiceId(uint32_t invoiceId)
{
	m_invoiceId = invoiceId;
	
}

void SellDoc::RemoveEntry(uint32_t cowId)
{
	m_Content.RemoveCow(cowId);
}

CowEntry* SellDoc::GetEntry(uint32_t cowId)
{
	return m_Content.GetCow(cowId);
}