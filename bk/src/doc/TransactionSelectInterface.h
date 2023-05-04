#ifndef __TRANSACTIONSELECTINTERFACE_H__
#define __TRANSACTIONSELECTINTERFACE_H__

#include "../context/SeasonSession.h"
#include "../cow/CowSelectInterface.h"
#include "CandidatesContent.h"
#include <set>

class TransactionSelectInterface : public CowSelectInterface
{
public:
	TransactionSelectInterface();	
	~TransactionSelectInterface();

	void Create(SeasonSession* pSession,
				uint32_t herdId,
				MinusDocTransactionFacade* pTransactionFacade);

	void Destroy();
	
	virtual ListContent* GetCandidates();
		
	virtual void PickRow(int row);
	virtual void UnpickRow(int row);
	virtual bool SelectCow(const CowNo& cowNo);
	virtual bool IsRowPicked(int row);
	virtual size_t GetPickedCount() const;
	virtual SelectResult SelectCows(const std::wstring& cowNoWildcard);

private:
	MinusDocTransactionFacade* m_transaction;
	CandidatesContent* m_pCandidates;
	std::set<ItemId> m_Picked;


};

#endif