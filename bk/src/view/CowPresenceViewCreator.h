#pragma once

#include "CowPresenceView.h"
#include "../context/SeasonSession.h"
#include <logic\herdregistry\ICowPresence.h>
#include "HentViewCreator.h"

class CowPresenceViewCreator 
{
public:
	CowPresenceViewCreator(SeasonSession* facade);
	CowPresenceView* CreateView(ICowPresence* pHerdPresence);
	void RefreshView(CowPresenceView* pView,ICowPresence* pHerdPresence);
private:
	SeasonSession* m_pFacade;
	HentViewCreator m_hentViewCreator;
	

};
