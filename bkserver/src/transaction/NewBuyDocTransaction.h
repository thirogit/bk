#pragma once

#include "IClientTransaction.h"
#include "PlusDocTransactionSkeleton.h"
#include "../log/LogInterface.h"
#include "../log/LoggerAware.h"

class NewBuyDocTransaction : public IClientTransaction, protected PlusDocTransactionSkeleton
{
public:
	NewBuyDocTransaction(ISeasonInterface* pInterface, LogInterface* logger, uint32_t transactionId, uint32_t herdId);
	virtual boost::any ExecuteOperation(uint16_t operation,const OperationArguments& args);
	virtual void Commit();
	virtual void Abort();
	virtual uint32_t GetId() const;
protected:
	uint32_t m_HerdId;
	uint32_t m_HentId;

	
};