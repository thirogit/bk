#include "stdafx.h"
#include "Countries.h"
#include <exception\LocalizedException.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

Countries Countries::instance;

Countries::Countries()
{
	struct COUNTRY_DESCRIPTOR
	{
		unsigned int number;		
		const char* code2a;
		const char* code3a;
		bool eu_memeber;
		const char* currency_code;
		int cowno_length;		
		int farmno_length;
		int herdno_length;
	} 
	countryDescriptors[] =
	{		
		{442	,	"LU",	"LUX",	true	,"EUR",	14, 11, 3},
		{208	,	"DK",	"DNK",	true	,"DKK",	14, 11, 3},
		{372	,	"IE",	"IRL",	true	,"EUR",	14, 11, 3},
		{32		,	"AT",	"AUT",	true	,"EUR",	14, 11, 3},
		{528	,	"NL",	"NLD",	true	,"EUR",	14, 11, 3},
		{46		,	"BE",	"BEL",	true	,"EUR",	14, 11, 3},
		{250	,	"FR",	"FRA",	true	,"EUR",	14, 11, 3},
		{276	,	"DE",	"DEU",	true	,"EUR",	14, 11, 3},
		{826	,	"GB",	"GBR",	true	,"GBP",	14, 11, 3},
		{380	,	"IT",	"ITA",	true	,"EUR",	14, 11, 3},
		{246	,	"FI",	"FIN",	true	,"EUR",	14, 11, 3},
		{752	,	"SE",	"SWE",	true	,"SEK",	14, 11, 3},
		{724	,	"ES",	"ESP",	true	,"EUR",	14, 11, 3},
		{300	,	"GR",	"GRC",	true	,"EUR",	14, 11, 3},
		{196	,	"CY",	"CYP",	true	,"EUR",	14, 11, 3},
		{705	,	"SI",	"SVN",	true	,"EUR",	14, 11, 3},
		{620	,	"PT",	"PRT",	true	,"EUR",	14, 11, 3},
		{370	,	"MT",	"MLT",	true	,"EUR",	14, 11, 3},
		{203	,	"CZ",	"CZE",	true	,"CZK",	14, 11, 3},
		{348	,	"HU",	"HUN",	true	,"HUF",	14, 11, 3},
		{703	,	"SK",	"SVK",	true	,"EUR",	14, 11, 3},
		{233	,	"EE",	"EST",	true	,"EUR",	14, 11, 3},
		{616	,	"PL",	"POL",	true	,"PLN",	14, 11, 3},
		{440	,	"LT",	"LTU",	true	,"LTL",	14, 11, 3},
		{428	,	"LV",	"LVA",	true	,"LVL",	14, 11, 3},
		{643	,	"RU",	"RUS",	false	,"RUB",	14, 11, 3},
		{756	,	"CH",	"CHE",	false	,"CHF",	14, 11, 3},
		{792	,	"TR",	"TUR",	false	,"TRY",	14, 11, 3},
		{578	,	"NO",	"NOR",	false	,"NOK",	14, 11, 3},
		{804	,	"UA",	"UKR",	false	,"UAH",	14, 11, 3},
		{642	,	"RO",	"ROU",	false	,"RON",	14, 11, 3},
		{398	,	"KZ",	"KAZ",	false	,"KZT",	14, 11, 3},
		{191	,	"HR",	"HRV",	true	,"HRK",	14, 11, 3},
		{112	,	"BY",	"BLR",	false	,"BYR",	14, 11, 3},		
		{100	,	"BG",	"BGR",	false	,"BGN",	14, 11, 3},
		{352	,	"IS",	"ISL",	false	,"ISK",	14, 11, 3},
		{31		,	"AZ",	"AZE",	false	,"AZN",	14, 11, 3},
		{807	,	"MK",	"MKD",	false	,"MKD",	14, 11, 3},
		{239	,	"GS",	"SGS",	false	,"GEL",	14, 11, 3},
		{51		,	"AM",	"ARM",	false	,"AMD",	14, 11, 3},
		{20		,	"AD",	"AND",	false	,"EUR",	14, 11, 3},
		{438	,	"LI",	"LIE",	false	,"CHF",	14, 11, 3},
		{492	,	"MC",	"MCO",	false	,"EUR",	14, 11, 3},
		{674	,	"SM",	"SMR",	false	,"EUR",	14, 11, 3},
		{0		,	NULL,	NULL,	false	,NULL ,	0}
	};
	
	

	Country* pCountry;
	COUNTRY_DESCRIPTOR* pCountryDescriptor = countryDescriptors;
	while(pCountryDescriptor->number)
	{
		pCountry = new Country(pCountryDescriptor->number);
		pCountry->SetCountryCode2A(pCountryDescriptor->code2a);
		pCountry->SetCountryCode3A(pCountryDescriptor->code3a);
		pCountry->SetCountryInEU(pCountryDescriptor->eu_memeber);
		pCountry->SetCurrencyCode(pCountryDescriptor->currency_code);		
		pCountry->SetCowNoLength(pCountryDescriptor->cowno_length);
		pCountry->SetFarmNoLength(pCountryDescriptor->farmno_length);
		pCountry->SetHerdNoLength(pCountryDescriptor->herdno_length);
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

	throw LocalizedException("unk_country_num");
}
const Country Countries::GetCountryByCode2a(const std::string& code2a)
{
	const boost::multi_index::index<CountriesIndex,index_tag_code2a>::type& indexByCode2Alpha = boost::multi_index::get<index_tag_code2a>(instance.countriesIndex);

	auto countryIt = indexByCode2Alpha.find(code2a);

	if(countryIt != indexByCode2Alpha.end())
		return *(*countryIt);

	throw LocalizedException("unk_country_cd");
}

