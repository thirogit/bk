#ifndef __CALCULATIONELEMENT_H__
#define __CALCULATIONELEMENT_H__

#include "../../context/transaction/InvoiceTransactionItem.h"
#include "CustomPrice.h"
#include <data\runtime\IItem.h>

class InvoiceCalculation;

class CalculationElement : public IItem
{
public:
	CalculationElement(const InvoiceTransactionItem *pItem, InvoiceCalculation *pCalculation);

	const InvoiceTransactionItem* GetItem() const;
	virtual ItemId GetId() const;
		
	Decimal GetNetPrice() const;
	Decimal GetGrossPrice() const;
	Decimal GetTaxValue() const;
	
	void SetCustomWeight(const NullDecimal& weight);
	NullDecimal GetCustomWeight() const;
	bool IsCustomWeight() const;
	bool IsAvgWeight() const;

	void SetCustomClass(uint32_t classId);
	uint32_t GetCustomClass() const;
	uint32_t GetInheritedClass() const;
	bool IsCustomClass() const;
	
	Decimal GetWeight() const;
	uint32_t GetClassId() const;
	Decimal GetNetPricePerKg() const;
	
	void SetCustomPricePerKg(const Decimal &pricePerKg);
	bool IsCustomPricePerKgForAll() const;

	void SetCustomGrossPrice(const Decimal& grossPrice);
	void SetCustomNetPrice(const Decimal& netPrice);
	bool IsAvgPrice() const;

	CustomPrice GetCustomPrice() const;
	void DisableCustomPrice();

	//Money GetCowPricePerKg(Rule* pRule,const Weight& weight,const CowSex& sex) const;

private:

	InvoiceCalculation *m_pCalculation;
	const InvoiceTransactionItem *m_pItem;

	CustomPrice m_customPrice;

	NullDecimal m_customNetPrice;
	NullDecimal m_customNetPricePerKg;
	NullDecimal m_customWeight;
	uint32_t m_customClassId;
	
};

#endif