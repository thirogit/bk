#pragma once

#include "IClientTransaction.h"
#include "ISeasonInterface.h"
#include "../log/LogInterface.h"

class TransferBuyDocTransaction : public IClientTransaction
{
public:
	TransferBuyDocTransaction(ISeasonInterface* pInterface, LogInterface* logger,uint32_t transactionId);
	void Open(uint32_t docId);

	virtual boost::any ExecuteOperation(uint16_t operation,const OperationArguments& args);
	virtual void Commit();
	virtual void Abort();
	virtual uint32_t GetId() const;
protected:
	uint32_t m_dstHerdId;	
	uint32_t m_docId;
	bool	 m_bInvoiced;
};

