#include "stdafx.h"
#include "Purchase.h"
#include <algorithm>
#include <arrays/DeepDelete.h>
#include <string/TextUtils.h>

Purchase::Purchase() : m_Downloaded(false), m_Herd(0)
{

}


Purchase::Purchase(const std::wstring& id) :  Purchase()
{
	m_PuracheId = id;
}

Purchase::~Purchase()
{
	DeepDelete(m_Hents);
	DeepDelete(m_Invoices);
}

Purchase::Purchase(const IPurchase* src)
{
	m_PuracheId = src->GetId();
	CopyFrom(src);
}

Purchase& Purchase::operator=(const Purchase& right)
{
	m_PuracheId = right.m_PuracheId;
	CopyFrom(right);
	return *this;
}

void Purchase::CopyDetailsFrom(const IPurchase* src)
{
	m_AgentCode = src->GetAgentCode();
	m_PlateNo = src->GetPlateNo();
	m_StartDate = src->GetStartDate();
	m_EndDate = src->GetEndDate();
	m_Herd = src->GetHerd();
	m_Downloaded = src->GetWasDownloaded();
}

void Purchase::CopyFrom(const IPurchase* src)
{
	DeepDelete(m_Hents);
	m_Hents.clear();
	DeepDelete(m_Invoices);
	m_Invoices.clear();

	CopyDetailsFrom(src);
	
	PtrEnumerator<IPurchaseHent> enumHents = src->EnumHents();
	while (enumHents.hasNext())
	{
		AddHent(*enumHents);
		enumHents.advance();
	}
	
	PtrEnumerator<IPurchaseInvoice> enumInvoices = src->EnumInvoices();
	while (enumInvoices.hasNext())
	{
		AddInvoice(*enumInvoices);
		enumInvoices.advance();
	}

	
}

Purchase::Purchase(const Purchase& src)
{
	m_PuracheId = src.m_PuracheId;
	CopyFrom(src);
}

Purchase::Purchase(Purchase&& src)
{
	m_PuracheId = src.m_PuracheId;

	CopyDetailsFrom(&src);

	src.m_AgentCode.empty();
	src.m_PlateNo.empty();
	src.m_StartDate.MakeNull();
	src.m_EndDate.MakeNull();
	src.m_Herd = NULL_ID;
	src.m_Downloaded = false;

	std::copy(src.m_Hents.begin(), src.m_Hents.end(), std::back_inserter(m_Hents));
	src.m_Hents.clear();
	std::copy(src.m_Invoices.begin(), src.m_Invoices.end(), std::back_inserter(m_Invoices));
	src.m_Invoices.clear();
	
}


void Purchase::CopyFrom(const Purchase& src)
{
	CopyFrom(&src);
}

const std::wstring& Purchase::GetId() const
{
	return m_PuracheId;
}

int Purchase::GetHerd() const
{
	return m_Herd;
}

const std::wstring& Purchase::GetAgentCode() const
{
	return m_AgentCode;
}
const std::wstring& Purchase::GetPlateNo() const
{
	return m_PlateNo;
}

const DateTime& Purchase::GetStartDate() const
{
	return m_StartDate;
}

const DateTime& Purchase::GetEndDate() const
{
	return m_EndDate;
}

PurchaseInvoice* Purchase::GetInvoiceAt(int index)
{
	return m_Invoices[index];
}

IPurchaseInvoice* Purchase::GetInvoiceAt(int index) const
{
	return m_Invoices[index];
}

PurchaseInvoice* Purchase::GetInvoice(const std::wstring& id)
{
	boost::multi_index::index<InvoicesIndex, index_tag_id>::type& indexById =
		boost::multi_index::get<index_tag_id>(m_Invoices);

	auto it = indexById.find(id);

	if (it != indexById.end())
		return (*it);

	return NULL;
}

IPurchaseInvoice* Purchase::GetInvoice(const std::wstring& id) const
{
	return const_cast<Purchase*>(this)->GetInvoice(id);
}

void Purchase::SetAgentCode(const std::wstring& sAgentCode)
{
	m_AgentCode = sAgentCode;
}

void Purchase::SetPlateNo(const std::wstring& sPlateNo)
{
	m_PlateNo = sPlateNo;
}

void Purchase::SetStartDate(const DateTime& startDt)
{
	m_StartDate = startDt;
}

void Purchase::SetEndDate(const DateTime& endDt)
{
	m_EndDate = endDt;
}

count_t Purchase::GetInvoiceCount() const
{
	return m_Invoices.size();
}

count_t Purchase::GetHentCount() const
{
	return m_Hents.size();
}

std::wstring Purchase::ObtainNewHentId() const
{
	std::wstring nextId = m_idGenerator.Generate();
	while (std::find_if(m_Hents.begin(), m_Hents.end(), [nextId](PurchaseHent* hent) -> bool {	return hent->GetId() == nextId;  }) != m_Hents.end())
	{
		nextId = m_idGenerator.Generate();
	}
	return nextId;	
}

std::wstring Purchase::ObtainNewInvoiceId() const
{
	std::wstring nextId = m_idGenerator.Generate();
	while (std::find_if(m_Invoices.begin(), m_Invoices.end(), [nextId](PurchaseInvoice* invoice) -> bool {	return invoice->GetId() == nextId;  }) != m_Invoices.end())
	{
		nextId = m_idGenerator.Generate();
	}
	return nextId;
}

PurchaseHent* Purchase::AddHent(IPurchaseHent* pHent)
{
	PurchaseHent* hent = new PurchaseHent(ObtainNewHentId());
	hent->CopyFrom(pHent);
	m_Hents.push_back(hent);
	return hent;
}

PurchaseInvoice* Purchase::AddInvoice()
{	
	PurchaseInvoice* newInvoice = new PurchaseInvoice(ObtainNewInvoiceId());
	m_Invoices.push_back(newInvoice);
	return newInvoice;
}

PurchaseInvoice* Purchase::AddInvoice(IPurchaseInvoice* pInvoice)
{
	PurchaseInvoice* newInvoice = new PurchaseInvoice(ObtainNewInvoiceId());
	newInvoice->CopyFrom(pInvoice);
	m_Invoices.push_back(newInvoice);
	return newInvoice;
}

PurchaseHent* Purchase::AddHent()
{
	PurchaseHent* newHent = new PurchaseHent(ObtainNewHentId());
	m_Hents.push_back(newHent);
	return newHent;
}

bool Purchase::GetWasDownloaded() const
{
	return m_Downloaded;
}

void Purchase::SetWasDownloaded(bool bDownloaded)
{
	m_Downloaded = bDownloaded;
}

void Purchase::SetHerd(int herd)
{
	m_Herd = herd;
}
	
PtrEnumerator<IPurchaseInvoice> Purchase::EnumInvoices() const
{
	return PtrEnumerator<IPurchaseInvoice>(m_Invoices);
}

PtrEnumerator<IPurchaseHent> Purchase::EnumHents() const
{
	return PtrEnumerator<IPurchaseHent>(m_Hents);
}


IPurchaseHent* Purchase::GetHentAt(int index) const
{
	return m_Hents[index];
}

IPurchaseHent* Purchase::GetHent(const HentNo& hentNo) const
{
	const boost::multi_index::index<PurchaseHentIndex, index_tag_hentNo>::type& indexByHentNo =
		boost::multi_index::get<index_tag_hentNo>(m_Hents);

	auto it = indexByHentNo.find(hentNo);

	if (it != indexByHentNo.end())
		return (*it);

	return NULL;

}
