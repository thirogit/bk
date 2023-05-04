#include "stdafx.h"
#include "ClientSeasonSessionFacade.h"
#include "ClientSeasonSession.h"
#include "SessionException.h"

ClientSeasonSessionFacade::ClientSeasonSessionFacade(ClientSeasonSession* pSession) : m_pSessionDelegate(pSession)
{
}

ClientSeasonSessionFacade::ClientSeasonSessionFacade(const ClientSeasonSessionFacade& src)
{
	operator=(src);
}

ClientSeasonSessionFacade& ClientSeasonSessionFacade::operator=(const ClientSeasonSessionFacade& right)
{
	m_pSessionDelegate = right.m_pSessionDelegate;
	return *this;
}

void ClientSeasonSessionFacade::ThrowIfNoDelegate() const
{
	if(!m_pSessionDelegate)
		BOOST_THROW_EXCEPTION(SessionException(L"invalid call, null delegate"));
}

uint32_t ClientSeasonSessionFacade::GetSessionToken() const
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->GetSessionToken();

}

uint32_t ClientSeasonSessionFacade::GetSeasonId() const
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->GetSeasonId();
}

bool ClientSeasonSessionFacade::IsGood() const
{
	return m_pSessionDelegate != NULL;
}

void ClientSeasonSessionFacade::LockHent(uint32_t hentId)
{
	ThrowIfNoDelegate();
	m_pSessionDelegate->LockHent(hentId);
}

void ClientSeasonSessionFacade::UnlockHent(uint32_t hentId)
{
	ThrowIfNoDelegate();
	m_pSessionDelegate->UnlockHent(hentId);
}

FetchHandle ClientSeasonSessionFacade::BeginFetch()
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->BeginFetch();
}

FetchHandle ClientSeasonSessionFacade::GetFetchHandle()
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->GetFetchHandle();
}

void ClientSeasonSessionFacade::EndFetch()
{
	ThrowIfNoDelegate();
	m_pSessionDelegate->EndFetch();
}

uint32_t ClientSeasonSessionFacade::InsertHent(IHent* pHent)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->InsertHent(pHent);
}

void ClientSeasonSessionFacade::UpdateHent(IHent* pHent)
{
	ThrowIfNoDelegate();
	m_pSessionDelegate->UpdateHent(pHent);
}

void ClientSeasonSessionFacade::DeleteHent(uint32_t hentId)
{
	ThrowIfNoDelegate();
	m_pSessionDelegate->DeleteHent(hentId);
}


uint16_t ClientSeasonSessionFacade::GetObserverPort() const
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->GetObserverPort();
}

void ClientSeasonSessionFacade::LockCow(uint32_t cowId)
{
	ThrowIfNoDelegate();
	m_pSessionDelegate->LockCow(cowId);
}

void ClientSeasonSessionFacade::UnlockCow(uint32_t cowId)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->UnlockCow(cowId);
}

IClientTransaction* ClientSeasonSessionFacade::OpenNewInDocTransaction(uint32_t herdId)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->OpenNewInDocTransaction(herdId);
}

IClientTransaction* ClientSeasonSessionFacade::OpenEditInDocTransaction(uint32_t docId)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->OpenEditInDocTransaction(docId);
}

IClientTransaction* ClientSeasonSessionFacade::FindDocTransaction(uint32_t transactionId)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->FindTransaction(transactionId);
}

IClientTransaction* ClientSeasonSessionFacade::OpenNewMoveDocTransaction(uint32_t srcHerdId,uint32_t dstHerdId)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->OpenNewMoveDocTransaction(srcHerdId,dstHerdId);
}

IClientTransaction* ClientSeasonSessionFacade::OpenNewSellDocTransaction(uint32_t herdId)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->OpenNewSellDocTransaction(herdId);
}

void ClientSeasonSessionFacade::CommitTransaction(uint32_t transactionId)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->CommitTransaction(transactionId);
}

void ClientSeasonSessionFacade::AbortTransaction(uint32_t transactionId)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->AbortTransaction(transactionId);
}

boost::any ClientSeasonSessionFacade::ExecuteOperation(uint32_t transactionId,uint32_t operation,const OperationArguments& args)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->ExecuteOperation(transactionId,operation,args);
}

IClientTransaction* ClientSeasonSessionFacade::OpenNewBuyDocTransaction(uint32_t herdId)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->OpenNewBuyDocTransaction(herdId);
}

IClientTransaction* ClientSeasonSessionFacade::OpenNewOutDocTransaction(uint32_t herdId)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->OpenNewOutDocTransaction(herdId);
}

void ClientSeasonSessionFacade::LockInDocWithCows(uint32_t docId)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->LockInDocWithCows(docId);
}

void ClientSeasonSessionFacade::UnlockInDoc(uint32_t docId)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->UnlockInDoc(docId);
}

void ClientSeasonSessionFacade::DeleteInDoc(uint32_t docId)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->DeleteInDoc(docId);
}

void ClientSeasonSessionFacade::LockBuyDoc(uint32_t docId)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->LockBuyDoc(docId);
}

void ClientSeasonSessionFacade::UnlockBuyDoc(uint32_t docId)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->UnlockBuyDoc(docId);
}	

void ClientSeasonSessionFacade::DeleteBuyDoc(uint32_t docId)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->DeleteBuyDoc(docId);
}

void ClientSeasonSessionFacade::LockMoveDoc(uint32_t docId)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->LockMoveDoc(docId);
}

void ClientSeasonSessionFacade::UnlockMoveDoc(uint32_t docId)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->UnlockMoveDoc(docId);
}


void ClientSeasonSessionFacade::DeleteMoveDoc(uint32_t docId)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->DeleteMoveDoc(docId);
}

void ClientSeasonSessionFacade::LockOutDoc(uint32_t docId)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->LockOutDoc(docId);
}

void ClientSeasonSessionFacade::UnlockOutDoc(uint32_t docId)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->UnlockOutDoc(docId);
}

void ClientSeasonSessionFacade::DeleteOutDoc(uint32_t docId)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->DeleteOutDoc(docId);
}

void ClientSeasonSessionFacade::LockSellDoc(uint32_t docId)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->LockSellDoc(docId);
}

void ClientSeasonSessionFacade::UnlockSellDoc(uint32_t docId)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->UnlockSellDoc(docId);
}

void ClientSeasonSessionFacade::DeleteSellDoc(uint32_t docId)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->DeleteSellDoc(docId);
}

IClientTransaction* ClientSeasonSessionFacade::OpenEditMoveDocTransaction(uint32_t docId)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->OpenEditMoveDocTransaction(docId);
}

IClientTransaction* ClientSeasonSessionFacade::OpenEditOutDocTransaction(uint32_t docId)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->OpenEditOutDocTransaction(docId);
}

IClientTransaction* ClientSeasonSessionFacade::OpenEditBuyDocTransaction(uint32_t docId)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->OpenEditBuyDocTransaction(docId);
}

IClientTransaction* ClientSeasonSessionFacade::OpenEditSellDocTransaction(uint32_t docId)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->OpenEditSellDocTransaction(docId);
}

IClientTransaction* ClientSeasonSessionFacade::OpenNewBuyInvoiceTransaction(InvoiceScope scope)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->OpenNewBuyInvoiceTransaction(scope);
}

IClientTransaction* ClientSeasonSessionFacade::OpenEditBuyInvoiceTransaction(uint32_t invoiceId)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->OpenEditBuyInvoiceTransaction(invoiceId);
}

void ClientSeasonSessionFacade::DeleteBuyInvoice(uint32_t invoiceId)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->DeleteBuyInvoice(invoiceId);
}

IClientTransaction* ClientSeasonSessionFacade::OpenNewSellInvoiceTransaction(InvoiceScope scope)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->OpenNewSellInvoiceTransaction(scope);
}

IClientTransaction* ClientSeasonSessionFacade::OpenEditSellInvoiceTransaction(uint32_t invoiceId)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->OpenEditSellInvoiceTransaction(invoiceId);
}

void ClientSeasonSessionFacade::DeleteSellInvoice(uint32_t invoiceId)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->DeleteSellInvoice(invoiceId);
}

void ClientSeasonSessionFacade::LockSellInvoice(uint32_t invoiceId)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->LockSellInvoice(invoiceId);
}

void ClientSeasonSessionFacade::UnlockSellInvoice(uint32_t invoiceId)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->UnlockSellInvoice(invoiceId);
}

void ClientSeasonSessionFacade::LockBuyInvoice(uint32_t invoiceId)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->LockBuyInvoice(invoiceId);
}

void ClientSeasonSessionFacade::UnlockBuyInvoice(uint32_t invoiceId)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->UnlockBuyInvoice(invoiceId);
}

void ClientSeasonSessionFacade::Close()
{
	ThrowIfNoDelegate();
	m_pSessionDelegate->Close();
	m_pSessionDelegate = NULL;
}

void ClientSeasonSessionFacade::UpdateCow(uint32_t cowId,const CowDetails& details)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->UpdateCow(cowId,details);
}

void ClientSeasonSessionFacade::OpenPurchaseRepository()
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->OpenPurchaseRepository();
}

int ClientSeasonSessionFacade::QueryPurchases(const DateTime& startDt, const DateTime& endDt)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->QueryPurchases(startDt,endDt);
}

PurchaseHeaders ClientSeasonSessionFacade::FetchPurchaseHeaders()
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->FetchPurchaseHeaders();
}

IClientTransaction* ClientSeasonSessionFacade::OpenDownloadPurchaseTransaction(const std::wstring& purchaseId)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->OpenDownloadPurchaseTransaction(purchaseId);
}

boost::optional<Purchase> ClientSeasonSessionFacade::FetchPurchase(const std::wstring& purchaseId)
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->FetchPurchase(purchaseId);
}

void ClientSeasonSessionFacade::ClosePurchaseRepository()
{
	ThrowIfNoDelegate();
	return m_pSessionDelegate->ClosePurchaseRepository();
}