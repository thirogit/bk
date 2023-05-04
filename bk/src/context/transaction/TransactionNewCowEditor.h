#ifndef __TRANSACTIONNEWCOWEDITOR_H__
#define __TRANSACTIONNEWCOWEDITOR_H__

#include "CowEditor.h"
#include "PlusDocTransactionFacade.h"

class TransactionNewCowEditor : public CowEditor
{
public:	
	TransactionNewCowEditor(PlusDocTransactionFacade* pTransactionFacade);
	virtual bool IsEditing() const;
	virtual void Save();	
private:
	PlusDocTransactionFacade* m_pTransactionFacade;
};

#endif