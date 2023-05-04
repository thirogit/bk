#include "stdafx.h"
#include "InvoiceDetails.h"
#include <utils/JsonUtils.h>

InvoiceDetails::InvoiceDetails() : m_payway(PayWay_None), m_payduedays(0)
{
}



void InvoiceDetails::CopyFrom(const InvoiceDetails& src)
{
	m_extras = src.m_extras;
	m_invoicedate = src.m_invoicedate;
	m_paiddate = src.m_paiddate;
	m_VATRate = src.m_VATRate;
	m_payway = src.m_payway;	
	m_customnumber = src.m_customnumber;	
	m_payduedays = src.m_payduedays;
	m_transactionplace = src.m_transactionplace;
	m_transactiondate = src.m_transactiondate;	
}

void InvoiceDetails::CopyFrom(const IInvoice* pSrc)
{
	m_extras = pSrc->GetExtras();
	m_invoicedate = pSrc->GetInvoiceDate();
	m_paiddate = pSrc->GetPaidDate();
	m_VATRate = pSrc->GetVATRate();
	m_payway = pSrc->GetPayWay();	
	m_customnumber = pSrc->GetCustomNumber();	
	m_payduedays = pSrc->GetPayDueDays();
	m_transactionplace = pSrc->GetTransactionPlace();
	m_transactiondate = pSrc->GetTransactionDate();	
}
	
const std::wstring& InvoiceDetails::GetExtras() const
{	
	return m_extras;
}

const DateTime& InvoiceDetails::GetInvoiceDate() const
{	
	return m_invoicedate;
}

const DateTime&	InvoiceDetails::GetPaidDate() const
{
	return m_paiddate;
}
		
const Decimal& InvoiceDetails::GetVATRate() const
{
	return m_VATRate;
}

PayWay InvoiceDetails::GetPayWay() const
{
	return m_payway;
}

const std::wstring& InvoiceDetails::GetCustomNumber() const
{
	return m_customnumber;
}

const std::wstring& InvoiceDetails::GetTransactionPlace() const
{
	return m_transactionplace;
}

const DateTime&	InvoiceDetails::GetTransactionDate() const
{
	return m_transactiondate;
}

int InvoiceDetails::GetPayDueDays() const
{
	return m_payduedays;
}


void InvoiceDetails::SetExtras(const std::wstring& sExtras)
{
	m_extras = sExtras;
}

void InvoiceDetails::SetInvoiceDate(const DateTime& invoiceDt)
{
	m_invoicedate = invoiceDt;
}

void InvoiceDetails::SetPaidDate(const DateTime& paidDt)
{
	m_paiddate = paidDt;
}

void InvoiceDetails::SetVATRate(const Decimal& vatRate)
{
	m_VATRate = vatRate;
}

void InvoiceDetails::SetPayWay(PayWay payWay)
{
	m_payway = payWay;
}

void InvoiceDetails::SetCustomNumber(const std::wstring& sCustomNumber)
{
	m_customnumber = sCustomNumber;
}

void InvoiceDetails::SetTransactionPlace(const std::wstring& sTransactionPlace)
{
	m_transactionplace = sTransactionPlace;
}

void InvoiceDetails::SetTransactionDate(const DateTime& transactionDt)
{
	m_transactiondate = transactionDt;
}

void InvoiceDetails::SetPayDueDays(int payDueDays)
{
	m_payduedays = payDueDays;
}
	

static const wchar_t* extras_attribute_name = L"extras";
static const wchar_t* invoicedate_attribute_name = L"invoice_date";
static const wchar_t* paiddate_attribute_name = L"paid_date";
static const wchar_t* vatrate_attribute_name = L"vat_rate";
static const wchar_t* payway_attribute_name = L"payway";
static const wchar_t* customnumber_attribute_name = L"custom_number";
static const wchar_t* payduedays_attribute_name = L"payduedays";
static const wchar_t* transactionplace_attribute_name = L"transactionplace";
static const wchar_t* transactiondate_attribute_name = L"transactiondate";

InvoiceDetails InvoiceDetails::FromJson(const json::value& value)
{
	InvoiceDetails result;

	result.m_extras = value[extras_attribute_name].as_string();
	result.m_invoicedate = json_DayDate(value[invoicedate_attribute_name]);
	result.m_paiddate = json_DayDate(value[paiddate_attribute_name]);
	result.m_VATRate = json_Decimal(value[vatrate_attribute_name]);
	result.m_payway = json_PayWay(value[payway_attribute_name]);
	result.m_customnumber = value[customnumber_attribute_name].as_string();
	result.m_payduedays = value[payduedays_attribute_name].as_integer();
	result.m_transactionplace = value[transactionplace_attribute_name].as_string();
	result.m_transactiondate = json_DayDate(value[transactiondate_attribute_name]);
	return std::move(result);
}

json::value InvoiceDetails::ToJson() const
{
	json::value result;

	result[extras_attribute_name] = json::value(this->m_extras);
	result[invoicedate_attribute_name] = json_DayDate(this->m_invoicedate);
	result[paiddate_attribute_name] = json_DayDate(this->m_paiddate);
	result[vatrate_attribute_name] = json_Decimal(this->m_VATRate);
	result[payway_attribute_name] = json_PayWay(this->m_payway);
	result[customnumber_attribute_name] = json::value(this->m_customnumber);
	result[payduedays_attribute_name] = json::value(this->m_payduedays);
	result[transactionplace_attribute_name] = json::value(this->m_transactionplace);
	result[transactiondate_attribute_name] = json_DayDate(this->m_transactiondate);

	return result;
}