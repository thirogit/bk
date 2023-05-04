#pragma once

#include "IClientTransaction.h"
#include "PlusDocTransactionSkeleton.h"

class NewInDocTransaction : public IClientTransaction, protected PlusDocTransactionSkeleton
{
public:
	NewInDocTransaction(ISeasonInterface* pInterface, LogInterface* logger, uint32_t transactionId, uint32_t herdId);
	virtual boost::any ExecuteOperation(uint16_t operation,const OperationArguments& args);
	virtual void Commit();
	virtual void Abort();
	virtual uint32_t GetId() const;
protected:
	uint32_t m_HerdId;	
};