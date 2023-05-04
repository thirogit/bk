#pragma once

#include <stdint.h>
#include "SeasonSpectator.h"
#include <arrays/NewPtrFlushArray.h>
#include <boost/thread/mutex.hpp>
#include "service/notification/Notification.h"
#include <service/INotificationPublish.h>
#include <set>
#include <RCF/RcfServer.hpp>
#include "cloud/CloudPublisher.h"

class FarmAudience
{
	class SeasonSpectatorImpl;
	class ObserverTransactionImpl;

	class StreakImpl : public Streak
	{
		friend class ObserverTransactionImpl;
	public:
		~StreakImpl();
		virtual void OnHentLock(uint32_t hentId);
		virtual void OnHentUnlock(uint32_t hentId);
		virtual void OnHentInserted(IHent* pHent);
		virtual void OnHentUpdated(IHent* pHent);
		virtual void OnHentDeleted(uint32_t hentId);
		virtual void OnBeginFetch();

		virtual void OnCowLock(uint32_t cowId);
		virtual void OnCowUnlock(uint32_t cowId);
		virtual void OnCowInserted(ICow* pCow);
		virtual void OnCowUpdated(ICow* pCow);
		virtual void OnCowDeleted(uint32_t cowId);

		virtual void OnInDocLock(uint32_t docId);
		virtual void OnInDocUnlock(uint32_t docId);
		virtual void OnInDocInserted(IInDoc* pDoc);
		virtual void OnInDocDeleted(uint32_t docId);
		virtual void OnInDocUpdated(IInDoc* pDoc);

		virtual void OnBuyDocLock(uint32_t docId);
		virtual void OnBuyDocUnlock(uint32_t docId);
		virtual void OnBuyDocInserted(IBuyDoc* pDoc);
		virtual void OnBuyDocDeleted(uint32_t docId);
		virtual void OnBuyDocUpdated(IBuyDoc* pDoc);

		virtual void OnMoveDocLock(uint32_t docId);
		virtual void OnMoveDocUnlock(uint32_t docId);
		virtual void OnMoveDocInserted(IMoveDoc* pDoc);
		virtual void OnMoveDocDeleted(uint32_t docId);
		virtual void OnMoveDocUpdated(IMoveDoc* pDoc);

		virtual void OnSellDocLock(uint32_t docId);
		virtual void OnSellDocUnlock(uint32_t docId);
		virtual void OnSellDocInserted(ISellDoc* pDoc);
		virtual void OnSellDocDeleted(uint32_t docId);
		virtual void OnSellDocUpdated(ISellDoc* pDoc);

		virtual void OnOutDocLock(uint32_t docId);
		virtual void OnOutDocUnlock(uint32_t docId);
		virtual void OnOutDocInserted(IOutDoc* pDoc);
		virtual void OnOutDocDeleted(uint32_t docId);
		virtual void OnOutDocUpdated(IOutDoc* pDoc);

		virtual void OnBuyInvoiceLocked(uint32_t invoiceId);
		virtual void OnBuyInvoiceInserted(IBuyInvoice* pInvoice);
		virtual void OnBuyInvoiceUpdated(IBuyInvoice* pInvoice);
		virtual void OnBuyInvoiceDelete(uint32_t invoiceId);
		virtual void OnBuyInvoiceUnlocked(uint32_t invoiceId);

		virtual void OnSellInvoiceLocked(uint32_t invoiceId);
		virtual void OnSellInvoiceInserted(ISellInvoice* pInvoice);
		virtual void OnSellInvoiceUpdated(ISellInvoice* pInvoice);
		virtual void OnSellInvoiceDelete(uint32_t invoiceId);
		virtual void OnSellInvoiceUnlocked(uint32_t invoiceId);
			
	private:
		std::vector<Notification*> m_notifications;
	};
	

	class ObserverTransactionImpl : public ObserverTransaction
	{
	public:
		ObserverTransactionImpl(SeasonSpectatorImpl* pParent);
		virtual Streak* StartStreak(); 

		virtual void OnHentLock(uint32_t hentId);
		virtual void OnHentUnlock(uint32_t hentId);
		virtual void OnHentInserted(IHent* pHent);
		virtual void OnHentUpdated(IHent* pHent);
		virtual void OnHentDeleted(uint32_t hentId);
		virtual void OnBeginFetch();

		virtual void OnCowLock(uint32_t cowId);
		virtual void OnCowUnlock(uint32_t cowId);
		virtual void OnCowInserted(ICow* pCow);
		virtual void OnCowUpdated(ICow* pCow);
		virtual void OnCowDeleted(uint32_t cowId);

		virtual void OnInDocLock(uint32_t docId);
		virtual void OnInDocUnlock(uint32_t docId);
		virtual void OnInDocInserted(IInDoc* pDoc);
		virtual void OnInDocDeleted(uint32_t docId);
		virtual void OnInDocUpdated(IInDoc* pDoc);

		virtual void OnBuyDocLock(uint32_t docId);
		virtual void OnBuyDocUnlock(uint32_t docId);
		virtual void OnBuyDocInserted(IBuyDoc* pDoc);
		virtual void OnBuyDocDeleted(uint32_t docId);
		virtual void OnBuyDocUpdated(IBuyDoc* pDoc);

		virtual void OnMoveDocLock(uint32_t docId);
		virtual void OnMoveDocUnlock(uint32_t docId);
		virtual void OnMoveDocInserted(IMoveDoc* pDoc);
		virtual void OnMoveDocDeleted(uint32_t docId);
		virtual void OnMoveDocUpdated(IMoveDoc* pDoc);

		virtual void OnSellDocLock(uint32_t docId);
		virtual void OnSellDocUnlock(uint32_t docId);
		virtual void OnSellDocInserted(ISellDoc* pDoc);
		virtual void OnSellDocDeleted(uint32_t docId);
		virtual void OnSellDocUpdated(ISellDoc* pDoc);

		virtual void OnOutDocLock(uint32_t docId);
		virtual void OnOutDocUnlock(uint32_t docId);
		virtual void OnOutDocInserted(IOutDoc* pDoc);
		virtual void OnOutDocDeleted(uint32_t docId);
		virtual void OnOutDocUpdated(IOutDoc* pDoc);

		virtual void OnBuyInvoiceLocked(uint32_t invoiceId);
		virtual void OnBuyInvoiceInserted(IBuyInvoice* pInvoice);
		virtual void OnBuyInvoiceUpdated(IBuyInvoice* pInvoice);
		virtual void OnBuyInvoiceDelete(uint32_t invoiceId);
		virtual void OnBuyInvoiceUnlocked(uint32_t invoiceId);

		virtual void OnSellInvoiceLocked(uint32_t invoiceId);
		virtual void OnSellInvoiceInserted(ISellInvoice* pInvoice);
		virtual void OnSellInvoiceUpdated(ISellInvoice* pInvoice);
		virtual void OnSellInvoiceDelete(uint32_t invoiceId);
		virtual void OnSellInvoiceUnlocked(uint32_t invoiceId);


		virtual void Commit();
		virtual void Rollback();
		virtual ~ObserverTransactionImpl();
		
	private:
		std::vector<StreakImpl*> m_streaks;
		SeasonSpectatorImpl* m_pParent;
	};


	class SeasonSpectatorImpl : public SeasonSpectator
	{
	public:
		SeasonSpectatorImpl(uint32_t seasonId);
		~SeasonSpectatorImpl();
		
		virtual uint16_t GetPort();
		virtual void Release();
		virtual ObserverTransactionPtr OpenTransaction();
	protected:
		void Post(Notification& notification);
		void PostMany(const std::vector<Notification*>& notifications);
	private:
		RCF::RcfServer m_server;		
		boost::shared_ptr<RCF::Publisher<I_NotificationPublish>> m_publisher;
		FarmAudience* m_pOwner;
		uint32_t m_seasonId;
		friend class FarmAudience;
	};

public:
	FarmAudience(const FarmNo& farmNo,CloudPublisher* cloudPublisher);
	~FarmAudience();

	void Finish();	
	SeasonSpectator* ObtainSpectator(uint32_t seasonId);

protected:
	void ReleaseSpectator(SeasonSpectatorImpl* pSpectator);
private:
	
	std::set<SeasonSpectatorImpl*> m_Audience;
		
	boost::mutex m_AudienceMutex;

	CloudPublisher* m_cloudPublisher;
	FarmNo m_farmNo;

	friend class SeasonSpectatorImpl;

};
