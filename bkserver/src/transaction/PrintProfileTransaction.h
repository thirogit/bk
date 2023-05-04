#pragma once

#include "IClientTransaction.h"

class PrintProfileTransaction : public IClientTransaction
{
public:
	PrintProfileTransaction(uint32_t transactionId);
	virtual boost::any ExecuteOperation(uint16_t operation,const OperationArguments& args);
	virtual void Commit();
	virtual void Abort();
	virtual uint32_t GetId() const;	
	void New(const std::string& docCode);
	void Edit(uint32_t docId);
	
protected:
	ItemId AddCow(uint32_t cowId);
	void DeleteCow(ItemId id);
protected:
	uint32_t m_herdId;
	uint32_t m_docId;
	ItemIndex m_newlyAddedItems;
};