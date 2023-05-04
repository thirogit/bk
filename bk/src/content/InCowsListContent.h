#pragma once
#include "CowsListContent.h"
#include "../context/SeasonSession.h"

class InCowsListContent : public CowsListContent
{
protected:
	virtual void CreateContent();	
	virtual void  OnHerdCowMoved(const CowPresenceKey& fromKey,const CowPresenceKey& toKey);
	virtual void  OnHerdCowRemoved(const CowPresenceKey& key);
	virtual void  OnHerdCowRevoked(const CowPresenceKey& fromKey,const CowPresenceKey& toKey);
};


