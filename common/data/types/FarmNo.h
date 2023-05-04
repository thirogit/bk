#ifndef __FARMNO_H__
#define __FARMNO_H__

#include <string>
#include "countries/Country.h"
#include "../../types/Nullable.h"
#include <boost/optional.hpp>
#include "../../exception/TagException.h"

struct tag_FarmNoException{};
typedef TagException<tag_FarmNoException> FarmNoException;

class FarmNo;

class FarmNoFactory
{
public:
	static FarmNo Create(const Country& country,const std::wstring& number);
	static FarmNo FromString(const std::wstring& sFarmNo);
};

class FarmNo : public Nullable
{
	class InternalFarmNo
	{
	public:
		InternalFarmNo();
		Country m_Country;
		std::wstring m_Number;	
		bool operator==(const InternalFarmNo& right) const;
	};

	FarmNo(Country country,const std::wstring& number);
public:
	FarmNo();
	FarmNo(const FarmNo& farmNo);
	std::wstring ToString() const;
	FarmNo& operator=(const FarmNo& right);
	Country GetCountry() const;
	std::wstring GetNumber() const;
	void CopyFrom(const FarmNo& src);

	virtual void MakeNull();
	virtual bool IsNull() const;

protected:
	boost::optional<InternalFarmNo> m_Value;
	
	friend bool operator>(const FarmNo& left, const FarmNo& right);
	friend bool operator<(const FarmNo& left, const FarmNo& right);
	friend bool operator==(const FarmNo& left, const FarmNo& right);
	friend class FarmNoFactory;	
};


#endif
