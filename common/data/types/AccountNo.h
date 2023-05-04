#ifndef __ACCOUNTNO_H__
#define __ACCOUNTNO_H__

#include <string>
#include "countries/Country.h"
#include "../../types/Nullable.h"
#include <boost/optional.hpp>
#include "../../exception/TagException.h"

struct tag_AccountNoException{};

typedef TagException<tag_AccountNoException> AccountNoException;


class AccountNo;

class AccountNoFactory
{
public:
	static AccountNo Create(const Country& country,const std::wstring& number);
	static AccountNo FromString(const std::wstring& sAccountNo);
};

class AccountNo : public Nullable
{
	class InternalAccountNo
	{
	public:
		InternalAccountNo();
		Country m_Country;
		std::wstring m_Number;	
	};

	AccountNo(const Country& country,const std::wstring& number);
public:
	AccountNo();
	AccountNo(const AccountNo& accountNo);
	std::wstring ToString() const;
	AccountNo& operator=(const AccountNo& right);
	Country GetCountry() const;
	std::wstring GetNumber() const;

	virtual void MakeNull();
	virtual bool IsNull() const;

protected:
	boost::optional<InternalAccountNo> m_Value;
#if _DEBUG
	std::wstring m_stringValue;
#endif

	friend class AccountNoFactory;
	
};

bool operator>=(const AccountNo& left, const AccountNo& right);
bool operator<=(const AccountNo& left, const AccountNo& right);
bool operator>(const AccountNo& left, const AccountNo& right);
bool operator<(const AccountNo& left, const AccountNo& right);
bool operator==(const AccountNo& left, const AccountNo& right);
bool operator!=(const AccountNo& left, const AccountNo& right);

#endif
