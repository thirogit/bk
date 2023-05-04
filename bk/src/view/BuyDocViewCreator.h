#ifndef __BUYDOCVIEWCREATOR_H__
#define __BUYDOCVIEWCREATOR_H__

#include "HentsDocView.h"
#include "HentsDocViewCreator.h"
#include <data\IBuyDoc.h>

class BuyDocViewCreator  
{
public:
	BuyDocViewCreator(SeasonSession* facade);
	HentsDocView* CreateView(IBuyDoc* pDoc);
	void RefreshView(HentsDocView* view,IBuyDoc* pDoc);
private:
	SeasonSession* m_pFacade;
	HentsDocViewCreator m_docViewCreator;
	

};


#endif