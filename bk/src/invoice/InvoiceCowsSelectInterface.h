#ifndef __INVOICECOWSSELECTINTERFACE_H__
#define __INVOICECOWSSELECTINTERFACE_H__

#include "../context/SeasonSession.h"
#include "../cow/CowSelectInterface.h"
#include "content/CowCandidatesContent.h"
#include <set>
#include "../runtime/UniqueIds.h"

class InvoiceCowsSelectInterface : public CowSelectInterface
{
public:
	InvoiceCowsSelectInterface();	
	~InvoiceCowsSelectInterface();

	void Create(SeasonSession* pSession,CowsScopeInvoiceTransactionFacade* transaction);
	void Destroy();
	
	virtual ListContent* GetCandidates();		
	virtual void PickRow(int row);
	virtual void UnpickRow(int row);
	virtual bool SelectCow(const CowNo& cowNo);
	virtual SelectResult SelectCows(const std::wstring& cowNoWildcard);
	virtual bool IsRowPicked(int row);
	virtual size_t GetPickedCount() const;	
	virtual UniqueIds GetCowsSelection() = 0;

protected:
	CowsScopeInvoiceTransactionFacade* m_transaction;
	CowCandidatesContent* m_pCandidates;
	SeasonSession* m_pSession;
};

#endif