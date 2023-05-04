#include "stdafx.h"
#include "InvoiceKey.h"


InvoiceKey::InvoiceKey() : m_direction(InvoiceDirection_None),m_invoiceId(NULL_ID)
{
}

InvoiceKey::InvoiceKey(const InvoiceKey& key)
{
	operator=(key);
}

InvoiceKey& InvoiceKey::operator=(const InvoiceKey& right)
{
	m_direction = right.m_direction;
	m_invoiceId = right.m_direction;
	return *this;
}
	
InvoiceKey::InvoiceKey(InvoiceDirection direction,uint32_t invoiceId) : m_direction(direction),m_invoiceId(invoiceId)
{
}

InvoiceDirection InvoiceKey::GetDirection() const
{
	return m_direction;
}

uint32_t InvoiceKey::GetInvoiceId() const
{
	return m_invoiceId;
}

void InvoiceKey::SetDirection(InvoiceDirection direction)
{
	m_direction = direction;
}

void InvoiceKey::SetInvoiceId(uint32_t invoiceId)
{
	m_invoiceId = invoiceId;
}

