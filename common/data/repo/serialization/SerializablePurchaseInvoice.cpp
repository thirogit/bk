#include "stdafx.h"
#include "SerializablePurchaseInvoice.h"
#include <CommonTypes.h>
#include "SerializablePurchaseCow.h"
#include "SerializablePurchaseHent.h"
#include "SerializablePurchaseInvoiceDeduction.h"
#include "../../types/serialization/TypesSerialization.h"

SerializablePurchaseInvoice::SerializablePurchaseInvoice(PurchaseInvoice* pInvoice) : m_pInvoice(pInvoice)
{

}

void SerializablePurchaseInvoice::serialize(SF::Archive &archive)
{
	archive & Id();
	archive & InvoiceDate();
	archive & TransactionDate();
	archive & TransactionPlace();
	archive & VatRate();
	archive & PaymentWay();
	archive & CustomNumber();
	archive & PayDueDays();
	archive & TypeOfInvoice();

	SerializablePurchaseHent(&m_pInvoice->m_InvoiceHent).serialize(archive);

	count_t cowCount,deductionsCount;

	if (archive.isRead())
	{
		archive & cowCount;

		for (count_t i = 0; i < cowCount; i++)
		{
			PurchaseCow* cow = new PurchaseCow(m_pInvoice->ObtainNewCowId());
			SerializablePurchaseCow(cow).serialize(archive);
			m_pInvoice->m_Cows.add(cow);
		}

		archive & deductionsCount;

		for (count_t i = 0; i < deductionsCount; i++)
		{
			PurchaseInvoiceDeduction* deduction = new PurchaseInvoiceDeduction(m_pInvoice->ObtainNewDeductionId());
			SerializablePurchaseInvoiceDeduction(deduction).serialize(archive);
			m_pInvoice->m_Deductions.add(deduction);
		}
	} 
	else
	{
		cowCount = m_pInvoice->GetCowCount();
		archive & cowCount;

		for (int i = 0; i < cowCount; i++)
		{
			SerializablePurchaseCow(m_pInvoice->GetCowAt(i)).serialize(archive);
		}

		deductionsCount = m_pInvoice->GetDeductionsCount();
		archive & deductionsCount;

		for (int i = 0; i < deductionsCount; i++)
		{
			SerializablePurchaseInvoiceDeduction(m_pInvoice->GetDeductionAt(i)).serialize(archive);
		}
	}	
}

std::wstring& SerializablePurchaseInvoice::Id()
{
	return m_pInvoice->m_id;
}


DateTime& SerializablePurchaseInvoice::InvoiceDate()
{
	return m_pInvoice->m_InvoiceDate;
}

DateTime& SerializablePurchaseInvoice::TransactionDate()
{
	return m_pInvoice->m_TransactionDate;
}

std::wstring& SerializablePurchaseInvoice::TransactionPlace()
{
	return m_pInvoice->m_TransactionPlace;
}

Decimal& SerializablePurchaseInvoice::VatRate()
{
	return m_pInvoice->m_VatRate;
}

PayWay& SerializablePurchaseInvoice::PaymentWay()
{
	return m_pInvoice->m_PayWay;
}

std::wstring& SerializablePurchaseInvoice::CustomNumber()
{
	return m_pInvoice->m_CustomNumber;
}

int& SerializablePurchaseInvoice::PayDueDays()
{
	return m_pInvoice->m_PayDueDays;
}

InvoiceType& SerializablePurchaseInvoice::TypeOfInvoice()
{
	return m_pInvoice->m_InvoiceType;
}

