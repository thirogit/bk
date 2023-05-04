#ifndef __HENTSDOCVIEWCREATOR_H__
#define __HENTSDOCVIEWCREATOR_H__

#include "HentsDocView.h"
#include "DocViewCreator.h"
#include <data\IHentsDoc.h>

class HentsDocViewCreator  
{
public:
	HentsDocViewCreator(SeasonSession* facade);
	void RefreshView(HentsDocView* pView,IHentsDoc* pDoc);
	HentsDocView* CreateView(IHentsDoc* pDoc);
private:
	SeasonSession* m_pFacade;
	DocViewCreator m_docViewCreator;
	

};


#endif