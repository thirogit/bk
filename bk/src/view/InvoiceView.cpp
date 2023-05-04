#include "stdafx.h"
#include "InvoiceView.h"

InvoiceView::InvoiceView(uint32_t invoiceId) : m_invoiceId(invoiceId),m_invoicetype(InvoiceType_None),m_payway(PayWay_None),m_payduedays(0),m_cowQty(0)
{
}
	
uint32_t InvoiceView::GetInvoiceId() const
{
	return m_invoiceId;
}
	

const std::wstring& InvoiceView::GetInvoiceNo() const
{
	return m_invoiceNo;
}
	
void InvoiceView::SetInvoiceNo(const std::wstring& invoiceNo)
{
	m_invoiceNo = invoiceNo;
}

void InvoiceView::SetCorrectFor(const std::wstring& correctForNo)
{
	m_CorrectForNo = correctForNo;
}

const std::wstring& InvoiceView::GetCorrectFor() const
{
	return m_CorrectForNo;
}

void InvoiceView::SetInvoiceType(InvoiceType invoiceType)
{
	m_invoicetype = invoiceType;
}

InvoiceType InvoiceView::GetInvoiceType() const
{
	return m_invoicetype;
}

const std::wstring& InvoiceView::GetExtras() const
{
	return m_extras;
}

void InvoiceView::SetExtras(const std::wstring& extras)
{
	m_extras = extras;
}

const DateTime& InvoiceView::GetInvoiceDate() const
{
	return m_invoicedate;
}

void InvoiceView::SetInvoiceDate(const DateTime& invoiceDt)
{
	m_invoicedate = invoiceDt;
}


const DateTime& InvoiceView::GetPaidDate() const
{
	return m_paiddate;
}

void InvoiceView::SetPaidDate(const DateTime& paidDt)
{
	m_paiddate = paidDt;
}


const Decimal& InvoiceView::GetTotalGrossValue() const
{
	return m_totalgrossvalue;
}

void InvoiceView::SetTotalGrossValue(const Decimal& grossValue)
{
	m_totalgrossvalue = grossValue;
}
	
void InvoiceView::SetTotalNetValue(const Decimal& netValue)
{
	m_totalnetvalue = netValue;
}

const Decimal& InvoiceView::GetTotalTaxValue() const
{
	return m_totaltaxvalue;
}
	
void InvoiceView::SetTotalTaxValue(const Decimal& taxValue)
{
	m_totaltaxvalue = taxValue;
}

const Decimal& InvoiceView::GetTotalNetValue() const
{
	return m_totalnetvalue;
}

const Decimal& InvoiceView::GetVatRate() const
{
	return m_vatrate;
}

void InvoiceView::SetVatRate(const Decimal& vatRate)
{
	m_vatrate = vatRate;
}

PayWay InvoiceView::GetPayWay() const
{
	return m_payway;
}

void InvoiceView::SetPayWay(PayWay payway)
{
	m_payway = payway;
}

const std::wstring& InvoiceView::GetCustomNumber() const
{
	return m_customnumber;
}

void InvoiceView::SetCustomNumber(const std::wstring& customNumber)
{
	m_customnumber = customNumber;
}

const std::wstring& InvoiceView::GetTransactionPlace() const
{
	return m_transactionplace;
}

void InvoiceView::SetTransactionPlace(const std::wstring& transactionPlace)
{
	m_transactionplace = transactionPlace;
}

const DateTime& InvoiceView::GetTransactionDate() const
{
	return m_transactiondate;
}

void InvoiceView::SetTransactionDate(const DateTime& transactionDt)
{
	m_transactiondate = transactionDt;
}

uint32_t InvoiceView::GetPayDueDays() const
{
	return m_payduedays;
}

void InvoiceView::SetPayDueDays(uint32_t payDueDays)
{
	m_payduedays = payDueDays;
}

size_t InvoiceView::GetCowQty() const
{
	return m_cowQty;
}

void InvoiceView::SetCowQty(size_t qty)
{
	m_cowQty = qty;
}


const std::wstring& InvoiceView::GetInvoiceHent() const
{
	return m_invoiceHent;
}

void InvoiceView::SetInvoiceHent(const std::wstring& hentAlias)
{
	m_invoiceHent = hentAlias;
}

const DateTime& InvoiceView::GetPayDueDate() const
{
	return m_payduedate;
}

void InvoiceView::SetPayDueDate(const DateTime& payDueDt)
{
	m_payduedate = payDueDt;
}


const Decimal& InvoiceView::GetDeductedAmount() const
{
	return m_deductedAmount;
}

void InvoiceView::SetDeductedAmount(const Decimal& deductedAmount)
{
	m_deductedAmount = deductedAmount;
}

const Decimal& InvoiceView::GetGrossAfterDeductions() const
{
	return m_grossAfterDeductions;
}

void InvoiceView::SetGrossAfterDeductions(const Decimal& grossAfterDeductions)
{
	m_grossAfterDeductions = grossAfterDeductions;
}

const Decimal& InvoiceView::GetTotalWeight() const
{
	return m_weight;
}

void InvoiceView::SetTotalWeight(const Decimal& weight)
{
	m_weight = weight;
}