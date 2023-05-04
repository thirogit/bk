#ifndef __SELLDOCVIEWCREATOR_H__
#define __SELLDOCVIEWCREATOR_H__

#include "HentsDocView.h"
#include "HentsDocViewCreator.h"
#include <data\ISellDoc.h>

class SellDocViewCreator  
{
public:
	SellDocViewCreator(SeasonSession* facade);
	HentsDocView* CreateView(ISellDoc* pDoc);
	void RefreshView(HentsDocView* view,ISellDoc* pDoc);
private:
	SeasonSession* m_pFacade;
	HentsDocViewCreator m_docViewCreator;
	

};


#endif