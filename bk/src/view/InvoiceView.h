#ifndef __INVOICEVIEW_H__
#define __INVOICEVIEW_H__

#include <types\DateTime.h>
#include <types\Decimal.h>
#include <data\InvoiceType.h>
#include <data\IInvoice.h>


struct InvoiceViewMember
{
	enum
	{		
		Number,
		Date,
		CorrectFor,
		Hent,
		CowQty,
		VatRate,
		PayWay,
		CustomNumber,
		TotalGrossValue,
		DeductedAmount,
		TotalGrossAfterDeductions,
		TotalNetValue,
		TotalTaxValue,
		PaidDate,	
		Extras,
		PayDueDays,
		PayDueDate,
		Type,
		TransactionPlace,
		TransactionDate,
		TotalWeight,
	};
};


class InvoiceView 
{
public:
	InvoiceView(uint32_t invoiceId);	
	uint32_t GetInvoiceId() const;	
	
	const std::wstring& GetInvoiceNo() const;	
	void SetInvoiceNo(const std::wstring& invoiceNo);
	
	void SetCorrectFor(const std::wstring& correctForNo);
	const std::wstring& GetCorrectFor() const;
	
	void SetInvoiceType(InvoiceType invoiceType);
	InvoiceType GetInvoiceType() const;
	
	const std::wstring& GetExtras() const;
	void SetExtras(const std::wstring& extras);
	
	const DateTime& GetInvoiceDate() const;
	void SetInvoiceDate(const DateTime& invoiceDt);
	
	const DateTime& GetPaidDate() const;
	void SetPaidDate(const DateTime& paidDt);
	
	const Decimal& GetTotalGrossValue() const;
	void SetTotalGrossValue(const Decimal& grossValue);
	
	const Decimal& GetTotalNetValue() const;
	void SetTotalNetValue(const Decimal& netValue);
	
	const Decimal& GetTotalTaxValue() const;
	void SetTotalTaxValue(const Decimal& taxValue);
	
	const Decimal& GetVatRate() const;
	void SetVatRate(const Decimal& vatRate);
	
	PayWay GetPayWay() const;
	void SetPayWay(PayWay payway);
	
	const std::wstring& GetCustomNumber() const;
	void SetCustomNumber(const std::wstring& customNumber);
	
	const std::wstring& GetTransactionPlace() const;
	void SetTransactionPlace(const std::wstring& transactionPlace);
	
	const DateTime& GetTransactionDate() const;
	void SetTransactionDate(const DateTime& transactionDt);
	
	uint32_t GetPayDueDays() const;
	void SetPayDueDays(uint32_t payDueDays);
	
	size_t GetCowQty() const;
	void SetCowQty(size_t qty);
	
	const std::wstring& GetInvoiceHent() const;
	void SetInvoiceHent(const std::wstring& hentAlias);
	
	const DateTime& GetPayDueDate() const;
	void SetPayDueDate(const DateTime& payDueDt);

	const Decimal& GetTotalWeight() const;
	void SetTotalWeight(const Decimal& weight);

	const Decimal& GetDeductedAmount() const;
	void SetDeductedAmount(const Decimal& deductedAmount);

	const Decimal& GetGrossAfterDeductions() const;
	void SetGrossAfterDeductions(const Decimal& grossAfterDeductions);

	
private:
	std::wstring m_invoiceNo;
	uint32_t m_invoiceId;
	std::wstring m_CorrectForNo;	
	InvoiceType m_invoicetype;	
	std::wstring m_extras;	
	DateTime m_invoicedate;
	DateTime m_paiddate;
	Decimal m_totalnetvalue;
	Decimal m_totalgrossvalue;
	Decimal m_totaltaxvalue;
	Decimal m_deductedAmount;
	Decimal m_grossAfterDeductions;
	Decimal m_vatrate;
	PayWay m_payway;	
	std::wstring m_customnumber;	
	uint32_t m_payduedays;
	DateTime m_payduedate;
	std::wstring m_transactionplace;
	DateTime m_transactiondate;
	size_t 	m_cowQty;
	std::wstring m_invoiceHent;
	Decimal m_weight;
};

#endif