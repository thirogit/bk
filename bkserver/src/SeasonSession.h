#pragma once

#include "SeasonSpectator.h"
#include "ClientSeasonSessionFacade.h"
#include <boost/atomic.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "dao/MuuDAO.h"
#include <logic/cowtrail/CowTracker.h>
#include <logic/herdregistry/HerdRegistry.h>
#include "InvoiceRelations.h"
#include "SessionLockRecord.h"
#include "CowNoReference.h"
#include <set>
#include "ClientSeasonSessionInterface.h"
#include "SessionTokenGenerator.h"
#include "seasonimpl/StateHolder.h"
#include "seasonimpl/SeasonSessionState.h"
#include "seasonimpl/FetchHandleImpl.h"
#include "index/HentsIndex.h"
#include "index/ClassesIndex.h"
#include "index/StocksIndex.h"
#include "index/IdableIndexes.h"
#include "BkLogger.h"
#include "SessionException.h"
#include "cloud/ChangePublisher.h"

class FarmSession;
class ClientSession;
class SeasonSessionBuilder;
class ClientSeasonSession;
class SeasonSessionFacadeImpl;
class ClientSeasonSessionInterfaceImpl;
class InsertPurchaseOperation;

class SeasonSession
{		
	typedef boost::any_range<uint32_t,boost::forward_traversal_tag, uint32_t, std::ptrdiff_t> IdRange;

	SeasonSession();
	void Close(ClientSeasonSession* session);	
	
	HentServerObject* FindHent(uint32_t hentId);
	HentServerObject* FindHent(const HentNo& hentNo);
	void LockHent(ClientSeasonSession* pClientSession,uint32_t hentId);
	void UnlockHent(ClientSeasonSession* pClient,uint32_t hentId);
	uint32_t InsertHent(ClientSeasonSession* pClient,IHent* hent);
	void UpdateHent(ClientSeasonSession* pClient,IHent* hent);
	void DeleteHent(ClientSeasonSession* pClient,uint32_t hentId);
	HentServerObject* AcquireHent(uint32_t hentId);
		
	CowServerObject* FindCow(uint32_t cowId);
	void LockCow(ClientSeasonSession* pClient,uint32_t cowId);
	void UnlockCow(ClientSeasonSession* pClient,uint32_t cowId);
	void UpdateCow(ClientSeasonSession* pClient,uint32_t cowId,const CowDetails& details);
	ICow* FindCowInDoc(IDoc* pDoc,const CowNo& cowNo);
	void ValidateCowDetails(const CowDetails& details);
	void ThrowChangeInroducesDuplicationInDoc(IDoc* pDoc,const CowNo& cowNo);
	CowServerObject* AcquireCow(uint32_t cowId);

	void ValidateDocDetails(const DocDetails& details);
	void ValidateOwnDocSnapshot(const OwnDocSnapshot& snapshot);
	void ThrowInvalidMotive();
	void ThrowInvalidAgent(uint32_t agentId);
	void ValidateItemCount(const DocSnapshot& snapshot);
	
	void CheckForInStockDuplication(uint32_t herdId,const CowNo& cowNo);
	void CheckIfCowWasMovedFromHerd(uint32_t herdId,const CowPresenceKey& key);
	void CheckForLockHighjack(std::vector<CowServerObject*>& cowSOs,ClientSeasonSession* pClient);
	void CheckIfCowNoChangeIsAllowed(uint32_t cowId,const CowNo& newCowNo);

	InDocServerObject* FindInDoc(uint32_t docId);
	InDocServerObject* AcquireInDoc(uint32_t docId);
	void LockInDocWithCows(ClientSeasonSession* pClient,uint32_t docId);
	void LockInDoc(ClientSeasonSession* pClient, uint32_t docId);
	void UnlockInDoc(ClientSeasonSession* pClient,uint32_t docId);
	void UpdateInDoc(ClientSeasonSession* pClient,const OwnDocSnapshot& snapshot);
	void InsertInDoc(ClientSeasonSession* pClient,const OwnDocSnapshot& snapshot);
	void DeleteInDoc(ClientSeasonSession* pClient,uint32_t docId);
	void ThrowOperationCausesDuplicationInHerd(const CowNo& cowNo,uint32_t herdId);

	void ThrowIfDocCowsAreNotStationary(IDoc* pDoc);
	void ValidateHentsDocSnapshot(const HentsDocSnapshot& snapshot);
	void ValidateBuyDocSnapshot(const HentsDocSnapshot& snapshot);
	BuyDocServerObject* FindBuyDoc(uint32_t docId);
	BuyDocServerObject* AcquireBuyDoc(uint32_t docId);
	void LockBuyDoc(ClientSeasonSession* pClient,uint32_t docId);
	void UnlockBuyDoc(ClientSeasonSession* pClient,uint32_t docId);
	void UpdateBuyDoc(ClientSeasonSession* pClient,const HentsDocSnapshot& snapshot);
	void InsertBuyDoc(ClientSeasonSession* pClient,const HentsDocSnapshot& snapshot);
	void DeleteBuyDoc(ClientSeasonSession* pClient,uint32_t docId);

	void ValidateMoveDocSnapshot(const MoveDocSnapshot& shapshot);
	MoveDocServerObject* FindMoveDoc(uint32_t docId);
	MoveDocServerObject* AcquireMoveDoc(uint32_t docId);
	void LockMoveDoc(ClientSeasonSession* pClient,uint32_t docId);
	void UnlockMoveDoc(ClientSeasonSession* pClient,uint32_t docId);
	void UpdateMoveDoc(ClientSeasonSession* pClient,const MoveDocSnapshot& snapshot);
	void InsertMoveDoc(ClientSeasonSession* pClient,const MoveDocSnapshot& snapshot);
	void DeleteMoveDoc(ClientSeasonSession* pClient,uint32_t docId);	
	
	void ValidateSellDocSnapshot(const HentsDocSnapshot& snapshot);
	SellDocServerObject* FindSellDoc(uint32_t docId);
	SellDocServerObject* AcquireSellDoc(uint32_t docId);
	void LockSellDoc(ClientSeasonSession* pClient,uint32_t docId);
	void UnlockSellDoc(ClientSeasonSession* pClient,uint32_t docId);
	void UpdateSellDoc(ClientSeasonSession* pClient,const HentsDocSnapshot& snapshot);
	void InsertSellDoc(ClientSeasonSession* pClient,const HentsDocSnapshot& snapshot);
	void DeleteSellDoc(ClientSeasonSession* pClient,uint32_t docId);

	void ValidateOutDocSnapshot(const OwnDocSnapshot& snapshot);
	OutDocServerObject* AcquireOutDoc(uint32_t docId);
	OutDocServerObject* FindOutDoc(uint32_t docId);
	void LockOutDoc(ClientSeasonSession* pClient,uint32_t docId);
	void UnlockOutDoc(ClientSeasonSession* pClient,uint32_t docId);
	void UpdateOutDoc(ClientSeasonSession* pClient,const OwnDocSnapshot& snapshot);
	void InsertOutDoc(ClientSeasonSession* pClient,const OwnDocSnapshot& snapshot);
	void DeleteOutDoc(ClientSeasonSession* pClient,uint32_t docId);

	void ValidateInvoiceDetails(const InvoiceDetails& details);
	void ValidateInvoiceSnapshot(const InvoiceSnapshot& snapshot);

	void ThrowInvoiceNotFound(const InvoiceKey& key);
	void ThrowInvoiceLockNotObtained(uint32_t invoiceId);
	void ThrowIfInvoiceIsPaid(IInvoice* pInvoice);

	BuyInvoiceServerObject* FindBuyInvoice(uint32_t invoiceId);
	BuyInvoiceServerObject* AcquireBuyInvoice(uint32_t invoiceId);
	void LockBuyInvoice(ClientSeasonSession* pClient,uint32_t invoiceId);
	void UnlockBuyInvoice(ClientSeasonSession* pClient,uint32_t invoiceId);
	void InsertBuyDocsInvoice(ClientSeasonSession* pClient,const InvoiceSnapshot& snapshot);
	void UpdateBuyDocsInvoice(ClientSeasonSession* pClient,const InvoiceSnapshot& snapshot);
	void InsertBuyCowsInvoice(ClientSeasonSession* pClient,const InvoiceSnapshot& snapshot);
	void UpdateBuyCowsInvoice(ClientSeasonSession* pClient,const InvoiceSnapshot& snapshot);
	void DeleteBuyInvoice(ClientSeasonSession* pClient,uint32_t invoiceId);
	
	void InsertPurchase(IPurchase* purchase, const DateTime& docDt);
	uint32_t ObtainNextBuyInvoiceNo(Transaction transaction);
	uint32_t ObtainNextSellInvoiceNo(Transaction transaction);

	SellInvoiceServerObject* FindSellInvoice(uint32_t invoiceId);
	SellInvoiceServerObject* AcquireSellInvoice(uint32_t invoiceId);
	void LockSellInvoice(ClientSeasonSession* pClient,uint32_t invoiceId);
	void UnlockSellInvoice(ClientSeasonSession* pClient,uint32_t invoiceId);
	void InsertSellDocsInvoice(ClientSeasonSession* pClient,const InvoiceSnapshot& snapshot);
	void UpdateSellDocsInvoice(ClientSeasonSession* pClient,const InvoiceSnapshot& snapshot);
	void InsertSellCowsInvoice(ClientSeasonSession* pClient,const InvoiceSnapshot& snapshot);
	void UpdateSellCowsInvoice(ClientSeasonSession* pClient,const InvoiceSnapshot& snapshot);
	void DeleteSellInvoice(ClientSeasonSession* pClient,uint32_t invoiceId);
	
	void ThrowHentAlreadyLockedBy(IHent* pHent,ClientSeasonSession* pSession);
	void ThrowCowAlreadyLockedBy(ICow* pCow,ClientSeasonSession* pClient);
	void ThrowInvoiceAlreadyLocked(IInvoice* pInvoice,ClientSeasonSession* pClient);
	void ThrowDocAlreadyLocked(IDoc* pDoc,ClientSeasonSession* pClient);
	void ThrowDocNotFound(const DocKey& docKey);
	void ThrowCowNotFound(uint32_t cowId);	
	void ThrowCowLockNotObtained(uint32_t cowId);
	void ThrowIfThereAreInvoices(IBuyDoc* pDoc);
	void ThrowIfThereAreInvoices(ISellDoc* pDoc);
	void ThrowIfThereAreInvoices(IInDoc* pDoc);
	void ThrowIfThereAreInvoices(IOutDoc* pDoc);

	void ThrowFatalError(const std::wstring& file,int line, const std::wstring& message);
	
	void ValidateMandatoryForHent(IHent* hent);
	void ThrowHentNotFound(uint32_t hentId);
	void ThrowLockHighjack();

	SeasonSessionState GetSessionState();	
	void EnterFetchState();
	void ExitFetchState();
	void EnterUpdateState();
	void ExitUpdateState();
	void CloseFetchHandle(FetchHandleImpl* pHandle);

	void UnlockCowObjects(ClientSeasonSession* pClient,const std::vector<CowServerObject*>& cowObjs);
	void LockCowObjects(ClientSeasonSession* pClient,const std::vector<CowServerObject*>& cowObjs);

	void GetCowSOsForDoc(std::vector<CowServerObject*>& destination, const IDoc* pDoc);	
	std::vector<CowServerObject*> GetCowSOsForDoc(const IDoc* pDoc);	
	std::vector<CowServerObject*> GetCowSOsForInvoice(const IInvoice* pInvoice);
	std::set<uint32_t> GetEntryDocIdsForInvoice(const IInvoice* pInvoice);
	std::set<uint32_t> GetExitDocIdsForInvoice(const IInvoice* pInvoice);
	std::vector<BuyDocServerObject*> AcquireBuyDocSOs(IdRange ids);
	std::vector<SellDocServerObject*> AcquireSellDocSOs(IdRange ids);
	CowIds GetCowOccurences(const CowNo& cowNo);


	template<typename SO>
	bool Lock(ClientSeasonSession* pClient,SO* pSO, LockType lockType = LockType_Deep)
	{
		if(pSO->Lock(pClient, lockType))
		{
			m_lockRecord.RememberLock(pClient,pSO);
			return true;
		}
		return false;
	}

	template<typename SO>
	void Unlock(ClientSeasonSession* pClient,SO* pSO)
	{
		if (!pSO->Object())
		{
			BOOST_THROW_EXCEPTION(SessionException(L"trying to unlock SO witout content"));
		}
		if(pSO->Unlock(pClient))
		{
			m_lockRecord.ForgetLock(pClient,pSO);			
		}
	}

public:	
	ClientSeasonSessionFacade Open(ClientSession* pClient);		
	ISeason* GetSeason() const;
	uint16_t GetObserverPort() const;
	//IFarm* GetFarm() const;
protected:
	IClass* FindClass(uint32_t classId);
	IStock* FindStock(uint32_t stockId);
	IAgent* FindAgent(uint32_t agentId);

	IClass* FindClass(const std::wstring& classCd);
	IStock* FindStock(const std::wstring& stockCd);
	IHerd* FindHerdByIndex(int herdIndex);
	IHerd* FindHerdById(uint32_t herdId);
	IAgent* FindAgent(const std::wstring&  agentCd);


	FetchHandle OpenFetchHandle();
	void Do(ClientSeasonSession* pClient,std::function<void (SeasonSessionFacade*)>& action);
	
private:
	Season* m_pSeason;
	FarmSession* m_pFarmSession;
	SeasonSpectator* m_pSpectator;
	ChangePublisher* m_pChangePublisher;
	ClientSeasonSessionInterfaceImpl* m_pClientInterface;
	MuuDAO m_DAO;

	HentsSOIndex m_Hents;
		
	CowsSOIndex m_Cows;
	CowTracker m_Tracker;
	std::map<uint32_t,HerdRegistry*> m_Registers;
	CowNoReference m_CowNoReference;
	InDocsIndex m_InDocs;
	OutDocsIndex m_OutDocs;
	BuyDocsIndex m_BuyDocs;
	SellDocsIndex m_SellDocs;
	MoveDocsIndex m_MoveDocs;

	BuyInvoicesIndex m_BuyInvoices;
	InvoiceRelations m_BuyInvoiceRelations;

	SellInvoicesIndex m_SellInvoices;
	InvoiceRelations m_SellInvoiceRelations;
		
	SessionLockRecord m_lockRecord;
	std::set<ClientSeasonSession*> m_SeasonClients;
	boost::shared_mutex m_ClientMutex;

	SessionTokenGenerator* m_tokenGenerator;

	boost::condition_variable_any m_StateCV;
	boost::shared_mutex m_StateMutex;

	std::set<FetchHandleImpl*> m_FetchHandles;
	StateHolder m_State;

	boost::thread m_worker;
	boost::asio::io_service m_service;
	boost::asio::io_service::work m_work;

	BkLogger* m_logger;

	friend class ChangePublisher;
	friend class FetchHandleImpl;	
	friend class ScopedUpdateState;
	friend class SeasonSessionBuilder;	
	friend class SeasonSessionFinalizer;
	friend class SeasonSessionFacadeImpl;
	friend class ClientSeasonSessionInterfaceImpl;
	friend class InsertPurchaseOperation;
};
