#include "stdafx.h"
#include "FarmAudience.h"
#include <service/notification/LockNotifications.h>
#include <service/notification/UnlockNotifications.h>
#include "SessionException.h"
#include "service/notification/NotificationEnvelope.h"
#include <RCF/TcpEndpoint.hpp>

#include <service/notification/HentNotifications.h>
#include <service/notification/DeleteNotifications.h>
#include <service/notification/DocNotifications.h>
#include <service/notification/CowNotifications.h>
#include <service/notification/InvoiceNotifications.h>
#include <arrays/DeepDelete.h>
#include <boost/foreach.hpp>

void FarmAudience::StreakImpl::OnHentLock(uint32_t hentId)
{
	m_notifications.push_back(new HentLockNotification(hentId,0));	
}

void FarmAudience::StreakImpl::OnHentUnlock(uint32_t hentId)
{
	m_notifications.push_back(new HentUnlockNotification(hentId));	
}

void FarmAudience::StreakImpl::OnHentInserted(IHent* pHent)
{
	m_notifications.push_back(new HentInsertNotification(pHent));
	
}

void FarmAudience::StreakImpl::OnHentUpdated(IHent* pHent)
{
	m_notifications.push_back(new HentUpdateNotification(pHent));
	
}

void FarmAudience::StreakImpl::OnHentDeleted(uint32_t hentId)
{
	m_notifications.push_back(new HentDeleteNotification(hentId));
	
}

void FarmAudience::StreakImpl::OnBeginFetch()
{
}

void FarmAudience::StreakImpl::OnCowLock(uint32_t cowId)
{
	m_notifications.push_back(new CowLockNotification(cowId,0));
	
}

void FarmAudience::StreakImpl::OnCowUnlock(uint32_t cowId)
{
	m_notifications.push_back(new CowUnlockNotification(cowId));
	
}

void FarmAudience::StreakImpl::OnCowInserted(ICow* pCow)
{
	m_notifications.push_back(new CowInsertNotification(pCow));
	
}

void FarmAudience::StreakImpl::OnCowUpdated(ICow* pCow)
{
	m_notifications.push_back(new CowUpdateNotification(pCow));
	
}

void FarmAudience::StreakImpl::OnCowDeleted(uint32_t cowId)
{
	m_notifications.push_back(new CowDeleteNotification(cowId));
	
}

void FarmAudience::StreakImpl::OnInDocLock(uint32_t docId)
{
	m_notifications.push_back(new InDocLockNotification(docId,0));
	
}

void FarmAudience::StreakImpl::OnInDocUnlock(uint32_t docId)
{
	m_notifications.push_back(new InDocUnlockNotification(docId));
	
}

void FarmAudience::StreakImpl::OnInDocInserted(IInDoc* pInDoc)
{
	m_notifications.push_back(new InDocInsertNotification(pInDoc));
	
}

void FarmAudience::StreakImpl::OnInDocDeleted(uint32_t docId)
{
	m_notifications.push_back(new InDocDeleteNotification(docId));
	
}

void FarmAudience::StreakImpl::OnInDocUpdated(IInDoc* pInDoc)
{
	m_notifications.push_back(new InDocUpdateNotification(pInDoc));
	
}

void FarmAudience::StreakImpl::OnBuyDocLock(uint32_t docId)
{
	m_notifications.push_back(new BuyDocLockNotification(docId,0));
	
}

void FarmAudience::StreakImpl::OnBuyDocUnlock(uint32_t docId)
{
	m_notifications.push_back(new BuyDocUnlockNotification(docId));
	
}

void FarmAudience::StreakImpl::OnBuyDocInserted(IBuyDoc* pDoc)
{
	m_notifications.push_back(new BuyDocInsertNotification(pDoc));
	
}

void FarmAudience::StreakImpl::OnBuyDocDeleted(uint32_t docId)
{
	m_notifications.push_back(new BuyDocDeleteNotification(docId));
	
}

void FarmAudience::StreakImpl::OnBuyDocUpdated(IBuyDoc* pDoc)
{
	m_notifications.push_back(new BuyDocUpdateNotification(pDoc));
	
}

void FarmAudience::StreakImpl::OnMoveDocLock(uint32_t docId)
{
	m_notifications.push_back(new MoveDocLockNotification(docId,0));
	
}
void FarmAudience::StreakImpl::OnMoveDocUnlock(uint32_t docId)
{
	m_notifications.push_back(new MoveDocUnlockNotification(docId));
	
}
void FarmAudience::StreakImpl::OnMoveDocInserted(IMoveDoc* pDoc)
{
	m_notifications.push_back(new MoveDocInsertNotification(pDoc));
	
}
void FarmAudience::StreakImpl::OnMoveDocDeleted(uint32_t docId)
{
	m_notifications.push_back(new MoveDocDeleteNotification(docId));
	
}
void FarmAudience::StreakImpl::OnMoveDocUpdated(IMoveDoc* pDoc)
{
	m_notifications.push_back(new MoveDocUpdateNotification(pDoc));
	
}

void FarmAudience::StreakImpl::OnSellDocLock(uint32_t docId)
{
	m_notifications.push_back(new SellDocLockNotification(docId,NULL_ID));
	
}
void FarmAudience::StreakImpl::OnSellDocUnlock(uint32_t docId)
{
	m_notifications.push_back(new SellDocUnlockNotification(docId));
	
}
void FarmAudience::StreakImpl::OnSellDocInserted(ISellDoc* pDoc)
{
	m_notifications.push_back(new SellDocInsertNotification(pDoc));
	
}
void FarmAudience::StreakImpl::OnSellDocDeleted(uint32_t docId)
{
	m_notifications.push_back(new SellDocDeleteNotification(docId));
	
}
void FarmAudience::StreakImpl::OnSellDocUpdated(ISellDoc* pDoc)
{
	m_notifications.push_back(new SellDocUpdateNotification(pDoc));
	
}
void FarmAudience::StreakImpl::OnOutDocLock(uint32_t docId)
{
	m_notifications.push_back(new OutDocLockNotification(docId,0));
	
}
void FarmAudience::StreakImpl::OnOutDocUnlock(uint32_t docId)
{
	m_notifications.push_back(new OutDocUnlockNotification(docId));
	
}
void FarmAudience::StreakImpl::OnOutDocInserted(IOutDoc* pDoc)
{
	m_notifications.push_back(new OutDocInsertNotification(pDoc));
	
}
void FarmAudience::StreakImpl::OnOutDocDeleted(uint32_t docId)
{
	m_notifications.push_back(new OutDocDeleteNotification(docId));
	
}
void FarmAudience::StreakImpl::OnOutDocUpdated(IOutDoc* pDoc)
{
	m_notifications.push_back(new OutDocUpdateNotification(pDoc));	
}

void FarmAudience::StreakImpl::OnBuyInvoiceLocked(uint32_t invoiceId)
{
	m_notifications.push_back(new BuyInvoiceLockNotification(invoiceId,0));
	
}

void FarmAudience::StreakImpl::OnBuyInvoiceInserted(IBuyInvoice* pInvoice)
{
	m_notifications.push_back(new BuyInvoiceInsertNotification(pInvoice));
	
}

void FarmAudience::StreakImpl::OnBuyInvoiceUpdated(IBuyInvoice* pInvoice)
{
	m_notifications.push_back(new BuyInvoiceUpdateNotification(pInvoice));
	
}

void FarmAudience::StreakImpl::OnBuyInvoiceDelete(uint32_t invoiceId)
{
	m_notifications.push_back(new BuyInvoiceDeleteNotification(invoiceId));
	
}

void FarmAudience::StreakImpl::OnBuyInvoiceUnlocked(uint32_t invoiceId)
{
	m_notifications.push_back(new BuyInvoiceUnlockNotification(invoiceId));
	
}

void FarmAudience::StreakImpl::OnSellInvoiceLocked(uint32_t invoiceId)
{
	m_notifications.push_back(new SellInvoiceLockNotification(invoiceId,0));
	
}

void FarmAudience::StreakImpl::OnSellInvoiceInserted(ISellInvoice* pInvoice)
{
	m_notifications.push_back(new SellInvoiceInsertNotification(pInvoice));
	
}

void FarmAudience::StreakImpl::OnSellInvoiceUpdated(ISellInvoice* pInvoice)
{
	m_notifications.push_back(new SellInvoiceUpdateNotification(pInvoice));
	
}

void FarmAudience::StreakImpl::OnSellInvoiceDelete(uint32_t invoiceId)
{
	m_notifications.push_back(new SellInvoiceDeleteNotification(invoiceId));
	
}

void FarmAudience::StreakImpl::OnSellInvoiceUnlocked(uint32_t invoiceId)
{
	m_notifications.push_back(new SellInvoiceUnlockNotification(invoiceId));	
}

FarmAudience::StreakImpl::~StreakImpl()
{
	DeepDelete(m_notifications);
}

//-------------------------------------------------------------------------------------------------

FarmAudience::ObserverTransactionImpl::ObserverTransactionImpl(SeasonSpectatorImpl* pParent) : m_pParent(pParent)
{	
}

FarmAudience::ObserverTransactionImpl::~ObserverTransactionImpl()
{
	DeepDelete(m_streaks);	
}

void FarmAudience::ObserverTransactionImpl::Commit()
{	
	
	NotificationEnvelope* envelope = new NotificationEnvelope();

	BOOST_FOREACH(StreakImpl* streak,m_streaks)
	{
		BOOST_FOREACH(Notification* n,streak->m_notifications)
		{
			if (m_pParent->m_pOwner->m_cloudPublisher != NULL)
			{
				m_pParent->m_pOwner->m_cloudPublisher->Publish(m_pParent->m_pOwner->m_farmNo, m_pParent->m_seasonId, n);
			}
			envelope->Insert(n);
		}
	}
	
	if (!envelope->IsEmpty())
	{
		m_pParent->Post(*envelope);
	}

	delete envelope;
	Rollback();
}

void FarmAudience::ObserverTransactionImpl::Rollback()
{
	DeepDelete(m_streaks);	
	m_streaks.clear();	
}

Streak* FarmAudience::ObserverTransactionImpl::StartStreak()
{
	StreakImpl* streak = new StreakImpl();
	m_streaks.push_back(streak);
	return streak;
}

void FarmAudience::ObserverTransactionImpl::OnHentLock(uint32_t hentId)
{
	StartStreak()->OnHentLock(hentId);
}

void FarmAudience::ObserverTransactionImpl::OnHentUnlock(uint32_t hentId)
{
	StartStreak()->OnHentUnlock(hentId);
}

void FarmAudience::ObserverTransactionImpl::OnHentInserted(IHent* pHent)
{
	StartStreak()->OnHentInserted(pHent);
}

void FarmAudience::ObserverTransactionImpl::OnHentUpdated(IHent* pHent)
{
	StartStreak()->OnHentUpdated(pHent);
}

void FarmAudience::ObserverTransactionImpl::OnHentDeleted(uint32_t hentId)
{
	StartStreak()->OnHentDeleted(hentId);
}

void FarmAudience::ObserverTransactionImpl::OnBeginFetch()
{
	StartStreak()->OnBeginFetch();
}

void FarmAudience::ObserverTransactionImpl::OnCowLock(uint32_t cowId)
{
	StartStreak()->OnCowLock(cowId);
}

void FarmAudience::ObserverTransactionImpl::OnCowUnlock(uint32_t cowId)
{
	StartStreak()->OnCowUnlock(cowId);
}

void FarmAudience::ObserverTransactionImpl::OnCowInserted(ICow* pCow)
{
	StartStreak()->OnCowInserted(pCow);
}

void FarmAudience::ObserverTransactionImpl::OnCowUpdated(ICow* pCow)
{
	StartStreak()->OnCowUpdated(pCow);
}

void FarmAudience::ObserverTransactionImpl::OnCowDeleted(uint32_t cowId)
{
	StartStreak()->OnCowDeleted(cowId);
}

void FarmAudience::ObserverTransactionImpl::OnInDocLock(uint32_t docId)
{
	StartStreak()->OnInDocLock(docId);
}

void FarmAudience::ObserverTransactionImpl::OnInDocUnlock(uint32_t docId)
{
	StartStreak()->OnInDocUnlock(docId);
}

void FarmAudience::ObserverTransactionImpl::OnInDocInserted(IInDoc* pInDoc)
{
	StartStreak()->OnInDocInserted(pInDoc);
}

void FarmAudience::ObserverTransactionImpl::OnInDocDeleted(uint32_t docId)
{
	StartStreak()->OnInDocDeleted(docId);
}

void FarmAudience::ObserverTransactionImpl::OnInDocUpdated(IInDoc* pInDoc)
{
	StartStreak()->OnInDocUpdated(pInDoc);
}

void FarmAudience::ObserverTransactionImpl::OnBuyDocLock(uint32_t docId)
{
	StartStreak()->OnBuyDocLock(docId);
}

void FarmAudience::ObserverTransactionImpl::OnBuyDocUnlock(uint32_t docId)
{
	StartStreak()->OnBuyDocUnlock(docId);
}

void FarmAudience::ObserverTransactionImpl::OnBuyDocInserted(IBuyDoc* pDoc)
{
	StartStreak()->OnBuyDocInserted(pDoc);
}

void FarmAudience::ObserverTransactionImpl::OnBuyDocDeleted(uint32_t docId)
{
	StartStreak()->OnBuyDocDeleted(docId);
}

void FarmAudience::ObserverTransactionImpl::OnBuyDocUpdated(IBuyDoc* pDoc)
{
	StartStreak()->OnBuyDocUpdated(pDoc);
}

void FarmAudience::ObserverTransactionImpl::OnMoveDocLock(uint32_t docId)
{
	StartStreak()->OnMoveDocLock(docId);
}

void FarmAudience::ObserverTransactionImpl::OnMoveDocUnlock(uint32_t docId)
{
	StartStreak()->OnMoveDocUnlock(docId);
}

void FarmAudience::ObserverTransactionImpl::OnMoveDocInserted(IMoveDoc* pDoc)
{
	StartStreak()->OnMoveDocInserted(pDoc);
}

void FarmAudience::ObserverTransactionImpl::OnMoveDocDeleted(uint32_t docId)
{
	StartStreak()->OnMoveDocDeleted(docId);
}

void FarmAudience::ObserverTransactionImpl::OnMoveDocUpdated(IMoveDoc* pDoc)
{
	StartStreak()->OnMoveDocUpdated(pDoc);
}

void FarmAudience::ObserverTransactionImpl::OnSellDocLock(uint32_t docId)
{
	StartStreak()->OnSellDocLock(docId);
}

void FarmAudience::ObserverTransactionImpl::OnSellDocUnlock(uint32_t docId)
{
	StartStreak()->OnSellDocUnlock(docId);
}

void FarmAudience::ObserverTransactionImpl::OnSellDocInserted(ISellDoc* pDoc)
{
	StartStreak()->OnSellDocInserted(pDoc);
}

void FarmAudience::ObserverTransactionImpl::OnSellDocDeleted(uint32_t docId)
{
	StartStreak()->OnSellDocDeleted(docId);
}

void FarmAudience::ObserverTransactionImpl::OnSellDocUpdated(ISellDoc* pDoc)
{
	StartStreak()->OnSellDocUpdated(pDoc);
}

void FarmAudience::ObserverTransactionImpl::OnOutDocLock(uint32_t docId)
{
	StartStreak()->OnOutDocLock(docId);
}

void FarmAudience::ObserverTransactionImpl::OnOutDocUnlock(uint32_t docId)
{
	StartStreak()->OnOutDocUnlock(docId);
}

void FarmAudience::ObserverTransactionImpl::OnOutDocInserted(IOutDoc* pDoc)
{
	StartStreak()->OnOutDocInserted(pDoc);
}

void FarmAudience::ObserverTransactionImpl::OnOutDocDeleted(uint32_t docId)
{
	StartStreak()->OnOutDocDeleted(docId);
}

void FarmAudience::ObserverTransactionImpl::OnOutDocUpdated(IOutDoc* pDoc)
{
	StartStreak()->OnOutDocUpdated(pDoc);
}

void FarmAudience::ObserverTransactionImpl::OnBuyInvoiceLocked(uint32_t invoiceId)
{
	StartStreak()->OnBuyInvoiceLocked(invoiceId);
}

void FarmAudience::ObserverTransactionImpl::OnBuyInvoiceInserted(IBuyInvoice* pInvoice)
{
	StartStreak()->OnBuyInvoiceInserted(pInvoice);
}

void FarmAudience::ObserverTransactionImpl::OnBuyInvoiceUpdated(IBuyInvoice* pInvoice)
{
	StartStreak()->OnBuyInvoiceUpdated(pInvoice);
}

void FarmAudience::ObserverTransactionImpl::OnBuyInvoiceDelete(uint32_t invoiceId)
{
	StartStreak()->OnBuyInvoiceDelete(invoiceId);
}

void FarmAudience::ObserverTransactionImpl::OnBuyInvoiceUnlocked(uint32_t invoiceId)
{
	StartStreak()->OnBuyInvoiceUnlocked(invoiceId);
}

void FarmAudience::ObserverTransactionImpl::OnSellInvoiceLocked(uint32_t invoiceId)
{
	StartStreak()->OnSellInvoiceLocked(invoiceId);
}

void FarmAudience::ObserverTransactionImpl::OnSellInvoiceInserted(ISellInvoice* pInvoice)
{
	StartStreak()->OnSellInvoiceInserted(pInvoice);
}

void FarmAudience::ObserverTransactionImpl::OnSellInvoiceUpdated(ISellInvoice* pInvoice)
{
	StartStreak()->OnSellInvoiceUpdated(pInvoice);
}

void FarmAudience::ObserverTransactionImpl::OnSellInvoiceDelete(uint32_t invoiceId)
{
	StartStreak()->OnSellInvoiceDelete(invoiceId);
}

void FarmAudience::ObserverTransactionImpl::OnSellInvoiceUnlocked(uint32_t invoiceId)
{
	StartStreak()->OnSellInvoiceUnlocked(invoiceId);
}

//-------------------------------------------------------------------------------------------------

FarmAudience::SeasonSpectatorImpl::SeasonSpectatorImpl(uint32_t seasonId) : m_server(RCF::TcpEndpoint("0.0.0.0",0)),m_pOwner(NULL), m_seasonId(seasonId)
{
	
}

FarmAudience::SeasonSpectatorImpl::~SeasonSpectatorImpl() 
{
}

ObserverTransactionPtr FarmAudience::SeasonSpectatorImpl::OpenTransaction()
{
	return ObserverTransactionPtr(new ObserverTransactionImpl(this));
}

uint16_t FarmAudience::SeasonSpectatorImpl::GetPort()
{
	return m_server.getIpServerTransport().getPort();
}

void FarmAudience::SeasonSpectatorImpl::Release()
{
	m_pOwner->ReleaseSpectator(this);
}

void FarmAudience::SeasonSpectatorImpl::Post(Notification& notification)
{
	m_publisher->publish().Notify(NotificationObj(notification.Clone()));
}

FarmAudience::FarmAudience(const FarmNo& farmNo, CloudPublisher* cloudPublisher) : m_cloudPublisher(cloudPublisher),m_farmNo(farmNo)
{
}

FarmAudience::~FarmAudience()
{
}

SeasonSpectator* FarmAudience::ObtainSpectator(uint32_t seasonId)
{
	boost::mutex::scoped_lock lock(m_AudienceMutex);
		
	SeasonSpectatorImpl* pNewSpectator = new SeasonSpectatorImpl(seasonId);
	pNewSpectator->m_pOwner = this;
	m_Audience.insert(pNewSpectator);
	
	pNewSpectator->m_server.start();
	pNewSpectator->m_publisher = pNewSpectator->m_server.createPublisher<I_NotificationPublish>();	
		
	return pNewSpectator;

}

void FarmAudience::Finish()
{
	boost::mutex::scoped_lock lock(m_AudienceMutex);

	BOOST_FOREACH(SeasonSpectatorImpl* pSpectator,m_Audience)	
	{
		pSpectator->m_publisher->close();
		pSpectator->m_server.stop();
		delete pSpectator;		
	}
	m_Audience.clear();
}

void FarmAudience::ReleaseSpectator(SeasonSpectatorImpl* pSpectator)
{
	boost::mutex::scoped_lock lock(m_AudienceMutex);

	auto spectatorIt = m_Audience.find(pSpectator);
	if(spectatorIt != m_Audience.end())
	{
		SeasonSpectatorImpl* pSpectator = *spectatorIt;		
		pSpectator->m_publisher->close();
		pSpectator->m_server.stop();
		delete pSpectator;
		m_Audience.erase(spectatorIt);
	}


}
