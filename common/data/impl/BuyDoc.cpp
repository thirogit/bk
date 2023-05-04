#include "stdafx.h"
#include "BuyDoc.h"


BuyDoc::BuyDoc(uint32_t docId) : m_DocId(docId),m_herdId(NULL_ID),m_hentId(NULL_ID),m_invoiceId(NULL_ID)
{
}


uint32_t BuyDoc::GetId() const
{
	return m_DocId;
}	

DocType BuyDoc::GetDocType() const
{
	return DocType_Buy;
}

void BuyDoc::CopyFrom(IBuyDoc* pSrc)
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

void BuyDoc::SetDetails(IDoc* pSrc)
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

void BuyDoc::SetLoadDate(const DateTime& loadDate)
{
	m_Details.SetLoadDate(loadDate);
}

const DateTime& BuyDoc::GetLoadDate() const
{
	return m_Details.GetLoadDate();
}


const DateTime& BuyDoc::GetDocDate() const
{
	return m_Details.GetDocDate();
}

const std::wstring& BuyDoc::GetExtras() const
{
	return m_Details.GetExtras();
}

const std::wstring& BuyDoc::GetPlateNo() const
{
	return m_Details.GetPlateNo();
}

Motive BuyDoc::GetMotive() const
{
	return m_Details.GetMotive();
}

const Time& BuyDoc::GetLoadStartDtTm() const
{
	return m_Details.GetLoadStartDtTm();
}

const Time& BuyDoc::GetLoadEndDtTm() const
{
	return m_Details.GetLoadEndDtTm();
}

uint32_t BuyDoc::GetAgentId() const
{
	return m_Details.GetAgentId();
}

int BuyDoc::GetCowCount() const
{
	return m_Content.GetCowCount();
}

PtrEnumerator<ICowEntry> BuyDoc::EnumCows() const
{
	return m_Content.EnumCows();
}

ICowEntry* BuyDoc::GetCow(uint32_t cowId) const
{
	return m_Content.GetCow(cowId);
}

ICowEntry*	BuyDoc::GetCowAt(int index) const
{
	return m_Content.GetCowAt(index);
}

uint32_t BuyDoc::GetHerdId() const
{
	return m_herdId;
}

uint32_t BuyDoc::GetHentId() const
{
	return m_hentId;
}

uint32_t BuyDoc::GetInvoiceId() const
{
	return m_invoiceId;
}

void BuyDoc::SetDocDate(const DateTime& docDate)
{
	m_Details.SetDocDate(docDate);
}

void BuyDoc::SetExtras(const std::wstring& sExtras)
{
	m_Details.SetExtras(sExtras);
}

void BuyDoc::SetPlateNo(const std::wstring& sPlateNo)
{
	m_Details.SetPlateNo(sPlateNo);
}

void BuyDoc::SetMotive(Motive motive)
{
	m_Details.SetMotive(motive);
}

void BuyDoc::SetLoadStartDtTm(const Time& loadStartTm)
{
	m_Details.SetLoadStartDtTm(loadStartTm);
}

void BuyDoc::SetLoadEndDtTm(const Time& loadEndTm)
{
	m_Details.SetLoadEndDtTm(loadEndTm);
}

void BuyDoc::SetAgentId(uint32_t agentId)
{
	m_Details.SetAgentId(agentId);
}

void BuyDoc::RemoveAt(int index)
{
	m_Content.RemoveAt(index);
}

void BuyDoc::RemoveAll()
{
	m_Content.RemoveAll();
}

CowEntry* BuyDoc::AddCow(uint32_t cowId)
{
	return m_Content.AddCow(cowId);
}

void BuyDoc::AddCow(CowEntry* pCow)
{
	m_Content.AddCow(pCow);
}

void BuyDoc::SetHerdId(uint32_t herdId)
{
	m_herdId = herdId;
}

void BuyDoc::SetHentId(uint32_t hentId)
{
	m_hentId = hentId;
}

void BuyDoc::SetInvoiceId(uint32_t invoiceId)
{
	m_invoiceId = invoiceId;	
}

void BuyDoc::SetDetails(const DocDetails& details)
{
	m_Details.CopyFrom(details);
}

void BuyDoc::RemoveEntry(uint32_t cowId)
{
	m_Content.RemoveCow(cowId);
}

CowEntry* BuyDoc::GetEntry(uint32_t cowId)
{
	return m_Content.GetCow(cowId);
}