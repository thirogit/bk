#include "stdafx.h"
#include "SerializablePurchaseInvoiceDeduction.h"
#include <CommonTypes.h>
#include "../../types/serialization/TypesSerialization.h"

SerializablePurchaseInvoiceDeduction::SerializablePurchaseInvoiceDeduction(PurchaseInvoiceDeduction* pInvoiceDeduction) : m_pDeduction(pInvoiceDeduction)
{

}

void SerializablePurchaseInvoiceDeduction::serialize(SF::Archive &archive)
{
	archive & Id();
	archive & Code();
	archive & Reason();
	archive & Fraction();
	archive & IsEnabled();
}

std::wstring& SerializablePurchaseInvoiceDeduction::Id()
{
	return m_pDeduction->m_id;
}

std::wstring& SerializablePurchaseInvoiceDeduction::Code()
{
	return m_pDeduction->m_code;
}

std::wstring& SerializablePurchaseInvoiceDeduction::Reason()
{
	return m_pDeduction->m_reason;
}

Decimal& SerializablePurchaseInvoiceDeduction::Fraction()
{
	return m_pDeduction->m_fraction;
}

bool& SerializablePurchaseInvoiceDeduction::IsEnabled()
{
	return m_pDeduction->m_enabled;
}



