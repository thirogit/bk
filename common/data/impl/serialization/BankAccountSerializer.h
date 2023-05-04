#pragma once 

#include <SF/Archive.hpp>
#include <SF/string.hpp> 

#include "../BankAccount.h"

class BankAccountSerializer
{
public:
	BankAccountSerializer(BankAccount* pBankAccount);
	void serialize(SF::Archive &archive);
protected:

	uint32_t&    Id();	
	std::wstring& BankName();
	AccountNo&   AccountNumber();
	std::wstring& Alias();		
private:
	BankAccount* m_pBankAccount;
};
