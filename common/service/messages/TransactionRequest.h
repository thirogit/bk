#ifndef __TRANSACTIONREQUEST_H__
#define __TRANSACTIONREQUEST_H__

#include <boost/shared_ptr.hpp>
#include <SF/Archive.hpp>
#include "../../data/runtime/OperationArguments.h"

class TransactionRequest
{
public:
	TransactionRequest();
	
	TransactionRequest& WithTransactionId(uint32_t transactionId);	
	TransactionRequest& WithOperation(uint16_t operation);
	TransactionRequest& WithParameters(const OperationArguments& args);

	uint32_t GetTransactionId() const;	
	uint16_t GetOperation() const;
	const OperationArguments& GetParameters() const;
	
	void serialize(SF::Archive &archive);	
private:
	OperationArguments m_args;
	uint32_t m_transactionId;
	uint16_t m_operation;
};

typedef boost::shared_ptr<TransactionRequest> TransactionRequestObj;

#endif
