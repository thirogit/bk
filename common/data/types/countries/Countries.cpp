#include "stdafx.h"
#include "Countries.h"


Countries Countries::instance;

Countries::Countries()
{
	struct COUNTRY_DESCRIPTOR
	{
		unsigned int number;		
		const wchar_t* code2a;
		const wchar_t* code3a;
		bool eu_memeber;
		const wchar_t* currency_code;
		int cowno_length;		
		int farmno_length;
		int herdno_length;
	} 
	countryDescriptors[] =
	{		
		{442	,	L"LU",	L"LUX",	true	,L"EUR",	14, 11, 3},
		{208	,	L"DK",	L"DNK",	true	,L"DKK",	13, 11, 3},
		{372	,	L"IE",	L"IRL",	true	,L"EUR",	14, 11, 3},
		{32		,	L"AT",	L"AUT",	true	,L"EUR",	11, 11, 3},
		{528	,	L"NL",	L"NLD",	true	,L"EUR",	11, 11, 3},
		{46		,	L"BE",	L"BEL",	true	,L"EUR",	11, 11, 3},
		{250	,	L"FR",	L"FRA",	true	,L"EUR",	12, 11, 3},
		{276	,	L"DE",	L"DEU",	true	,L"EUR",	12, 11, 3},
		{826	,	L"GB",	L"GBR",	true	,L"GBP",	14, 11, 3},
		{380	,	L"IT",	L"ITA",	true	,L"EUR",	14, 11, 3},
		{246	,	L"FI",	L"FIN",	true	,L"EUR",	14, 11, 3},
		{752	,	L"SE",	L"SWE",	true	,L"SEK",	14, 11, 3},
		{724	,	L"ES",	L"ESP",	true	,L"EUR",	14, 11, 3},
		{300	,	L"GR",	L"GRC",	true	,L"EUR",	14, 11, 3},
		{196	,	L"CY",	L"CYP",	true	,L"EUR",	14, 11, 3},
		{705	,	L"SI",	L"SVN",	true	,L"EUR",	14, 11, 3},
		{620	,	L"PT",	L"PRT",	true	,L"EUR",	14, 11, 3},
		{370	,	L"MT",	L"MLT",	true	,L"EUR",	14, 11, 3},
		{203	,	L"CZ",	L"CZE",	true	,L"CZK",	11, 11, 3},
		{348	,	L"HU",	L"HUN",	true	,L"HUF",	14, 11, 3},
		{703	,	L"SK",	L"SVK",	true	,L"EUR",	11, 11, 3},
		{233	,	L"EE",	L"EST",	true	,L"EUR",	12, 11, 3},
		{616	,	L"PL",	L"POL",	true	,L"PLN",	14, 11, 3},
		{440	,	L"LT",	L"LTU",	true	,L"LTL",	14, 11, 3},
		{428	,	L"LV",	L"LVA",	true	,L"LVL",	14, 11, 3},
		{643	,	L"RU",	L"RUS",	false	,L"RUB",	14, 11, 3},
		{756	,	L"CH",	L"CHE",	false	,L"CHF",	14, 11, 3},
		{792	,	L"TR",	L"TUR",	false	,L"TRY",	14, 11, 3},
		{578	,	L"NO",	L"NOR",	false	,L"NOK",	14, 11, 3},
		{804	,	L"UA",	L"UKR",	false	,L"UAH",	14, 11, 3},
		{642	,	L"RO",	L"ROU",	false	,L"RON",	14, 11, 3},
		{398	,	L"KZ",	L"KAZ",	false	,L"KZT",	14, 11, 3},
		{191	,	L"HR",	L"HRV",	true	,L"HRK",	14, 11, 3},
		{112	,	L"BY",	L"BLR",	false	,L"BYR",	14, 11, 3},		
		{100	,	L"BG",	L"BGR",	false	,L"BGN",	14, 11, 3},
		{352	,	L"IS",	L"ISL",	false	,L"ISK",	14, 11, 3},
		{31		,	L"AZ",	L"AZE",	false	,L"AZN",	14, 11, 3},
		{807	,	L"MK",	L"MKD",	false	,L"MKD",	14, 11, 3},
		{239	,	L"GS",	L"SGS",	false	,L"GEL",	14, 11, 3},
		{51		,	L"AM",	L"ARM",	false	,L"AMD",	14, 11, 3},
		{20		,	L"AD",	L"AND",	false	,L"EUR",	14, 11, 3},
		{438	,	L"LI",	L"LIE",	false	,L"CHF",	14, 11, 3},
		{492	,	L"MC",	L"MCO",	false	,L"EUR",	14, 11, 3},
		{674	,	L"SM",	L"SMR",	false	,L"EUR",	14, 11, 3},
		{0		,	NULL,	NULL,	false	,NULL ,	0}
	};
	
	

	Country* pCountry;
	COUNTRY_DESCRIPTOR* pCountryDescriptor = countryDescriptors;
	while(pCountryDescriptor->number)
	{
		
		Country::InternalCountry internalCountry;
		internalCountry.isoNumber = pCountryDescriptor->number;
		internalCountry.code2a = pCountryDescriptor->code2a;
		internalCountry.code3a = pCountryDescriptor->code3a;
		internalCountry.inEU = pCountryDescriptor->eu_memeber;
		internalCountry.currencyCode = pCountryDescriptor->currency_code;
		internalCountry.cowNoLength = pCountryDescriptor->cowno_length;
		internalCountry.farmNoLength = pCountryDescriptor->farmno_length;
		internalCountry.herdNoLength = pCountryDescriptor->herdno_length;

		pCountry = new Country();
		pCountry->m_Value = internalCountry;
		countriesIndex.insert(pCountry);


		pCountryDescriptor++;
	}	
}

Countries::~Countries()
{
	for(CountriesIndex::iterator it = countriesIndex.begin();it != countriesIndex.end();it++)
		delete it.get_node()->value();

}

Countries::Enumerator Countries::EnumCountries()
{
	return Countries::Enumerator(instance.countriesIndex);
}

const Country Countries::GetCountryByNumber(unsigned int number)
{
	const boost::multi_index::index<CountriesIndex,index_tag_isonumber>::type& indexByNumber = boost::multi_index::get<index_tag_isonumber>(instance.countriesIndex);

	auto countryIt = indexByNumber.find(number);

	if(countryIt != indexByNumber.end())
		return *(*countryIt);

	BOOST_THROW_EXCEPTION(CountriesException(L"unk_country_num"));
}
const Country Countries::GetCountryByCode2a(const std::wstring& code2a)
{
	const boost::multi_index::index<CountriesIndex,index_tag_code2a>::type& indexByCode2Alpha = boost::multi_index::get<index_tag_code2a>(instance.countriesIndex);

	auto countryIt = indexByCode2Alpha.find(code2a);

	if(countryIt != indexByCode2Alpha.end())
		return *(*countryIt);

	BOOST_THROW_EXCEPTION(CountriesException(L"unk_country_cd"));
}

