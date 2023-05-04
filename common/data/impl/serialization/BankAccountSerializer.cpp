#include "stdafx.h"
#include "BankAccountSerializer.h"
#include "../../types/serialization/TypesSerialization.h"

BankAccountSerializer::BankAccountSerializer(BankAccount* pBankAccount) : m_pBankAccount(pBankAccount)
{
}


std::wstring& BankAccountSerializer::BankName()
{
	return m_pBankAccount->m_bankname;
}

AccountNo& BankAccountSerializer::AccountNumber()
{
	return m_pBankAccount->m_accountNo;
}	
uint32_t& BankAccountSerializer::Id()
{
	return m_pBankAccount->m_id;
}

std::wstring& BankAccountSerializer::Alias()
{
	return m_pBankAccount->m_alias;
}

void BankAccountSerializer::serialize(SF::Archive &archive)
{
	archive & Id();
	archive & BankName();
	archive & AccountNumber();
	archive & Alias();		
}