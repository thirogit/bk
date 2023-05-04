#include "stdafx.h"
#include "ConstantField.h"


ConstantField::ConstantField(const std::wstring& sFieldValue) : m_FieldValue(sFieldValue)
{

}

std::wstring ConstantField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	return m_FieldValue;
}