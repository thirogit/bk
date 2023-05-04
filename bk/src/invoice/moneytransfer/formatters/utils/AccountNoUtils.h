#ifndef __ACCOUNTNOUTILS_H__
#define __ACCOUNTNOUTILS_H__

#include <data\types\AccountNo.h>

class AccountNoUtils
{
public:
	static std::wstring GetBankNo(const AccountNo& accountNo);

};


#endif