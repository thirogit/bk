#pragma once
#include "OwnDocsListContent.h"

class OutDocsListContent : public OwnDocsListContent
{
protected:
	virtual void CreateContent();
	virtual void OnHerdOutDocInserted(uint32_t docId);	
	virtual void OnHerdOutDocDeleted(uint32_t docId);
	virtual void OnHerdOutDocLocked(uint32_t docId);
	virtual void OnHerdOutDocUnlocked(uint32_t docId);	
	virtual void OnHerdOutDocUpdated(uint32_t docId);
	void SetLocked(bool bLocked,uint32_t docId);

};


