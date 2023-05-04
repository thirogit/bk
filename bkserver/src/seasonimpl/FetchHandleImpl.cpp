#include "stdafx.h"
#include "FetchHandleImpl.h"
#include "SeasonSession.h"

FetchHandleImpl::FetchHandleImpl(SeasonSession* pSession) :	m_pSession(pSession)
{
	m_HentIt = m_pSession->m_Hents.begin();
	m_CowIt = m_pSession->m_Cows.begin();
	m_InDocIt = m_pSession->m_InDocs.begin();
	m_OutDocIt = m_pSession->m_OutDocs.begin();
	m_BuyDocIt = m_pSession->m_BuyDocs.begin();
	m_SellDocIt = m_pSession->m_SellDocs.begin();
	m_MoveDocIt = m_pSession->m_MoveDocs.begin();
	m_BuyInvoiceIt = m_pSession->m_BuyInvoices.begin();
	m_SellInvoiceIt = m_pSession->m_SellInvoices.begin();
}

FetchHandleImpl::~FetchHandleImpl()
{
	m_pSession->CloseFetchHandle(this);
}

HentServerObject* FetchHandleImpl::NextHentObject()
{
	if(m_HentIt != m_pSession->m_Hents.end())
		return *m_HentIt++;

	return NULL;
}

size_t	FetchHandleImpl::HentObjectsCount()
{
	return m_pSession->m_Hents.size();
}

CowServerObject* FetchHandleImpl::NextCowObject()
{
	if(m_CowIt != m_pSession->m_Cows.end())
		return *m_CowIt++;

	return NULL;
}

size_t FetchHandleImpl::CowObjectsCount()
{
	return m_pSession->m_Cows.size();
}
	
InDocServerObject* FetchHandleImpl::NextInDocObject()
{
	if(m_InDocIt != m_pSession->m_InDocs.end())
		return *m_InDocIt++;
	return NULL;
}

size_t FetchHandleImpl::InDocObjectsCount()
{
	return m_pSession->m_InDocs.size();
}
	
SellDocServerObject* FetchHandleImpl::NextSellDocObject()
{
	if(m_SellDocIt != m_pSession->m_SellDocs.end())
		return *m_SellDocIt++;
	return NULL;
}

size_t FetchHandleImpl::SellDocObjectsCount()
{
	return m_pSession->m_SellDocs.size();
}
	
MoveDocServerObject* FetchHandleImpl::NextMoveDocObject()
{
	if(m_MoveDocIt != m_pSession->m_MoveDocs.end())
		return *m_MoveDocIt++;

	return NULL;
}

size_t	FetchHandleImpl::MoveDocObjectsCount()
{
	return m_pSession->m_MoveDocs.size();
}
	
BuyDocServerObject* FetchHandleImpl::NextBuyDocObject()
{
	if(m_BuyDocIt != m_pSession->m_BuyDocs.end())
		return *m_BuyDocIt++;

	return NULL;
}

size_t	FetchHandleImpl::BuyDocObjectsCount()
{
	return m_pSession->m_BuyDocs.size();
}
	
OutDocServerObject* FetchHandleImpl::NextOutDocObject()
{
	if(m_OutDocIt != m_pSession->m_OutDocs.end())
		return *m_OutDocIt++;

	return NULL;
}

size_t	FetchHandleImpl::OutDocObjectsCount()
{
	return m_pSession->m_OutDocs.size();
}
	
BuyInvoiceServerObject* FetchHandleImpl::NextBuyInvoiceObject()
{
	if(m_BuyInvoiceIt != m_pSession->m_BuyInvoices.end())
		return *m_BuyInvoiceIt++;
	return NULL;
}

size_t FetchHandleImpl::BuyInvoiceObjectsCount()
{
	return m_pSession->m_BuyInvoices.size();
}
	
SellInvoiceServerObject*	FetchHandleImpl::NextSellInvoiceObject()
{
	if(m_SellInvoiceIt != m_pSession->m_SellInvoices.end())
		return *m_SellInvoiceIt++;

	return NULL;
}

size_t	FetchHandleImpl::SellInvoiceObjectsCount()
{
	return m_pSession->m_SellInvoices.size();
}
	
