#include "stdafx.h"
#include "SerializableSellInvoice.h"
#include "SerializableInvoiceDetails.h"
#include "SerializableInvoiceHent.h"
#include "SerializableInvoiceContent.h"
#include "InvoiceDeductionsSerializer.h"

SerializableSellInvoice::SerializableSellInvoice(SellInvoice* pInvoice) : m_pInvoice(pInvoice)
{
}

uint32_t& 	SerializableSellInvoice::InvoiceNo()
{
	return m_pInvoice->m_invoiceno;
}

uint32_t& 	SerializableSellInvoice::Id()
{
	return m_pInvoice->m_invoiceid;
}

uint32_t& 	SerializableSellInvoice::CorrectFor()
{
	return m_pInvoice->m_correctfor;
}

InvoiceType& SerializableSellInvoice::TypeOfInvoice()
{
	return m_pInvoice->m_invoicetype;
}

InvoiceScope& SerializableSellInvoice::Scope()
{
	return m_pInvoice->m_invoicescope;
}

InvoiceDetails& SerializableSellInvoice::Details()
{
	return m_pInvoice->m_Details;
}

InvoiceContent& SerializableSellInvoice::Content()
{
	return m_pInvoice->m_Content;
}

InvoiceHent& SerializableSellInvoice::Hent()
{
	return m_pInvoice->m_Hent;
}

InvoiceDeductions& SerializableSellInvoice::Deductions()
{
	return m_pInvoice->m_Deductions;
}
	
void SerializableSellInvoice::serialize(SF::Archive &archive)
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


