#pragma once

#include "HentView.h"
#include "../context/SeasonSession.h"

class FarmHentViewCreator 
{
public:
	FarmHentViewCreator(SeasonSession* facade);
	HentView* CreateView(uint32_t herdId);
	void RefreshView(HentView* pView,uint32_t herdId);
private:
	SeasonSession* m_pFacade;
};
