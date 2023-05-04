#ifndef __INVOICEDETAILS_H__
#define __INVOICEDETAILS_H__

#include "../IInvoice.h"
#include  <casajson/json.h>

class SerializableInvoiceDetails;

class InvoiceDetails 
{
public:
	InvoiceDetails();
	void CopyFrom(const InvoiceDetails& src);
	void CopyFrom(const IInvoice* pSrc);
	
	const std::wstring&	GetExtras() const;
	const DateTime&		GetInvoiceDate() const;
	const DateTime&		GetPaidDate() const;		
	const Decimal&		GetVATRate() const;
	PayWay				GetPayWay() const;
	const std::wstring&	GetCustomNumber() const;
	const std::wstring&	GetTransactionPlace() const;
	const DateTime&		GetTransactionDate() const;
	int					GetPayDueDays() const;
	
	const Decimal&		GetExchangeRate() const;

	
	void SetExtras(const std::wstring& sExtras);
	void SetInvoiceDate(const DateTime& invoiceDt);
	void SetPaidDate(const DateTime& paidDt);
	void SetVATRate(const Decimal& vatRate);
	void SetPayWay(PayWay payWay);
	void SetCustomNumber(const std::wstring& sCustomNumber);
	void SetTransactionPlace(const std::wstring& sTransactionPlace);
	void SetTransactionDate(const DateTime& transactionDt);
	void SetPayDueDays(int payDueDays);
	void SetExchangeRate(const Decimal& rate);

	static InvoiceDetails FromJson(const json::value& value);
	json::value ToJson() const;
protected:
		
	std::wstring m_extras;
	DateTime 	m_invoicedate;
	DateTime 	m_paiddate;
	Decimal  	m_VATRate;
	PayWay 	 	m_payway;	
	std::wstring m_customnumber;	
	int 		m_payduedays;
	std::wstring m_transactionplace;
	DateTime 	m_transactiondate;	
	Decimal		m_exchangeRate;
	
	friend class SerializableInvoiceDetails;
};

#endif