#ifndef __HERDOBSERVER_H__
#define __HERDOBSERVER_H__

#include <stdint.h>
#include <logic\herdregistry\CowPresenceKey.h>

class HerdObserver
{
public:
	
	virtual void  OnHerdCowLocked(const CowPresenceKey& key) = 0;
	virtual void  OnHerdCowUnlocked(const CowPresenceKey& key) = 0;
			
	virtual void  OnHerdCowAdded(const CowPresenceKey& key) = 0;
	virtual void  OnHerdCowDeleted(const CowPresenceKey& key) = 0;	
	virtual void  OnHerdCowMoved(const CowPresenceKey& fromKey,const CowPresenceKey& toKey) = 0;
	virtual void  OnHerdCowRemoved(const CowPresenceKey& key) = 0;
	virtual void  OnHerdCowRevoked(const CowPresenceKey& fromKey,const CowPresenceKey& toKey) = 0;

	virtual void  OnHerdCowUpdated(const CowPresenceKey& key) = 0;	

	virtual void OnHerdInDocLocked(uint32_t docId) = 0;
	virtual void OnHerdInDocUnlocked(uint32_t docId) = 0;	
	virtual void OnHerdInDocInserted(uint32_t docId) = 0;
	virtual void OnHerdInDocUpdated(uint32_t docId) = 0;
	virtual void OnHerdInDocDeleted(uint32_t docId) = 0;

	virtual void OnHerdBuyDocLocked(uint32_t docId) = 0;
	virtual void OnHerdBuyDocUnlocked(uint32_t docId) = 0;	
	virtual void OnHerdBuyDocInserted(uint32_t docId) = 0;
	virtual void OnHerdBuyDocUpdated(uint32_t docId) = 0;
	virtual void OnHerdBuyDocDeleted(uint32_t docId) = 0;	

	virtual void OnHerdMoveDocLocked(uint32_t docId) = 0;
	virtual void OnHerdMoveDocUnlocked(uint32_t docId) = 0;
	virtual void OnHerdMoveDocInserted(uint32_t docId) = 0;
	virtual void OnHerdMoveDocUpdated(uint32_t docId) = 0;
	virtual void OnHerdMoveDocDeleted(uint32_t docId) = 0;
		
	virtual void OnHerdSellDocLocked(uint32_t docId) = 0;
	virtual void OnHerdSellDocUnlocked(uint32_t docId) = 0;	
	virtual void OnHerdSellDocInserted(uint32_t docId) = 0;
	virtual void OnHerdSellDocUpdated(uint32_t docId) = 0;
	virtual void OnHerdSellDocDeleted(uint32_t docId) = 0;

	virtual void OnHerdOutDocLocked(uint32_t docId) = 0;
	virtual void OnHerdOutDocUnlocked(uint32_t docId) = 0;	
	virtual void OnHerdOutDocInserted(uint32_t docId) = 0;
	virtual void OnHerdOutDocUpdated(uint32_t docId) = 0;
	virtual void OnHerdOutDocDeleted(uint32_t docId) = 0;
	
	
	
};


#endif