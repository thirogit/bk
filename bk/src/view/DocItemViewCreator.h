#ifndef __DOCITEMVIEWCREATOR_H__
#define __DOCITEMVIEWCREATOR_H__

#include "../context/SeasonSession.h"

#include "DocItemView.h"
#include "../context/transaction/DocItem.h"

class DocItemViewCreator 
{
public:
	DocItemViewCreator(SeasonSession* facade);
	DocItemView* CreateView(const DocItem* pItem);
	void 	 RefreshView(const DocItem* pItem,DocItemView* pView);	
private:
	SeasonSession* m_pFacade;
};


#endif