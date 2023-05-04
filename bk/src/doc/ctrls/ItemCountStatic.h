#ifndef __ITEMCOUNTSTATIC_H__
#define __ITEMCOUNTSTATIC_H__

#include "../../context/transaction/DocTransactionFacade.h"
#include "../../context/transaction/EmptyInvoiceTransactionObserver.h"

class CItemCountStatic : public CStatic, protected EmptyInvoiceTransactionObserver
{
public:
	CItemCountStatic(DocTransactionFacade* pFacade);
	void UpdateCount();
	
protected:
	virtual void PreSubclassWindow();
	
	virtual void OnItemUpdated(ItemId id);
	virtual void OnItemAdded(ItemId id);
	virtual void OnItemDeleted(ItemId id);

	void SetItemCount(size_t count);

	void Register();
	void Unregister();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();

	DocTransactionFacade* m_pFacade;


	DECLARE_MESSAGE_MAP()
};

#endif