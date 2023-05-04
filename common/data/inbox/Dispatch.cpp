#include "stdafx.h"
#include "Purchase.h"
#include <algorithm>
#include <arrays/DeepDelete.h>

Purchase::Purchase(uint32_t puracheId) : m_PuracheId(puracheId), m_Downloaded(false), m_Herd(0)
{
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

void Purchase::CopyFrom(const IPurchase* src)
{
	DeepDelete(m_Hents);
	m_Hents.clear();
	DeepDelete(m_Invoices);
	m_Invoices.clear();

	m_AgentCode = src->GetAgentCode();
	m_PlateNo = src->GetPlateNo();
	m_StartDate = src->GetStartDate();
	m_EndDate = src->GetEndDate();
	m_Herd = src->GetHerd();
	
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

	m_Downloaded = src->GetWasDownloaded();
}

Purchase::Purchase(const Purchase& src)
{
	m_PuracheId = src.m_PuracheId;
	CopyFrom(src);
}


void Purchase::CopyFrom(const Purchase& src)
{
	CopyFrom(&src);
}

uint32_t Purchase::GetId() const
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

PurchaseInvoice* Purchase::GetInvoice(uint32_t id)
{
	boost::multi_index::index<InvoicesIndex, index_tag_id>::type& indexById =
		boost::multi_index::get<index_tag_id>(m_Invoices);

	auto it = indexById.find(id);

	if (it != indexById.end())
		return (*it);

	return NULL;
}

IPurchaseInvoice* Purchase::GetInvoice(uint32_t id) const
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

uint32_t Purchase::ObtainNewHentId() const
{
	if (m_Hents.empty())
		return 1;

	auto maxIt = std::max_element(m_Hents.begin(), m_Hents.end(), 
				[](PurchaseHent* hent1, PurchaseHent* hent2) -> bool
				{
					return hent1->GetId() < hent2->GetId();
				});

	return (*maxIt)->GetId()+1;
}

uint32_t Purchase::ObtainNewInvoiceId() const
{
	if (m_Invoices.empty())
		return 1;

	auto maxIt = std::max_element(m_Invoices.begin(), m_Invoices.end(),
				[](PurchaseInvoice* invoice1, PurchaseInvoice* invoice2) -> bool
				{
					return invoice1->GetId() < invoice2->GetId();
				});

	return (*maxIt)->GetId()+1;
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
