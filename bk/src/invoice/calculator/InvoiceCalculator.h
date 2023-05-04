#ifndef __INVOICECALCULATOR_H__
#define __INVOICECALCULATOR_H__

#include <data\IInvoice.h>

namespace InvoiceCalculator 
{
	class CalculationResult
	{
	public:
		CalculationResult();
		CalculationResult(const CalculationResult& src);
		CalculationResult& operator=(const CalculationResult& right);

		const Decimal& GetDeductedAmount() const;
	           Decimal GetGrossAfterDeductionsValue() const;
		const Decimal& GetGrossValue() const;
		const Decimal& GetNetValue() const;
		const Decimal& GetTaxValue() const;
		const Decimal& GetWeight() const;

		void SetGrossValue(const Decimal& value);
		void SetNetValue(const Decimal& value);
		void SetTaxValue(const Decimal& value);
		void SetWeight(const Decimal& value);
		void SetDeductedAmount(const Decimal& amount);

	private:
		Decimal m_grossValue;
		Decimal m_netValue;
		Decimal m_taxValue;
		Decimal m_weightValue;
		Decimal m_deductedAmount;		
	};

	CalculationResult Calculate(const IInvoice* pInvoice);
};


#endif