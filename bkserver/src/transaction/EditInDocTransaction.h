#pragma once

#include "IClientTransaction.h"
#include "PlusDocTransactionSkeleton.h"

class EditInDocTransaction : public IClientTransaction, protected PlusDocTransactionSkeleton
{
public:
	EditInDocTransaction(ISeasonInterface* pInterface, LogInterface* logger, uint32_t transactionId);
	void Edit(uint32_t docId);

	virtual boost::any ExecuteOperation(uint16_t operation,const OperationArguments& args);
	virtual void Commit();
	virtual void Abort();
	virtual uint32_t GetId() const;
protected:
	
protected:
	uint32_t m_HerdId;	
	uint32_t m_docId;
};

