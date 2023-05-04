#pragma once

#include "IClientTransaction.h"
#include "InvoiceTransactionSkeleton.h"

class SellInvoiceCowsTransaction : public IClientTransaction, protected InvoiceTransactionSkeleton
{
public:
	SellInvoiceCowsTransaction(ISeasonInterface* pInterface,uint32_t transactionId);	
	virtual boost::any ExecuteOperation(uint16_t operation,const OperationArguments& args);	

	void New();
	void Edit(uint32_t invoiceId);
		
	virtual void Commit();
	virtual void Abort();
	virtual uint32_t GetId() const;

protected:
	ItemId AddCow(uint32_t cowId);
	void SetDoc(uint32_t docId);
	void CheckIfDocIsSet();

protected:
	uint32_t m_OutDocId;
};