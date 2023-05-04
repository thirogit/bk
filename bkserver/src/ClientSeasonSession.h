#pragma once

#include <arrays/Enumerator.h>
#include "object/HentServerObject.h"
#include <data/IClass.h>
#include "object/CowServerObject.h"
#include <data/IStock.h>
#include "object/InDocServerObject.h"
#include "object/OutDocServerObject.h"
#include "object/BuyDocServerObject.h"
#include "object/SellDocServerObject.h"
#include "object/MoveDocServerObject.h"
#include "object/BuyInvoiceServerObject.h"
#include "object/SellInvoiceServerObject.h"
#include <data/IAgent.h>
#include <boost/atomic.hpp>
#include "transaction/IClientTransaction.h"
#include "IFetchHandle.h"
#include <data/IHerd.h>
#include <data/DocKey.h>
#include <data/InvoiceKey.h>
#include "ClientSeasonSessionInterface.h"
#include "transaction/ISeasonInterface.h"
#include "ClientSession.h"
#include "settings/UserSettings.h"
#include <data/repo/Purchases.h>
#include "repo/RepoClient.h"
#include "log/LogInterface.h"

class SeasonSession;

class ClientSeasonSession
{
	class SeasonInterface : public ISeasonInterface
	{
	public:
		SeasonInterface(ClientSeasonSession* parent);
		virtual void Do(std::function<void (SeasonSessionFacade*)>& action);
		virtual void DoUser(std::function<void(UserSessionFacade*)>& action);
	private:
		ClientSeasonSession* m_parent;
	};

	class SeasonLogInterface : public LogInterface
	{
	public:
		SeasonLogInterface(ClientSeasonSession* parent);
		virtual void Log(LogLevel level, const std::wstring& msg);
	private:
		std::wstring FormatSeasonMessage(const std::wstring& msg) const;
	private:
		ClientSeasonSession* m_parent;
	};

public:
	ClientSeasonSession(ClientSession* pClientSession,ClientSeasonSessionInterface* pInterface,uint32_t sessionToken);
	~ClientSeasonSession();
	uint32_t GetSessionToken() const;
	uint32_t GetSeasonId() const;	
	ClientSession* GetClientSession() const;
	uint16_t GetObserverPort() const;
	void Close();
	
	FetchHandle BeginFetch();
	FetchHandle GetFetchHandle();
	void EndFetch();

	void LockHent(uint32_t hentId);
	void UnlockHent(uint32_t hentId);
	uint32_t InsertHent(IHent* pHent);
	void UpdateHent(IHent* pHent);
	void DeleteHent(uint32_t hentId);

	void LockCow(uint32_t cowId);
	void UnlockCow(uint32_t cowId);
	void UpdateCow(uint32_t cowId,const CowDetails& details);

	//void LockInDoc(uint32_t docId);
	void LockInDocWithCows(uint32_t docId);

	void UnlockInDoc(uint32_t docId);
	void DeleteInDoc(uint32_t docId);

	void LockBuyDoc(uint32_t docId);
	void UnlockBuyDoc(uint32_t docId);
	void DeleteBuyDoc(uint32_t docId);

	void LockMoveDoc(uint32_t docId);
	void UnlockMoveDoc(uint32_t docId);
	void DeleteMoveDoc(uint32_t docId);

	void LockOutDoc(uint32_t docId);
	void UnlockOutDoc(uint32_t docId);
	void DeleteOutDoc(uint32_t docId);

	void LockSellDoc(uint32_t docId);
	void UnlockSellDoc(uint32_t docId);	
	void DeleteSellDoc(uint32_t docId);

	void LockBuyInvoice(uint32_t invoiceId);
	void UnlockBuyInvoice(uint32_t invoiceId);
	void DeleteBuyInvoice(uint32_t invoiceId);

	void LockSellInvoice(uint32_t invoiceId);
	void UnlockSellInvoice(uint32_t invoiceId);
	void DeleteSellInvoice(uint32_t invoiceId);
	
	IClientTransaction* OpenNewInDocTransaction(uint32_t herdId);
	IClientTransaction* OpenEditInDocTransaction(uint32_t docId);
	
	IClientTransaction* OpenNewMoveDocTransaction(uint32_t srcHerdId,uint32_t dstHerdId);
	IClientTransaction* OpenEditMoveDocTransaction(uint32_t docId);

	IClientTransaction* OpenNewSellDocTransaction(uint32_t herdId);
	IClientTransaction* OpenEditSellDocTransaction(uint32_t docId);

	IClientTransaction* OpenNewBuyDocTransaction(uint32_t herdId);
	IClientTransaction* OpenEditBuyDocTransaction(uint32_t docId);

	IClientTransaction* OpenNewOutDocTransaction(uint32_t herdId);
	IClientTransaction* OpenEditOutDocTransaction(uint32_t docId);

	IClientTransaction* OpenNewBuyInvoiceTransaction(InvoiceScope scope);
	IClientTransaction* OpenEditBuyInvoiceTransaction(uint32_t invoiceId);
	
	IClientTransaction* OpenNewSellInvoiceTransaction(InvoiceScope scope);
	IClientTransaction* OpenEditSellInvoiceTransaction(uint32_t invoiceId);
	
	IClientTransaction* FindTransaction(uint32_t transactionId);
	IClientTransaction* AcquireTransaction(uint32_t transactionId);
	void DisposeTransaction(uint32_t transactionId);
	void CommitTransaction(uint32_t transactionId);
	void AbortTransaction(uint32_t transactionId);
	boost::any ExecuteOperation(uint32_t transactionId,uint32_t operation,const OperationArguments& args);
		
	void OpenPurchaseRepository();
	int QueryPurchases(const DateTime& startDt, const DateTime& endDt);	
	PurchaseHeaders FetchPurchaseHeaders();
	boost::optional<Purchase> FetchPurchase(const std::wstring& purchaseId);
	IClientTransaction* OpenDownloadPurchaseTransaction(const std::wstring& purchaseId);
	void ClosePurchaseRepository();
protected:

	template<typename F> void SeasonDoAny(F f)
	{
		m_pInterface->DoAny(this,f);
	}

	template<typename F> void UserDoAny(F f)
	{
		m_pClientSession->UserDoAny(f);
	}

	template<typename R,typename F>
	R SeasonEval(F functor)
	{
		return m_pInterface->EvalAny<R>(this,functor);
	}

	template<typename M,typename A1> void SeasonDoFacade(M facadeMem,A1& a1)
	{
		auto facadeFn = std::mem_fn(facadeMem);
		std::function<void (SeasonSessionFacade*)> action = std::bind(facadeFn,std::placeholders::_1,a1);
		m_pInterface->Do(this,action);
	}

	template<typename M,typename A1,typename A2> void SeasonDoFacade(M facadeMem,A1& a1,A2& a2)
	{
		auto facadeFn = std::mem_fn(facadeMem);
		std::function<void (SeasonSessionFacade*)> action = std::bind(facadeFn,std::placeholders::_1,a1,a2);
		m_pInterface->Do(this,action);
	}
	
	void ThrowLockOnDocNotAcquiredForTransaction(const DocKey& docKey);
	void ThrowLockOnInvoiceNotAcquiredForTransaction(const InvoiceKey& invoiceKey);

	void ThrowIfNotInFetchState();
	void ThrowTransactionNotFound();
	void ThrowUnsupportedInvoiceScope();

	uint32_t ObtainNextTransactionId();
	IHerd* AcquireHerd(uint32_t herdId);

	void ThrowIfNoRepoSession();

private:
	repo::RepoClient* m_pClient;	
	repo::PurchaseHeaderCursorPtr m_purchasesCursor;

	ClientSession* m_pClientSession;
	ClientSeasonSessionInterface* m_pInterface;
	SeasonLogInterface* m_pLog;
	SeasonInterface* m_pSeasonInterface;
	uint32_t m_SessionToken;
	boost::atomic<uint32_t> m_TransactionIdSequence;
	std::map<uint32_t,IClientTransaction*> m_Transactions;
	FetchHandle m_FetchHandle;
};
