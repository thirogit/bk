#pragma once

#include "IClientTransaction.h"
#include "DocTransactionSkeleton.h"

class EditSellDocTransaction : public IClientTransaction, protected DocTransactionSkeleton
{
public:
	EditSellDocTransaction(ISeasonInterface* pInterface, LogInterface* logger, uint32_t transactionId);
	virtual boost::any ExecuteOperation(uint16_t operation,const OperationArguments& args);
	void Edit(uint32_t docId);	
	virtual void Commit();
	virtual void Abort();
	virtual uint32_t GetId() const;
protected:
	ItemId AddCow(uint32_t cowId);
	void DeleteCow(ItemId id);
	void ThrowIfThereIsInvoice();

	ItemIndex<DocTransactionCowItem> m_newlyAddedItems;
private:
	 uint32_t m_docId;
	 uint32_t m_herdId;
	 uint32_t m_hentId;
	 uint32_t m_sellInvoiceId;

};