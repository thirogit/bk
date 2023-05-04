#ifndef __IBANKACCOUNT_H__
#define __IBANKACCOUNT_H__

#include "types/AccountNo.h"
#include "IIdable.h"

class IBankAccount : public IIdable
{
public:
	virtual const std::wstring& GetBankName() const = 0;
	virtual const AccountNo& GetAccountNo() const = 0;
	virtual const std::wstring& GetAlias() const = 0;
	
};

#endif