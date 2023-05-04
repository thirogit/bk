#ifndef __FORWARDHERDOBSERVER_H__
#define __FORWARDHERDOBSERVER_H__

#include "HerdObserver.h"

class ForwardHerdObserver : public HerdObserver
{
public:
	
	virtual void  OnHerdCowLocked(const CowPresenceKey& key) {};
	virtual void  OnHerdCowUnlocked(const CowPresenceKey& key) {};
			
	virtual void  OnHerdCowAdded(const CowPresenceKey& key) {}; // through in/buy doc
	virtual void  OnHerdCowRemoved(const CowPresenceKey& key) {}; //via sell/out doc
	virtual void  OnHerdCowDeleted(const CowPresenceKey& key) {}; // canceling entry event in dest herd
	virtual void  OnHerdCowMoved(const CowPresenceKey& fromKey,const CowPresenceKey& toKey) {}; //via move doc
	virtual void  OnHerdCowUpdated(const CowPresenceKey& key) {};	
	virtual void  OnHerdCowRevoked(const CowPresenceKey& fromKey,const CowPresenceKey& toKey) {}; //canceling exit event in src herd

	virtual void OnHerdInDocLocked(uint32_t docId) {};
	virtual void OnHerdInDocUnlocked(uint32_t docId) {};	
	virtual void OnHerdInDocInserted(uint32_t docId) {};
	virtual void OnHerdInDocUpdated(uint32_t docId) {};
	virtual void OnHerdInDocDeleted(uint32_t docId) {};

	virtual void OnHerdBuyDocLocked(uint32_t docId) {};
	virtual void OnHerdBuyDocUnlocked(uint32_t docId) {};	
	virtual void OnHerdBuyDocInserted(uint32_t docId) {};
	virtual void OnHerdBuyDocUpdated(uint32_t docId) {};
	virtual void OnHerdBuyDocDeleted(uint32_t docId) {};

	virtual void OnHerdMoveDocLocked(uint32_t docId) {};
	virtual void OnHerdMoveDocUnlocked(uint32_t docId) {};
	virtual void OnHerdMoveDocInserted(uint32_t docId) {};
	virtual void OnHerdMoveDocUpdated(uint32_t docId) {};
	virtual void OnHerdMoveDocDeleted(uint32_t docId) {};

	virtual void OnHerdSellDocLocked(uint32_t docId) {};
	virtual void OnHerdSellDocUnlocked(uint32_t docId) {};	
	virtual void OnHerdSellDocInserted(uint32_t docId) {};
	virtual void OnHerdSellDocUpdated(uint32_t docId) {};
	virtual void OnHerdSellDocDeleted(uint32_t docId) {};
		
	virtual void OnHerdOutDocLocked(uint32_t docId) {};
	virtual void OnHerdOutDocUnlocked(uint32_t docId) {};	
	virtual void OnHerdOutDocInserted(uint32_t docId) {};
	virtual void OnHerdOutDocUpdated(uint32_t docId) {};
	virtual void OnHerdOutDocDeleted(uint32_t docId) {};
	
};


#endif