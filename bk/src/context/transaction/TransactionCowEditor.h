#ifndef __TRANSACTIONCOWEDITOR_H__
#define __TRANSACTIONCOWEDITOR_H__

#include "CowEditor.h"
#include <data\runtime\ItemId.h>
#include "PlusDocTransactionFacade.h"

class TransactionCowEditor : public CowEditor
{
public:	
	TransactionCowEditor(PlusDocTransactionFacade* pTransaction);	
	void Edit(ItemId id);
	virtual bool IsEditing() const;
	virtual void Save();			

private:
	PlusDocTransactionFacade* m_pTransaction;
	ItemId m_itemId;
};

#endif