#include "stdafx.h"
#include "AccountNoUtils.h"

 std::wstring AccountNoUtils::GetBankNo(const AccountNo& accountNo)
{
	 std::wstring sAccountNo = accountNo.GetNumber();

	if(sAccountNo.length() == 26)
	{
		return sAccountNo.substr(2,8);
	}
	return L"########";

}