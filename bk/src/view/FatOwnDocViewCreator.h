#ifndef __FATOWNDOCVIEWCREATOR_H__
#define __FATOWNDOCVIEWCREATOR_H__

#include "FatDocView.h"
#include "OwnDocViewCreator.h"
#include "CowPresenceViewCreator.h"

typedef FatDocView<OwnDocView> FatOwnDocView;

class FatOwnDocViewCreator  
{
public:
	FatOwnDocViewCreator(SeasonSession* facade);
	FatOwnDocView* CreateView(const IOwnDoc* pDoc);
	void RefreshView(FatOwnDocView*pView,const IOwnDoc* pDoc);
private:
	SeasonSession* m_pFacade;
	OwnDocViewCreator m_docViewCreator;
	CowPresenceViewCreator m_presenceViewCreator;
	

};


#endif