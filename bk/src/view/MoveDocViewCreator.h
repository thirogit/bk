#ifndef __MOVEDOCVIEWCREATOR_H__
#define __MOVEDOCVIEWCREATOR_H__

#include "MoveDocView.h"
#include "DocViewCreator.h"
#include <data\IMoveDoc.h>

class MoveDocViewCreator  
{
public:
	MoveDocViewCreator(SeasonSession* facade);
	MoveDocView* CreateView(const IMoveDoc* pDoc);
	void RefreshView(MoveDocView* pView,const IMoveDoc* pDoc);

private:
	SeasonSession* m_pFacade;
	DocViewCreator m_docViewCreator;
	

};


#endif