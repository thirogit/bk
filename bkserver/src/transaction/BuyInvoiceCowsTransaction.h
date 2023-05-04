#pragma once

#include "IClientTransaction.h"
#include "InvoiceTransactionSkeleton.h"

class BuyInvoiceCowsTransaction : public IClientTransaction, protected InvoiceTransactionSkeleton
{
public:
	BuyInvoiceCowsTransaction(ISeasonInterface* pInterface,uint32_t transactionId);	
	virtual boost::any ExecuteOperation(uint16_t operation,const OperationArguments& args);	

	void New();
	void Edit(uint32_t invoiceId);
		
	virtual void Commit();
	virtual void Abort();
	virtual uint32_t GetId() const;

protected:
	ItemId AddCow(uint32_t cowId);
	void DeleteItem(const ItemId& itemId);
	void SetDoc(uint32_t docId);
	void CheckIfDocIsSet();
	void Cleanup();

protected:
	uint32_t m_InDocId;
	std::set<uint32_t> m_newlyAddedCows;
};