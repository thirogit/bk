#pragma once

#include "HentView.h"
#include "../context/SeasonSession.h"

class HentViewCreator 
{
public:
	HentViewCreator(SeasonSession* facade);
	HentView* CreateView(HentClientObject* pHentCO);
	void RefreshView(HentView* pView,HentClientObject* pHentCO);
	void RefreshView(HentView* pView,const IInvoiceHent* pHent);
private:
	SeasonSession* m_pFacade;
};
