#ifndef __INVOICECALCULATION_H__
#define __INVOICECALCULATION_H__

#include "../../context/Observerable.h"
#include "CalculationObserver.h"
#include <boost\range\any_range.hpp>
#include <vector>
#include "../../context/transaction/InvoiceTransactionFacade.h"
#include <exception/TagException.h>
#include "DeductionElement.h"
#include <data\runtime\ItemIndex.h>


typedef boost::any_range<const CalculationElement*,boost::forward_traversal_tag, CalculationElement* const, std::ptrdiff_t> CalculationElements;
typedef boost::any_range<const DeductionElement*, boost::forward_traversal_tag, DeductionElement* const, std::ptrdiff_t> DeductionElements;

struct tag_CalculationException {};
typedef TagException<tag_CalculationException> CalculationException;

class InvoiceCalculation : public Observerable<CalculationObserver>,protected InvoiceTransactionObserver
{
	typedef ItemIndex<CalculationElement> ElementIndex;
	typedef ItemIndex<DeductionElement> DeductionIndex;

public:
	InvoiceCalculation(InvoiceTransactionFacade* pTransaction);
	~InvoiceCalculation();
	void New();
	void Edit();

	InvoiceTransactionFacade* GetTransaction();

	void SetTargetNetTotal(const NullDecimal& totalNet);
	void SetTargetGrossTotal(const NullDecimal& totalGross);
	void SetTargetTotalWeight(const NullDecimal& totalWeight);

	void SetVATRate(const Decimal& vatRate);
	const Decimal& GetVATRate() const;
	void SetCustomNetPricePerKgForAll(const NullDecimal& netPricePerKg);

	void SetItemCustomNetPricePerKg(ItemId id,const NullDecimal& netPricePerKg);
	void SetItemCustomNetPrice(ItemId id,const NullDecimal& netPrice);
	void SetItemCustomGrossPrice(ItemId id,const NullDecimal& grossPrice);
	void SetItemCustomWeight(ItemId id,const NullDecimal& weight);
	void SetItemCustomClass(ItemId id,uint32_t classId);
	void DisableCustomPrice(ItemId id);

	void EnableDeduction(ItemId deductionId, bool bEnable);
	
	CalculationElements Elements();
	DeductionElements Deductions();
	const CalculationElement* GetElementAt(int index);

	size_t GetElementCount() const;
	
	bool IsAvgPriceEnabled() const;
	NullDecimal GetAvgPrice() const;
	
	bool IsUseCustomPricePerKgForAll() const;
	NullDecimal GetCustomPricePerKgForAll() const;
	
	bool IsAvgWeightEnabled() const;
	NullDecimal GetAvgWeight() const;

	Decimal GetNetTotal();
	Decimal GetGrossTotal();
	Decimal GetTaxValue();
	Decimal GetWeightTotal();
	Decimal GetGrossAfterDeductions();
protected:
	virtual void OnItemUpdated(ItemId id);
	virtual void OnItemAdded(ItemId id);
	virtual void OnItemDeleted(ItemId id);

	virtual void OnDeductionUpdated(ItemId id);
	virtual void OnDeductionAdded(ItemId id);
	virtual void OnDeductionDeleted(ItemId id);

	CalculationElement* FindElement(ItemId id);
	DeductionElement* FindDeduction(ItemId id);

	void NotifyElementChanged(const CalculationElement* pElement);
	void NotifyElementAdded(ItemId id);
	void NotifyElementDeleted(ItemId id);
	void NotifyElementUpdated(ItemId id);

	void NotifyDeductionAdded(ItemId id);
	void NotifyDeductionDeleted(ItemId id);
	void NotifyDeductionUpdated(ItemId id);

	void ThrowIfAllHasCustomPrice();
	Decimal SumCustomWeight();
	Decimal SumNetCustomPrice();
	Decimal SumGrossCustomPrice();

	long CountWithCustomWeight();
	long CountWithCustomPrice();
	void NotifyAllChanged();
	void RecalculateAverageWeight();

	

private:
	ElementIndex m_elements;
	DeductionIndex m_deductions;

	Decimal m_VATRate;
	NullDecimal m_CustomPricePerKg4All;
	NullDecimal m_CustomTotalWeight;

	NullDecimal m_CustomNetTotal;

	NullDecimal m_CustomGrossTotal;

	NullDecimal m_AvgPrice;
	NullDecimal m_AvgWeight;

	InvoiceTransactionFacade* m_pTransaction;
};

#endif