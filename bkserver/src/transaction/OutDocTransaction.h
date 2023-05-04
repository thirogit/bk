#pragma once

#include "IClientTransaction.h"
#include "DocTransactionSkeleton.h"

class ClientSeasonSession;
class SeasonSessionFacade;

class OutDocTransaction : public IClientTransaction, protected DocTransactionSkeleton
{
public:
	OutDocTransaction(ISeasonInterface* pInterface, LogInterface* logger, uint32_t transactionId);
	virtual boost::any ExecuteOperation(uint16_t operation,const OperationArguments& args);
	virtual void Commit();
	virtual void Abort();
	virtual uint32_t GetId() const;	
	void New(uint32_t herdId);
	void Edit(uint32_t docId);
	
protected:
	ItemId AddCow(uint32_t cowId);
	void DeleteCow(ItemId id);
	void ThrowIfCowHasSellInvoice(DocTransactionCowItem* pItem);
protected:
	uint32_t m_herdId;
	uint32_t m_docId;
	ItemIndex<DocTransactionCowItem> m_newlyAddedItems;
};