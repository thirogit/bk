#include "stdafx.h"
#include "Farms.h"
#include <boost/foreach.hpp>
#include <arrays\DeepDelete.h>

FarmAccount::FarmAccount(uint32_t accountId) : m_Id(accountId),m_port(0)
{
}

FarmAccount::FarmAccount(const FarmAccount& src) : m_Id(src.m_Id)
{
	CopyFrom(src);
}

void FarmAccount::CopyFrom(const FarmAccount& right)
{
	m_accountName = right.m_accountName;
	m_address = right.m_address;
	m_port = right.m_port;
	m_username = right.m_username;
	m_password = right.m_password;
	m_farmNo = right.m_farmNo;
}	


uint32_t FarmAccount::GetId() const
{
	return m_Id;
}

const FarmNo& FarmAccount::GetFarm() const
{
	return m_farmNo;
}

void FarmAccount::SetName(const std::wstring& accountName)
{
	m_accountName = accountName;
}

void FarmAccount::SetFarm(const FarmNo& farmNo)
{
	m_farmNo = farmNo;
}

void FarmAccount::SetAddress(const std::wstring& address)
{
	m_address = address;
}

void FarmAccount::SetPort(uint16_t port)
{
	m_port = port;
}

void FarmAccount::SetLogin(const std::wstring& username)
{
	m_username = username;
}

void FarmAccount::SetPassword(const std::wstring& password)
{
	m_password = password;
}

const std::wstring& FarmAccount::GetName() const
{
	return m_accountName;
}

const std::wstring& FarmAccount::GetAddress() const
{
	return m_address;
}

uint16_t FarmAccount::GetPort() const
{
	return m_port;
}

const std::wstring& FarmAccount::GetLogin() const
{
	return m_username;
}

const std::wstring& FarmAccount::GetPassword() const
{
	return m_password;
}

//#################################################################################################

   
uint32_t FarmAccounts::ObtainNewId()
{
	uint32_t maxid = 0;
	BOOST_FOREACH(FarmAccount* account,m_accounts)
	{
		maxid = max(maxid,account->GetId());
	}

	return maxid+1;
}

FarmAccounts::FarmAccounts()
{
}

FarmAccounts::FarmAccounts(FarmAccounts&& src)
{
	MoveFrom(src);
}

void FarmAccounts::MoveFrom(FarmAccounts& src)
{
	std::copy(src.m_accounts.begin(),src.m_accounts.end(),std::back_inserter(m_accounts));
	src.m_accounts.clear();
}

FarmAccounts& FarmAccounts::operator=(FarmAccounts&& right)
{
	MoveFrom(right);
	return *this;
}

FarmAccounts::~FarmAccounts()
{
	DeepDelete(m_accounts);
}


PtrEnumerator<const FarmAccount> FarmAccounts::EnumAccounts() const
{
	return PtrEnumerator<const FarmAccount>(m_accounts);
}

FarmAccounts::accounts_container::iterator FarmAccounts::FindAccountWithId(uint32_t id)
{
	auto it = std::find_if(m_accounts.begin(),m_accounts.end(),[id](FarmAccount* account) -> bool
																 {
																	 return account->GetId() == id;
																 });

	return it;
}

void FarmAccounts::DeleteAccount(uint32_t id)
{
	auto it = FindAccountWithId(id);

	if(it != m_accounts.end())
	{
		delete *it;
		m_accounts.erase(it);
	}
}

uint32_t FarmAccounts::AddAccount(const FarmAccount& account)
{
	uint32_t id = ObtainNewId();
	FarmAccount* newAccount = new FarmAccount(id);
	newAccount->CopyFrom(account);
	m_accounts.push_back(newAccount);
	return id;
}

void FarmAccounts::UpdateAccount(const FarmAccount& account)
{
	auto it = FindAccountWithId(account.GetId());
	if(it != m_accounts.end())
	{
		(*it)->CopyFrom(account);
	}
}

FarmAccount* FarmAccounts::FindAccount(uint32_t accountId)
{
	auto it = FindAccountWithId(accountId);
	if(it != m_accounts.end())
	{
		return (*it);
	}

	return NULL;
}