#ifndef __COWNO_H__
#define __COWNO_H__

#include <string>
#include "countries/Country.h"
#include "../../types/Nullable.h"
#include <boost/optional.hpp>
#include "../../exception/TagException.h"

struct tag_CowNoException{};

typedef TagException<tag_CowNoException> CowNoException;


class CowNo;

class CowNoFactory
{
public:
	static CowNo Create(const Country& country,const std::wstring& number);
	static CowNo FromString(const std::wstring& sCowNo);
};

class CowNo : public Nullable
{
	class InternalCowNo
	{
	public:
		InternalCowNo();
		Country m_Country;
		std::wstring m_Number;	

		bool operator==(const InternalCowNo& right) const;
	};

	CowNo(Country country,const std::wstring& number);
public:
	CowNo();
	CowNo(const CowNo& cowNo);
	std::wstring ToString() const;
	CowNo& operator=(const CowNo& right);
	Country GetCountry() const;
	std::wstring GetNumber() const;
	void CopyFrom(const CowNo& src);
	virtual void MakeNull();
	virtual bool IsNull() const;
protected:
	boost::optional<InternalCowNo> m_Value;
	
	friend class CowNoFactory;	
	friend bool operator==(const CowNo& left, const CowNo& right);
};

bool operator>(const CowNo& left, const CowNo& right);
bool operator<(const CowNo& left, const CowNo& right);
bool operator!=(const CowNo& left, const CowNo& right);

#endif
