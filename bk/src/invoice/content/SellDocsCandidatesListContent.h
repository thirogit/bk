#ifndef __SELLDOCSCANDIDATESLISTCONTENT_H__
#define __SELLDOCSCANDIDATESLISTCONTENT_H__

#include "HentsDocsCandidatesListContent.h"

class SellDocsCandidatesListContent : public HentsDocsCandidatesListContent
{	
protected:
	virtual void CreateContent();
	virtual IHentsDoc* GetHentsDoc(uint32_t docId);
};

#endif