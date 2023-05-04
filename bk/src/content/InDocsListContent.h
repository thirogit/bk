#pragma once
#include "OwnDocsListContent.h"


class InDocsListContent : public OwnDocsListContent
{
protected:
	virtual void CreateContent();
	virtual void OnHerdInDocLocked(uint32_t docId);
	virtual void OnHerdInDocUnlocked(uint32_t docId);	
	virtual void OnHerdInDocInserted(uint32_t docId);
	virtual void OnHerdInDocUpdated(uint32_t docId);
	virtual void OnHerdInDocDeleted(uint32_t docId);

	void SetLocked(bool bLocked,uint32_t docId);
	
};


