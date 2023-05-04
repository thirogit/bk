#ifndef __BUYDOCSCANDIDATESLISTCONTENT_H__
#define __BUYDOCSCANDIDATESLISTCONTENT_H__

#include "HentsDocsCandidatesListContent.h"


class BuyDocsCandidatesListContent : public HentsDocsCandidatesListContent
{
protected:
	virtual void CreateContent();
	virtual IHentsDoc* GetHentsDoc(uint32_t docId);
};

#endif