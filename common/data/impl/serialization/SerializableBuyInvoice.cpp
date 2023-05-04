#include "stdafx.h"
#include "SerializableBuyInvoice.h"
#include "SerializableInvoiceDetails.h"
#include "SerializableInvoiceContent.h"
#include "SerializableInvoiceHent.h"
#include "InvoiceDeductionsSerializer.h"

SerializableBuyInvoice::SerializableBuyInvoice(BuyInvoice* pInvoice) : m_pInvoice(pInvoice)
{
}

uint32_t& 	SerializableBuyInvoice::InvoiceNo()
{
	return m_pInvoice->m_invoiceno;
}

uint32_t& 	SerializableBuyInvoice::Id()
{
	return m_pInvoice->m_invoiceid;
}

uint32_t& 	SerializableBuyInvoice::CorrectFor()
{
	return m_pInvoice->m_correctfor;
}

InvoiceType& SerializableBuyInvoice::TypeOfInvoice()
{
	return m_pInvoice->m_invoicetype;
}

InvoiceScope& SerializableBuyInvoice::Scope()
{
	return m_pInvoice->m_invoicescope;
}

InvoiceDetails& SerializableBuyInvoice::Details()
{
	return m_pInvoice->m_Details;
}

InvoiceContent& SerializableBuyInvoice::Content()
{
	return m_pInvoice->m_Content;
}

InvoiceHent& SerializableBuyInvoice::Hent()
{
	return m_pInvoice->m_Hent;
}

InvoiceDeductions& SerializableBuyInvoice::Deductions()
{
	return m_pInvoice->m_Deductions;
}
	
void SerializableBuyInvoice::serialize(SF::Archive &archive)
{
	archive & Id();
	archive & TypeOfInvoice();
	archive& 	InvoiceNo();
	archive& 	Id();
	archive& 	CorrectFor();
	archive&	Scope();	
	SerializableInvoiceDetails(Details()).serialize(archive);
	SerializableInvoiceHent(Hent()).serialize(archive);
	SerializableInvoiceContent(Content()).serialize(archive);
	InvoiceDeductionsSerializer(Deductions()).serialize(archive);
}


