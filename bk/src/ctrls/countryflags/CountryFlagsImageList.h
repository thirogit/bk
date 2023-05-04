#ifndef __COUNTRYFLAGSIMAGELIST_H__
#define __COUNTRYFLAGSIMAGELIST_H__

#include <map>
#include <stdint.h>
#include "../../utils/BmpImageList.h"

class CountryFlagsImageList
{		
public:
	CountryFlagsImageList();
	~CountryFlagsImageList();	
	HICON GetFlagByCountryNumber(uint32_t number);	
	HBITMAP GetFlagBitmap(uint32_t countryNumber,COLORREF clrBkGrnd);
	int GetCountryFlagIndex(uint32_t countryNumber);
	
private:
	void LazyInitFlagImageList();
	CBmpImageList* m_pFlagImageList;
	std::map<uint32_t,uint32_t> m_FlagsToCountry;
};



#endif