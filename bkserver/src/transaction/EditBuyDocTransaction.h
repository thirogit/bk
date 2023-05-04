#pragma once

#include "IClientTransaction.h"
#include "PlusDocTransactionSkeleton.h"

class EditBuyDocTransaction : public IClientTransaction, protected PlusDocTransactionSkeleton
{
public:
	EditBuyDocTransaction(ISeasonInterface* pInterface, LogInterface* logger,uint32_t transactionId);
	void Edit(uint32_t docId);

	virtual boost::any ExecuteOperation(uint16_t operation,const OperationArguments& args);
	virtual void Commit();
	virtual void Abort();
	virtual uint32_t GetId() const;
protected:
	void DeleteCow(ItemId id);
	ItemId AddCow(CowDetails details);
protected:
	uint32_t m_herdId;	
	uint32_t m_docId;
	uint32_t m_hentId;
	bool	 m_bInvoiced;
};

