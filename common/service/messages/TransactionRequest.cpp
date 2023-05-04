#include "stdafx.h"
#include "TransactionRequest.h"
#include "../../data/IIdable.h"
#include "../../data/runtime/serialization/SerializableOperationArguments.h"

TransactionRequest::TransactionRequest() : m_operation(0),m_transactionId(NULL_ID)
{
}
	
TransactionRequest& TransactionRequest::WithTransactionId(uint32_t transactionId)
{
	m_transactionId = transactionId;
	return *this;
}

TransactionRequest& TransactionRequest::WithOperation(uint16_t operation)
{
	m_operation = operation;
	return *this;
}

TransactionRequest& TransactionRequest::WithParameters(const OperationArguments& args)
{
	m_args = args;
	return *this;
}

uint32_t TransactionRequest::GetTransactionId() const
{
	return m_transactionId;
}

uint16_t TransactionRequest::GetOperation() const
{
	return m_operation;
}

const OperationArguments& TransactionRequest::GetParameters() const
{
	return m_args;
}
	
void TransactionRequest::serialize(SF::Archive &archive)
{
	archive & m_transactionId;
	archive & m_operation;
	SerializableOperationArguments(&m_args).serialize(archive);
}
