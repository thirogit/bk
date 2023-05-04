#pragma once

#include <data/runtime/IItem.h>
#include <data/impl/InvoiceDeduction.h>

class DeductionTransactionItem : public IItem
{
public:
	DeductionTransactionItem(const ItemId& id);
	virtual ItemId GetId() const;	

	void SetDeductionFrom(const IInvoiceDeduction* deduction);
	void SetDeduction(const InvoiceDeduction& deduction);
	const InvoiceDeduction& GetDeduction() const;
private:
	ItemId m_id;
	InvoiceDeduction m_deduction;
};
