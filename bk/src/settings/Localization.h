#pragma once

#include <stdint.h>
#include <data/types/Country.h>

class HandyCountries
{
public:
	bool country1Enabled;
	bool country2Enabled;
	bool country3Enabled;	

	uint32_t country1;
	uint32_t country2;
	uint32_t country3;	
};


class Localization
{
public:
	Localization();
	Localization(const Localization& src);
	Localization& operator=(const Localization& right);

	
	const HandyCountries& GetHandyCountries() const;
	void SetHandyCountries(const HandyCountries& countries);
	
private:
	HandyCountries m_handyCountries;	

};


	

