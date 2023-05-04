#ifndef __BANKACCOUNT_H__
#define __BANKACCOUNT_H__

#include "../IBankAccount.h"

class BankAccountSerializer;

class BankAccount : public IBankAccount
{
public:
	BankAccount(uint32_t id);
	BankAccount(const IBankAccount* bankAccount);
	void CopyFrom(const IBankAccount* bankAccount);

	virtual uint32_t GetId() const;
	virtual const std::wstring& GetBankName() const;
	virtual const AccountNo& GetAccountNo() const;
	virtual const std::wstring& GetAlias() const;

	
	void SetBankName(const std::wstring& bankname) ;
	void SetAccountNo(const AccountNo& accountno) ;
	void SetAlias(const std::wstring& alias) ;


private:
	uint32_t m_id;
	std::wstring m_bankname;
	AccountNo m_accountNo;
	std::wstring m_alias;

	friend class BankAccountSerializer;
	
};

#endif