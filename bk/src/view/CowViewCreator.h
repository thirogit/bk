#pragma once

#include "CowView.h"
#include "../context/SeasonSession.h"

class CowViewCreator 
{
public:
	CowViewCreator(SeasonSession* facade);
	CowView* CreateView(ICow* pCow);
	void RefreshView(CowView* pView,ICow* pCow);
private:
	SeasonSession* m_pFacade;
};
