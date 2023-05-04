#include "stdafx.h"
#include "CountryNameText.h"
#include "../utils/StringLoader.h"
#include "../resource.h"

CountryNameText::CountryNameText(const Country& country)
{
	struct COUNTRY_NAME_ASSOCIATION
	{
		UINT number;		
		UINT nNameResId;
	} 
	nameAssociations[] =
	{		
		{442	, IDS_COUNTRY_LUXEMBOURG},
		{208	, IDS_COUNTRY_DENMARK},
		{372	, IDS_COUNTRY_IRELAND},
		{32		, IDS_COUNTRY_AUSTRIA},
		{528	, IDS_COUNTRY_NETHERLANDS},
		{46		, IDS_COUNTRY_BELGIUM},
		{250	, IDS_COUNTRY_FRANCE},
		{276	, IDS_COUNTRY_GERMANY},
		{826	, IDS_COUNTRY_UNITEDKINGDOM},
		{380	, IDS_COUNTRY_ITALY},
		{246	, IDS_COUNTRY_FINLAND},
		{752	, IDS_COUNTRY_SWEDEN},
		{724	, IDS_COUNTRY_SPAIN},
		{300	, IDS_COUNTRY_GREECE},
		{196	, IDS_COUNTRY_CYPRUS},
		{705	, IDS_COUNTRY_SLOVENIA},
		{620	, IDS_COUNTRY_PORTUGAL},
		{370	, IDS_COUNTRY_MALTA},
		{203	, IDS_COUNTRY_CZECHREPUBLIC},
		{348	, IDS_COUNTRY_HUNGARY},
		{703	, IDS_COUNTRY_SLOVAKIA},
		{233	, IDS_COUNTRY_ESTONIA},
		{616	, IDS_COUNTRY_POLAND},
		{440	, IDS_COUNTRY_LITHUANIA},
		{428	, IDS_COUNTRY_LATVIA},
		{643	, IDS_COUNTRY_RUSSIA},
		{756	, IDS_COUNTRY_SWITZERLAND},
		{792	, IDS_COUNTRY_TURKEY},
		{578	, IDS_COUNTRY_NORWAY},
		{804	, IDS_COUNTRY_UKRAINE},
		{642	, IDS_COUNTRY_ROMANIA},
		{398	, IDS_COUNTRY_KAZAKHSTAN},
		{191	, IDS_COUNTRY_CROATIA},
		{112	, IDS_COUNTRY_BELARUS},	
		{100	, IDS_COUNTRY_BULGARIA},
		{352	, IDS_COUNTRY_ICELAND},
		{31		, IDS_COUNTRY_AZERBAIJAN},
		{807	, IDS_COUNTRY_MACEDONIA},
		{239	, IDS_COUNTRY_GEORGIA},
		{51		, IDS_COUNTRY_ARMENIA},
		{20		, IDS_COUNTRY_ANDORRA},
		{438	, IDS_COUNTRY_LIECHTENSTEIN},
		{492	, IDS_COUNTRY_MONACO},
		{674	, IDS_COUNTRY_SANMARINO},
		{0		, -1}
	};
		
	COUNTRY_NAME_ASSOCIATION* pAssociation = nameAssociations;
	while(pAssociation->number)
	{
		if(country.GetCountryISONumber() == pAssociation->number)
		{
			std::wstring::operator=(StringLoader(pAssociation->nNameResId));
			break;
		}
		pAssociation++;
	}

}

const std::wstring& CountryNameText::ToString()
{
	return *this;
}

