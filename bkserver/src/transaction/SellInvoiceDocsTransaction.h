#pragma once

#include <set>
#include "IClientTransaction.h"
#include "InvoiceTransactionSkeleton.h"

class SellInvoiceDocsTransaction : public IClientTransaction, protected InvoiceTransactionSkeleton
{	
public:
	SellInvoiceDocsTransaction(ISeasonInterface* pInterface,uint32_t transactionId);	
	virtual boost::any ExecuteOperation(uint16_t operation,const OperationArguments& args);	

	void New();
	void Edit(uint32_t invoiceId);
		
	virtual void Commit();
	virtual void Abort();
	virtual uint32_t GetId() const;

protected:	
	void AddDoc(uint32_t docId);
	void DeleteDoc(uint32_t docId);
	ItemsRange GetDocItems(uint32_t docId);
	void UnlockNewlyAddedDocs();

protected:
	std::set<uint32_t> m_newlyAddedDocIds;
	std::set<uint32_t> m_deletedDocIds;
	std::set<uint32_t> m_DocIds;
};