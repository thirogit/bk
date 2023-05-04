#include "stdafx.h"
#include "InvoiceKind.h"


InvoiceKind::InvoiceKind() : m_type(InvoiceType_None), m_direction(InvoiceDirection_None)
{
}

InvoiceKind::InvoiceKind(const InvoiceKind& src)
{
	m_type = src.m_type;
	m_direction = src.m_direction;
}

InvoiceKind& InvoiceKind::operator=(const InvoiceKind& right)
{
	m_type = right.m_type;
	m_direction = right.m_direction;
	return *this;
}

InvoiceKind::InvoiceKind(InvoiceDirection direction, InvoiceType type) : m_type(type), m_direction(direction)
{
}

InvoiceDirection InvoiceKind::GetDirection() const
{
	return m_direction;
}

InvoiceType InvoiceKind::GetInvoiceType() const
{
	return m_type;
}

void InvoiceKind::SetDirection(InvoiceDirection direction)
{
	m_direction = direction;
}

void InvoiceKind::SetInvoiceType(InvoiceType type)
{
	m_type = type;
}


