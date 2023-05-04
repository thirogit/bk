#pragma once

#include "IClientTransaction.h"
#include "DocTransactionSkeleton.h"

class NewSellDocTransaction : public IClientTransaction, protected DocTransactionSkeleton
{
public:
	NewSellDocTransaction(ISeasonInterface* pInterface, LogInterface* logger, uint32_t transactionId, uint32_t herdId);
	virtual boost::any ExecuteOperation(uint16_t operation,const OperationArguments& args);
	virtual void Commit();
	virtual void Abort();
	virtual uint32_t GetId() const;
protected:
	ItemId AddCow(uint32_t cowId);
	void DeleteCow(ItemId id);
protected:
	uint32_t m_HerdId;
	uint32_t m_HentId;

	
};