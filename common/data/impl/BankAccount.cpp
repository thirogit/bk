#include "stdafx.h"
#include "BankAccount.h"

BankAccount::BankAccount(uint32_t id) : m_id(id)
{
}

BankAccount::BankAccount(const IBankAccount* bankAccount)
{
	m_id = bankAccount->GetId();
	CopyFrom(bankAccount);	
}

void BankAccount::CopyFrom(const IBankAccount* bankAccount)
{
	m_bankname = bankAccount->GetBankName();
	m_accountNo = bankAccount->GetAccountNo();
	m_alias = bankAccount->GetAlias();
}

uint32_t BankAccount::GetId() const
{
	return m_id;
}

const std::wstring& BankAccount::GetBankName() const
{
	return m_bankname;
}

const AccountNo& BankAccount::GetAccountNo() const
{
	return m_accountNo;
}

const std::wstring& BankAccount::GetAlias() const
{
	return m_alias;
}


void BankAccount::SetBankName(const std::wstring& bankname) 
{
	m_bankname = bankname;
}

void BankAccount::SetAccountNo(const AccountNo& accountno) 
{
	m_accountNo = accountno;
}


void BankAccount::SetAlias(const std::wstring& alias) 
{
	m_alias = alias;
}

