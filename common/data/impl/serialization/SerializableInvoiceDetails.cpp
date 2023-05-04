#include "stdafx.h"
#include "SerializableInvoiceDetails.h"
#include "../../types/serialization/TypesSerialization.h"


SerializableInvoiceDetails::SerializableInvoiceDetails(InvoiceDetails& details) : m_Details(details)
{
}


std::wstring& SerializableInvoiceDetails::Extras()
{
	return m_Details.m_extras;
}

DateTime& SerializableInvoiceDetails::InvoiceDate()
{
	return m_Details.m_invoicedate;
}

DateTime& SerializableInvoiceDetails::PaidDate()
{
	return m_Details.m_paiddate;
}

Decimal& SerializableInvoiceDetails::VATRate()
{
	return m_Details.m_VATRate;
}

PayWay& SerializableInvoiceDetails::PaymentWay()
{
	return m_Details.m_payway;
}

std::wstring& SerializableInvoiceDetails::CustomNumber()
{
	return m_Details.m_customnumber;
}

int& SerializableInvoiceDetails::PayDueDays()
{
	return m_Details.m_payduedays;
}

std::wstring& SerializableInvoiceDetails::TransactionPlace()
{
	return m_Details.m_transactionplace;
}

DateTime& SerializableInvoiceDetails::TransactionDate()
{
	return m_Details.m_transactiondate;
}



void SerializableInvoiceDetails::serialize(SF::Archive &archive)
{
	
	archive & Extras();
	archive & InvoiceDate();
	archive & PaidDate();
	archive & VATRate();
	archive & PaymentWay();	
	archive & CustomNumber();
	archive & PayDueDays();
	archive & TransactionPlace();
	archive & TransactionDate();	

}


