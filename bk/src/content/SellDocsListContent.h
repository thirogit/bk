#pragma once
#include "HentsDocsListContent.h"

class SellDocsListContent : public HentsDocsListContent
{
protected:
	virtual void CreateContent();
	virtual void OnHerdSellDocInserted(uint32_t docId);
	virtual void OnHerdSellDocDeleted(uint32_t docId);
	virtual void OnHerdSellDocUpdated(uint32_t docId);
	virtual void OnHerdSellDocLocked(uint32_t docId);
	virtual void OnHerdSellDocUnlocked(uint32_t docId);	

	void SetLocked(bool bLocked,uint32_t docId);



};


