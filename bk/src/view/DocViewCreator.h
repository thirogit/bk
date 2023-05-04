#ifndef __DOCVIEWCREATOR_H__
#define __DOCVIEWCREATOR_H__

#include "../context/SeasonSession.h"

#include "DocView.h"

class DocViewCreator 
{
public:
	DocViewCreator(SeasonSession* facade);
	DocView* CreateView(const IDoc* pDoc);
	void 	 RefreshView(const IDoc* pDoc,DocView* pView);
private:
	SeasonSession* m_pFacade;
};


#endif