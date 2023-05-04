#pragma once
#include "HentsDocsListContent.h"

class BuyDocsListContent : public HentsDocsListContent
{
protected:
	virtual void CreateContent();
	virtual void OnHerdBuyDocInserted(uint32_t docId);
	virtual void OnHerdBuyDocUnlocked(uint32_t docId);
	virtual void OnHerdBuyDocLocked(uint32_t docId);
	void SetLocked(bool bLocked,uint32_t docId);
	virtual void OnHerdBuyDocUpdated(uint32_t docId);
	void OnHerdBuyDocDeleted(uint32_t docId);

};

