#ifndef __DEDUCTIONELEMENT_H__
#define __DEDUCTIONELEMENT_H__

#include "../../context/transaction/InvoiceTransactionDeductionItem.h"
#include <data\runtime\IItem.h>
#include <types\NullDecimal.h>

class InvoiceCalculation;

class DeductionElement : public IItem
{
public:
	DeductionElement(const InvoiceTransactionDeductionItem *pDeduction, InvoiceCalculation *pCalculation);

	virtual ItemId GetId() const;
	const InvoiceDeduction& GetDeduction() const;
	void SetDeduction(const InvoiceDeduction& deduction);
	NullDecimal GetDeductedAmount() const;
	
private:
	InvoiceCalculation *m_pCalculation;
	ItemId m_id;
	InvoiceDeduction m_deduction;	
};

#endif