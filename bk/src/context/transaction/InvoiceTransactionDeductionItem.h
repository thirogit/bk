#ifndef __INVOICETRANSACTIONDEDUCTIONITEM_H__
#define __INVOICETRANSACTIONDEDUCTIONITEM_H__

#include <data\impl\InvoiceDeduction.h>
#include <data\runtime\IItem.h>

class InvoiceTransactionDeductionItem : public IItem
{
public:
	InvoiceTransactionDeductionItem(const ItemId& itemId);
	
	virtual ItemId GetId() const;

	const InvoiceDeduction& GetDeduction() const;
	void SetDeduction(const InvoiceDeduction& deduction);
	
	DeductionDetails GetDetails() const;
	void SetDetails(const DeductionDetails& details);
	
	bool IsEnabled() const;
	void SetEnabled(bool bEnabled);	

private:
	InvoiceDeduction m_deduction;
	ItemId m_id;
};

#endif