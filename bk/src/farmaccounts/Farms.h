#pragma once

#include <vector>
#include <stdint.h>
#include <arrays\Enumerator.h>
#include <data\types\FarmNo.h>

class FarmAccount
{
public:
	FarmAccount(uint32_t accountId);
	FarmAccount(const FarmAccount& src);

	void CopyFrom(const FarmAccount& right);
	
	void SetName(const std::wstring& accountName);
	void SetFarm(const FarmNo& farmNo);
	void SetAddress(const std::wstring& address);
	void SetPort(uint16_t port);
	void SetLogin(const std::wstring& login);
	void SetPassword(const std::wstring& password);

	const std::wstring& GetName() const;
	const FarmNo& GetFarm() const;
	const std::wstring& GetAddress() const;
	uint16_t GetPort() const;
	const std::wstring& GetLogin() const;
	const std::wstring& GetPassword() const;
	uint32_t GetId() const;

private:
	uint32_t 	m_Id;
	std::wstring m_accountName;
	std::wstring m_address;
	uint16_t m_port;
	std::wstring m_username;
	std::wstring m_password;	
	FarmNo m_farmNo;
};

class FarmAccounts
{
	typedef std::vector<FarmAccount*> accounts_container;
    accounts_container m_accounts;
    
	void MoveFrom(FarmAccounts& src);
	uint32_t ObtainNewId();
	accounts_container::iterator FindAccountWithId(uint32_t id);
public:
	FarmAccounts();
	~FarmAccounts();
	FarmAccounts(FarmAccounts&& src);

	FarmAccounts& operator=(FarmAccounts&& right);

    PtrEnumerator<const FarmAccount> EnumAccounts() const;
	void DeleteAccount(uint32_t id);
	uint32_t AddAccount(const FarmAccount& account);
	void UpdateAccount(const FarmAccount& account);
	FarmAccount* FindAccount(uint32_t accountId);
	
};