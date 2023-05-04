#ifndef __FATHENTSDOCVIEWCREATOR_H__
#define __FATHENTSDOCVIEWCREATOR_H__

#include "FatHentsDocView.h"
#include "HentsDocViewCreator.h"
#include "CowPresenceViewCreator.h"

class FatHentsDocViewCreator  
{
public:
	FatHentsDocViewCreator(SeasonSession* facade);
	FatHentsDocView* CreateView(const IHentsDoc* pDoc);
	void RefreshView(FatHentsDocView*pView,const IHentsDoc* pDoc);
private:
	SeasonSession* m_pFacade;
	HentsDocViewCreator m_docViewCreator;
	CowPresenceViewCreator m_presenceViewCreator;
	

};


#endif