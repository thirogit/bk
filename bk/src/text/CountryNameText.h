#ifndef __COUNTRYTEXT_H__
#define __COUNTRYTEXT_H__

#include <string>
#include <data/types/countries/Country.h>
#include <map>

class CountryNameText : private std::wstring
{
public:	
	CountryNameText(const Country& country);
	const std::wstring& ToString();
};

#endif