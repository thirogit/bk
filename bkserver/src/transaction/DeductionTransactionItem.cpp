#include "stdafx.h"
#include "DeductionTransactionItem.h"

DeductionTransactionItem::DeductionTransactionItem(const ItemId& id) : m_id(id)
{
}

ItemId DeductionTransactionItem::GetId() const
{
	return m_id;
}

void DeductionTransactionItem::SetDeduction(const InvoiceDeduction& deduction)
{
	m_deduction = deduction;
}

const InvoiceDeduction& DeductionTransactionItem::GetDeduction() const
{
	return m_deduction;
}