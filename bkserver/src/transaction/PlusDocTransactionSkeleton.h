#pragma once

#include "DocTransactionSkeleton.h"

class PlusDocTransactionSkeleton : public DocTransactionSkeleton
{
public:
	PlusDocTransactionSkeleton(ISeasonInterface* pInterface, LogInterface* logger, uint32_t transactionId);
	~PlusDocTransactionSkeleton();	
protected:
	ItemId AddCow(const CowDetails& details);
	void DeleteCow(ItemId id);
	void UpdateCow(ItemId id, const CowDetails& details);
	
	
	
};