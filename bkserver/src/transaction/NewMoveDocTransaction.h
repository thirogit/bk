#pragma once

#include "IClientTransaction.h"
#include "DocTransactionSkeleton.h"

class NewMoveDocTransaction : public IClientTransaction, protected DocTransactionSkeleton
{
public:
	NewMoveDocTransaction(ISeasonInterface* pInterface, LogInterface* logger, uint32_t transactionId, uint32_t srcHerdId, uint32_t dstHerdId);
	virtual boost::any ExecuteOperation(uint16_t operation,const OperationArguments& args);
	virtual void Commit();
	virtual void Abort();
	virtual uint32_t GetId() const;
protected:
	ItemId AddCow(uint32_t cowId);

private:
	 uint32_t m_srcHerdId;
	 uint32_t m_dstHerdId;
};