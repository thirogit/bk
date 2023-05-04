#ifndef __COUNTRY_H__
#define __COUNTRY_H__

#include <string>
#include "../../../types/Nullable.h"
#include <boost/optional.hpp>

class Countries;

class Country : public Nullable
{
	class InternalCountry
	{
	public:
		std::wstring code2a;
		std::wstring code3a;
		unsigned int isoNumber;
		bool		inEU;
		std::wstring currencyCode;
		int			cowNoLength;
		int			farmNoLength;
		int			herdNoLength;
	};

public:
	Country();
	Country(const Country& copy);
	Country& operator=(const Country& right);

	virtual void MakeNull();
	virtual bool IsNull() const;

	unsigned int GetCountryISONumber() const;
	bool IsCountryInEU() const;
	std::wstring GetCountryCode2A() const;
	std::wstring GetCountryCode3A() const;
	std::wstring GetCurrencyCode() const;
	int GetCowNoLength() const;
	int GetFarmNoLength() const;
	int GetHerdNoLength() const;
		

private:
	
	boost::optional<InternalCountry> m_Value;

	friend class Countries;	
};

#endif