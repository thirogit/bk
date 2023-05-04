#ifndef __OWNDOCVIEWCREATOR_H__
#define __OWNDOCVIEWCREATOR_H__

#include "OwnDocView.h"
#include "DocViewCreator.h"
#include <data\IOwnDoc.h>

class OwnDocViewCreator  
{
public:
	OwnDocViewCreator(SeasonSession* facade);
	OwnDocView* CreateView(const IOwnDoc* pDoc);
	void RefreshView(OwnDocView*pView,const IOwnDoc* pDoc);
private:
	SeasonSession* m_pFacade;
	DocViewCreator m_docViewCreator;
	

};


#endif